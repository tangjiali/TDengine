/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "executorInt.h"
#include "filter.h"
#include "function.h"
#include "functionMgt.h"
#include "operator.h"
#include "querytask.h"
#include "streamexecutorInt.h"
#include "tchecksum.h"
#include "tcommon.h"
#include "tcompare.h"
#include "tdatablock.h"
#include "tfill.h"
#include "tglobal.h"
#include "tlog.h"
#include "ttime.h"


static int32_t doStreamIntervalNonblockAggImpl(SOperatorInfo* pOperator, SSDataBlock* pSDataBlock, uint64_t groupId,
                                               SSHashObj* pUpdatedMap, SSHashObj* pDeletedMap) {
  int32_t                      code = TSDB_CODE_SUCCESS;
  int32_t                      lino = 0;
  SStreamIntervalOperatorInfo* pInfo = (SStreamIntervalOperatorInfo*)pOperator->info;
  pInfo->dataVersion = TMAX(pInfo->dataVersion, pSDataBlock->info.version);

  SResultRowInfo* pResultRowInfo = &(pInfo->binfo.resultRowInfo);
  SExecTaskInfo*  pTaskInfo = pOperator->pTaskInfo;
  SExprSupp*      pSup = &pOperator->exprSupp;
  int32_t         numOfOutput = pSup->numOfExprs;
  TSKEY*          tsCols = NULL;
  SRowBuffPos*    pResPos = NULL;
  SResultRow*     pResult = NULL;
  int32_t         forwardRows = 0;

  SColumnInfoData* pColDataInfo = taosArrayGet(pSDataBlock->pDataBlock, pInfo->primaryTsIndex);
  tsCols = (int64_t*)pColDataInfo->pData;

  void*            pPkVal = NULL;
  int32_t          pkLen = 0;
  SColumnInfoData* pPkColDataInfo = NULL;
  if (hasSrcPrimaryKeyCol(&pInfo->basic)) {
    pPkColDataInfo = taosArrayGet(pSDataBlock->pDataBlock, pInfo->basic.primaryPkIndex);
  }

  int32_t     startPos = 0;
  TSKEY       ts = getStartTsKey(&pSDataBlock->info.window, tsCols);
  STimeWindow nextWin = {0};
  nextWin = getActiveTimeWindow(pInfo->aggSup.pResultBuf, pResultRowInfo, ts, &pInfo->interval, TSDB_ORDER_ASC);

  while (1) {
    int32_t winCode = TSDB_CODE_SUCCESS;
    code = setIntervalOutputBuf(pInfo->pState, &nextWin, &pResPos, groupId, pSup->pCtx, numOfOutput,
                                pSup->rowEntryInfoOffset, &pInfo->aggSup, &pInfo->stateStore, &winCode);
    QUERY_CHECK_CODE(code, lino, _end);

    pResult = (SResultRow*)pResPos->pRowBuff;
    forwardRows = getNumOfRowsInTimeWindow(&pSDataBlock->info, tsCols, startPos, nextWin.ekey, binarySearchForKey, NULL,
                                           TSDB_ORDER_ASC);

    SWinKey key = {
        .ts = pResult->win.skey,
        .groupId = groupId,
    };

    // todo(liuyao) 在这里需要修改，每个分区只保留一个窗口。
    // if (pInfo->twAggSup.calTrigger == STREAM_TRIGGER_AT_ONCE && pUpdatedMap) {
    //   code = saveWinResult(&key, pResPos, pUpdatedMap);
    //   QUERY_CHECK_CODE(code, lino, _end);
    // }

    updateTimeWindowInfo(&pInfo->twAggSup.timeWindowData, &nextWin, 1);
    code = applyAggFunctionOnPartialTuples(pTaskInfo, pSup->pCtx, &pInfo->twAggSup.timeWindowData, startPos,
                                           forwardRows, pSDataBlock->info.rows, numOfOutput);
    QUERY_CHECK_CODE(code, lino, _end);
    key.ts = nextWin.skey;

    if (pInfo->delKey.ts > key.ts) {
      pInfo->delKey = key;
    }
    int32_t prevEndPos = forwardRows - 1 + startPos;
    startPos =
        getNextQualifiedWindow(&pInfo->interval, &nextWin, &pSDataBlock->info, tsCols, prevEndPos, TSDB_ORDER_ASC);
    if (startPos < 0) {
      break;
    }
  }

_end:
  if (code != TSDB_CODE_SUCCESS) {
    qError("%s failed at line %d since %s. task:%s", __func__, lino, tstrerror(code), GET_TASKID(pTaskInfo));
  }
  return code;
}

static int32_t doStreamIntervalNonblockAggNext(SOperatorInfo* pOperator, SSDataBlock** ppRes) {
  int32_t                      code = TSDB_CODE_SUCCESS;
  int32_t                      lino = 0;
  SStreamIntervalOperatorInfo* pInfo = pOperator->info;
  SExecTaskInfo*               pTaskInfo = pOperator->pTaskInfo;
  SStorageAPI*                 pAPI = &pOperator->pTaskInfo->storageAPI;
  SExprSupp*                   pSup = &pOperator->exprSupp;

  qDebug("stask:%s  %s status: %d", GET_TASKID(pTaskInfo), getStreamOpName(pOperator->operatorType), pOperator->status);

  if (pOperator->status == OP_EXEC_DONE) {
    (*ppRes) = NULL;
    return code;
  }

  // todo(liuyao) 这里需要修改，如果结果，就返回，这里需要清理缓存。
  if (pOperator->status == OP_RES_TO_RETURN) {
    SSDataBlock* resBlock = NULL;
    code = buildIntervalResult(pOperator, &resBlock);
    QUERY_CHECK_CODE(code, lino, _end);
    if (resBlock != NULL) {
      (*ppRes) = resBlock;
      return code;
    }

    if (pInfo->reCkBlock) {
      pInfo->reCkBlock = false;
      printDataBlock(pInfo->pCheckpointRes, getStreamOpName(pOperator->operatorType), GET_TASKID(pTaskInfo));
      (*ppRes) = pInfo->pCheckpointRes;
      return code;
    }

    setStreamOperatorCompleted(pOperator);
    (*ppRes) = NULL;
    return code;
  }

  SOperatorInfo* downstream = pOperator->pDownstream[0];

  while (1) {
    SSDataBlock* pBlock = NULL;
    code = downstream->fpSet.getNextFn(downstream, &pBlock);
    QUERY_CHECK_CODE(code, lino, _end);

    if (pBlock == NULL) {
      qDebug("===stream===return data:%s. recv datablock num:%" PRIu64, getStreamOpName(pOperator->operatorType),
             pInfo->numOfDatapack);
      pInfo->numOfDatapack = 0;
      pOperator->status = OP_RES_TO_RETURN;
      break;
    }

    pInfo->numOfDatapack++;
    printSpecDataBlock(pBlock, getStreamOpName(pOperator->operatorType), "recv", GET_TASKID(pTaskInfo));
    setStreamOperatorState(&pInfo->basic, pBlock->info.type);

    if (pBlock->info.type == STREAM_DELETE_DATA || pBlock->info.type == STREAM_DELETE_RESULT ||
        pBlock->info.type == STREAM_CLEAR) {
      code = doDeleteWindows(pOperator, &pInfo->interval, pBlock, pInfo->pDelWins, pInfo->pUpdatedMap, NULL);
      QUERY_CHECK_CODE(code, lino, _end);
      continue;
    } else if (pBlock->info.type == STREAM_CREATE_CHILD_TABLE || pBlock->info.type == STREAM_DROP_CHILD_TABLE) {
      printDataBlock(pBlock, getStreamOpName(pOperator->operatorType), GET_TASKID(pTaskInfo));
      (*ppRes) = pBlock;
      return code;
    } else if (pBlock->info.type == STREAM_CHECKPOINT) {
      pAPI->stateStore.streamStateCommit(pInfo->pState);
      doStreamIntervalSaveCheckpoint(pOperator);
      pInfo->reCkBlock = true;
      code = copyDataBlock(pInfo->pCheckpointRes, pBlock);
      QUERY_CHECK_CODE(code, lino, _end);

      continue;
    } else {
      if (pBlock->info.type != STREAM_NORMAL && pBlock->info.type != STREAM_INVALID) {
        code = TSDB_CODE_QRY_EXECUTOR_INTERNAL_ERROR;
        QUERY_CHECK_CODE(code, lino, _end);
      }
    }

    if (pBlock->info.type == STREAM_NORMAL && pBlock->info.version != 0) {
      // set input version
      pTaskInfo->version = pBlock->info.version;
    }

    if (pInfo->scalarSupp.pExprInfo != NULL) {
      SExprSupp* pExprSup = &pInfo->scalarSupp;
      code = projectApplyFunctions(pExprSup->pExprInfo, pBlock, pBlock, pExprSup->pCtx, pExprSup->numOfExprs, NULL);
      QUERY_CHECK_CODE(code, lino, _end);
    }

    code = setInputDataBlock(pSup, pBlock, TSDB_ORDER_ASC, MAIN_SCAN, true);
    QUERY_CHECK_CODE(code, lino, _end);

    code = doStreamParallelIntervalAggImpl(pOperator, pBlock, pBlock->info.id.groupId, pInfo->pUpdatedMap, pInfo->pDeletedMap);
    if (code == TSDB_CODE_STREAM_INTERNAL_ERROR) {
      pOperator->status = OP_RES_TO_RETURN;
      code = TSDB_CODE_SUCCESS;
      break;
    }
    QUERY_CHECK_CODE(code, lino, _end);

    if (tSimpleHashGetSize(pInfo->pUpdatedMap) > 0) {
      break;
    }
  }

  if (!pInfo->destHasPrimaryKey) {
    removeDeleteResults(pInfo->pUpdatedMap, pInfo->pDelWins);
  } else {
    code = copyIntervalDeleteKey(pInfo->pDeletedMap, pInfo->pDelWins);
    QUERY_CHECK_CODE(code, lino, _end);
  }

  copyUpdateResult(&pInfo->pUpdatedMap, pInfo->pUpdated, winPosCmprImpl);

  initMultiResInfoFromArrayList(&pInfo->groupResInfo, pInfo->pUpdated);
  pInfo->pUpdated = NULL;
  code = blockDataEnsureCapacity(pInfo->binfo.pRes, pOperator->resultInfo.capacity);
  QUERY_CHECK_CODE(code, lino, _end);

  _hash_fn_t hashFn = taosGetDefaultHashFunction(TSDB_DATA_TYPE_BINARY);
  pInfo->pUpdatedMap = tSimpleHashInit(4096, hashFn);
  QUERY_CHECK_NULL(pInfo->pUpdatedMap, code, lino, _end, terrno);

  code = buildIntervalResult(pOperator, ppRes);
  QUERY_CHECK_CODE(code, lino, _end);

  return code;

_end:
  if (code != TSDB_CODE_SUCCESS) {
    qError("%s failed at line %d since %s. task:%s", __func__, lino, tstrerror(code), GET_TASKID(pTaskInfo));
    pTaskInfo->code = code;
  }
  setStreamOperatorCompleted(pOperator);
  (*ppRes) = NULL;
  return code;
}
