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

#define _DEFAULT_SOURCE
#include "mndAuth.h"

static int32_t mndProcessAuthReq(SMnodeMsg *pReq);

int32_t mndInitAuth(SMnode *pMnode) {
  mndSetMsgHandle(pMnode, TDMT_MND_AUTH, mndProcessAuthReq);
  return 0;
}

void mndCleanupAuth(SMnode *pMnode) {}

int32_t mndRetriveAuth(SMnode *pMnode, char *user, char *spi, char *encrypt, char *secret, char *ckey) { return 0; }

static int32_t mndProcessAuthReq(SMnodeMsg *pReq) {
  SAuthReq *pAuth = pReq->rpcMsg.pCont;

  int32_t   contLen = sizeof(SAuthRsp);
  SAuthRsp *pRsp = rpcMallocCont(contLen);
  pReq->pCont = pRsp;
  pReq->contLen = contLen;

  int32_t code = mndRetriveAuth(pReq->pMnode, pAuth->user, &pRsp->spi, &pRsp->encrypt, pRsp->secret, pRsp->ckey);
  mTrace("user:%s, auth req received, spi:%d encrypt:%d ruser:%s", pReq->user, pAuth->spi, pAuth->encrypt, pAuth->user);
  return code;
}