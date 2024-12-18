/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 * * This program is free software: you can use, redistribute, and/or modify
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

#include "transComm.h"
#include "tversion.h"

#ifdef TD_ACORE
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#endif

#define DIV_ROUNDUP(x, y) (((x) + ((y)-1)) / (y))

#define EVT_TIMEOUT 0x01
#define EVT_READ    0x02
#define EVT_WRITE   0x04
#define EVT_SIGNAL  0x08
#define EVT_ASYNC   0x10

/* How many bytes to allocate for N fds? */
#define SELECT_ALLOC_SIZE(n) (DIV_ROUNDUP(n, NFDBITS) * sizeof(fd_mask))

typedef void (*AsyncCb)(void *async, int32_t status);

typedef struct {
  void         *data;
  int32_t       sendFd;
  AsyncCb       cb;
  queue         q;
  TdThreadMutex mutex;
} SAsyncHandle;
typedef struct {
  queue   q;
  int32_t acceptFd;
} SFdArg;
typedef struct {
  queue q;
} SFdQueue;
typedef struct SWorkThrd {
  TdThread thread;
  // //uv_connect_t connect_req;
  // uv_pipe_t*   pipe;
  // uv_os_fd_t   fd;
  // uv_loop_t*   loop;
  // SAsyncPool*  asyncPool;
  queue msg;

  queue conn;
  void *pInst;
  bool  quit;

  // SIpWhiteListTab* pWhiteList;
  int64_t whiteListVer;
  int8_t  enableIpWhiteList;

  int32_t connRefMgt;

  int32_t       pipe_fd[2];  //
  int32_t       pipe_queue_fd[2];
  int32_t       client_count;
  int8_t        inited;
  TdThreadMutex mutex;
  SFdQueue      fdQueue;

  SAsyncHandle *notifyNewConnHandle;
  SAsyncHandle *handle;
} SWorkThrd2;
typedef struct SServerObj {
  TdThread     thread;
  int          workerIdx;
  int          numOfThreads;
  int          numOfWorkerReady;
  SWorkThrd2 **pThreadObj;

  // uv_pipe_t   pipeListen;
  // uv_pipe_t** pipe;
  uint32_t ip;
  uint32_t port;
  // uv_async_t* pAcceptAsync;  // just to quit from from accept thread
  int32_t serverFd;
  bool    inited;
} SServerObj2;

typedef void (*__sendCb)(SFdArg *arg, int32_t status);
typedef void (*__readCb)(SFdArg *arg, int32_t status);
typedef void (*__asyncCb)(SFdArg *arg, int32_t status);

enum EVT_TYPE { EVT_ASYNC_T = 0, EVT_CONN_T = 1, EVT_SIGANL_T };

typedef struct {
  int32_t fd;
  void   *data;
  int32_t event;

  __sendCb  sentFn;
  __readCb  readFn;
  __asyncCb asyncFn;

  int8_t evtType;
  void  *arg;
} SFdCbArg;
typedef struct {
  int32_t   evtFds;
  int32_t   evtFdsSize;
  int32_t   resizeOutSets;
  fd_set   *evtReadSetIn;
  fd_set   *evtWriteSetIn;
  fd_set   *evtReadSetOut;
  fd_set   *evtWriteSetOut;
  SHashObj *pFdTable;
} SEvtMgt;

static int32_t evtMgtResize(SEvtMgt *pOpt, int32_t cap);
static int32_t evtMgtCreate(SEvtMgt **pOpt);
static int32_t evtMgtDispath(SEvtMgt *pOpt, struct timeval *tv);
static int32_t evtMgtAdd(SEvtMgt *pOpt, int32_t fd, int32_t events, SFdCbArg *arg);
static int32_t evtMgtRemove(SEvtMgt *pOpt, int32_t fd, int32_t events, SFdCbArg *arg);
static void    evtMgtDestroy(SEvtMgt *pOpt);

int32_t evtMgtHandle(SEvtMgt *pOpt, int32_t res, int32_t fd);

static int32_t evtMgtCreate(SEvtMgt **pOpt) {
  int32_t  code = 0;
  SEvtMgt *pRes = taosMemoryCalloc(1, sizeof(SEvtMgt));
  if (pRes == NULL) {
    *pOpt = NULL;
    return terrno;
  }
  pRes->evtFds = 0;
  pRes->evtFdsSize = 0;
  pRes->resizeOutSets = 0;
  pRes->evtReadSetIn = NULL;
  pRes->evtWriteSetIn = NULL;
  pRes->evtReadSetOut = NULL;
  pRes->evtWriteSetOut = NULL;

  code = evtMgtResize(pRes, (32 + 1));
  if (code != 0) {
    evtMgtDestroy(pRes);
  } else {
    *pOpt = pRes;
  }

  return code;
}

// int32_t selectUtilRange()

int32_t evtMgtHandleImpl(SEvtMgt *pOpt, SFdCbArg *pArg) {
  int32_t code = 0;
  if (pArg->evtType == EVT_CONN_T) {
    SAsyncHandle *handle = pArg->arg;
    handle->cb(handle, 0);
  } else if (pArg->evtType == EVT_ASYNC_T) {
    SAsyncHandle *handle = pArg->arg;
    handle->cb(handle, 0);
  } else {
  }
  return code;
}
int32_t evtMgtHandle(SEvtMgt *pOpt, int32_t res, int32_t fd) {
  int32_t code = 0;
  if (res & EVT_READ) {
    // handle read
  }
  if (res & EVT_WRITE) {
    // handle write
  }
  SFdCbArg *pArg = taosHashGet(pOpt->pFdTable, &fd, sizeof(fd));
  if (pArg == NULL) {
    return TAOS_SYSTEM_ERROR(EBADF);
  }
  code = evtMgtHandleImpl(pOpt, pArg);
  return code;
}
static int32_t evtMgtDispath(SEvtMgt *pOpt, struct timeval *tv) {
  int32_t code = 0, res = 0, i, j, nfds = 0;
  if (pOpt->resizeOutSets) {
    fd_set *readSetOut = NULL, *writeSetOut = NULL;
    int32_t sz = pOpt->evtFdsSize;
    readSetOut = taosMemoryRealloc(pOpt->evtReadSetOut, sz);
    if (readSetOut == NULL) {
      return terrno;
    }
    pOpt->evtReadSetOut = readSetOut;

    writeSetOut = taosMemoryRealloc(pOpt->evtWriteSetOut, sz);
    if (writeSetOut == NULL) {
      return terrno;
    }

    pOpt->evtWriteSetOut = writeSetOut;
    pOpt->resizeOutSets = 0;
  }

  memcpy(pOpt->evtReadSetOut, pOpt->evtReadSetIn, pOpt->evtFdsSize);
  memcpy(pOpt->evtWriteSetOut, pOpt->evtWriteSetIn, pOpt->evtFdsSize);

  nfds = pOpt->evtFds + 1;
  // TODO lock or not
  code = select(nfds, pOpt->evtReadSetOut, pOpt->evtWriteSetOut, NULL, tv);
  if (code < 0) {
    return TAOS_SYSTEM_ERROR(errno);
  }

  for (j = 0; j < nfds; j++) {
    res = 0;
    if (FD_ISSET(j, pOpt->evtReadSetOut)) {
      res |= EVT_READ;
    }
    if (FD_ISSET(j, pOpt->evtWriteSetOut)) {
      res |= EVT_WRITE;
    }
    code = evtMgtHandle(pOpt, res, i);
    if (code != 0) {
      tError("failed to handle fd %d since %s", i, tstrerror(code));
    }
  }

  return code;
}

static int32_t evtMgtResize(SEvtMgt *pOpt, int32_t cap) {
  int32_t code = 0;

  fd_set *readSetIn = NULL;
  fd_set *writeSetIn = NULL;

  readSetIn = taosMemoryRealloc(pOpt->evtReadSetIn, cap);
  if (readSetIn == NULL) {
    return terrno;
  }
  pOpt->evtReadSetIn = readSetIn;

  writeSetIn = taosMemoryRealloc(pOpt->evtWriteSetIn, cap);
  if (writeSetIn == NULL) {
    return terrno;
  }

  pOpt->evtWriteSetIn = writeSetIn;
  pOpt->resizeOutSets = 1;

  memset((char *)pOpt->evtReadSetIn + pOpt->evtFdsSize, 0, cap - pOpt->evtFdsSize);
  memset((char *)pOpt->evtWriteSetIn + pOpt->evtFdsSize, 0, cap - pOpt->evtFdsSize);

  pOpt->evtFdsSize = cap;
  return code;
}

static int32_t evtMgtAdd(SEvtMgt *pOpt, int32_t fd, int32_t events, SFdCbArg *arg) {
  // add new fd to the set
  int32_t code = 0;
  if (pOpt->evtFds < fd) {
    int32_t fdSize = pOpt->evtFdsSize;

    if (fdSize < (int32_t)sizeof(fd_mask)) {
      fdSize = (int32_t)sizeof(fd_mask);
    }
    while (fdSize < (int32_t)SELECT_ALLOC_SIZE(fd + 1)) {
      fdSize *= 2;
    }
    if (fdSize != pOpt->evtFdsSize) {
      if (evtMgtResize(pOpt, fdSize)) {
        return -1;
      }
      pOpt->evtFds = fd;
    }
  }
  if (events & EVT_READ) {
    FD_SET(fd, pOpt->evtReadSetIn);
  }

  if (events & EVT_WRITE) {
    FD_SET(fd, pOpt->evtWriteSetIn);
  }
  code = taosHashPut(pOpt->pFdTable, &fd, sizeof(fd), arg, sizeof(*arg));
  return 0;
}

static int32_t evtMgtRemove(SEvtMgt *pOpt, int32_t fd, int32_t events, SFdCbArg *arg) {
  int32_t code = 0;
  ASSERT((events & EVT_SIGNAL) == 0);

  if (pOpt->evtFds < fd) {
    return TAOS_SYSTEM_ERROR(EBADF);
  }

  if (events & EVT_READ) {
    FD_CLR(fd, pOpt->evtReadSetIn);
  }

  if (events & EVT_WRITE) {
    FD_CLR(fd, pOpt->evtWriteSetIn);
  }
  SFdCbArg *pArg = taosHashGet(pOpt->pFdTable, &fd, sizeof(fd));
  if (pArg == NULL) {
    // TODO, destroy pArg
  } else {
    code = taosHashRemove(pOpt->pFdTable, &fd, sizeof(fd));
  }
  return code;
}
static void evtMgtDestroy(SEvtMgt *pOpt) {
  if (pOpt == NULL) return;

  if (pOpt->evtReadSetIn) {
    taosMemoryFree(pOpt->evtReadSetIn);
  }

  if (pOpt->evtWriteSetIn) {
    taosMemoryFree(pOpt->evtWriteSetIn);
  }

  if (pOpt->evtReadSetOut) {
    taosMemoryFree(pOpt->evtReadSetOut);
  }

  if (pOpt->evtWriteSetOut) {
    taosMemoryFree(pOpt->evtWriteSetOut);
  }

  taosHashCleanup(pOpt->pFdTable);
  taosMemoryFree(pOpt);
}

static int32_t evtAsyncInit(SEvtMgt *pOpt, int32_t fd[2], SAsyncHandle **async, AsyncCb cb, int8_t evtType) {
  int32_t       code = 0;
  SAsyncHandle *pAsync = taosMemoryCalloc(1, sizeof(SAsyncHandle));
  if (pAsync == NULL) {
    return terrno;
  }
  pAsync->data = pOpt;

  taosThreadMutexInit(&pAsync->mutex, NULL);
  pAsync->sendFd = fd[1];
  pAsync->cb = cb;

  SFdCbArg arg = {.evtType = evtType, .arg = pAsync, .fd = fd[0]};
  arg.arg = pAsync;

  code = evtMgtAdd(pOpt, fd[0], EVT_READ, &arg);
  if (code != 0) {
    taosMemoryFree(pAsync);
    return code;
  }
  QUEUE_INIT(&pAsync->q);
  *async = pAsync;
  return code;
}

static int32_t evtAsyncSend(SAsyncHandle *async, queue *q) {
  int32_t code = 0;
  taosThreadMutexLock(&async->mutex);
  QUEUE_PUSH(&async->q, q);
  taosThreadMutexUnlock(&async->mutex);
  int32_t nBytes = write(async->sendFd, "1", 1);
  if (nBytes != 1) {
    return TAOS_SYSTEM_ERROR(errno);
  }

  return code;
}

void *transAcceptThread(void *arg) {
  int32_t code = 0;

  setThreadName("trans-accept-work");
  SServerObj2       *srv = arg;
  struct sockaddr_in client_addr;
  socklen_t          client_addr_len = sizeof(client_addr);
  int32_t            workerIdx = 0;

  while (1) {
    int32_t client_fd = accept(srv->serverFd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
      tError("failed to accept since %s", tstrerror(TAOS_SYSTEM_ERROR(errno)));
      continue;
    }
    workerIdx = (workerIdx + 1) % srv->numOfThreads;
    SWorkThrd2 *pThrd = srv->pThreadObj[workerIdx];

    SFdArg *arg = taosMemoryCalloc(1, sizeof(SFdArg));
    if (arg == NULL) {
      tError("failed to create fd arg since %s", tstrerror(terrno));
      taosThreadMutexUnlock(&pThrd->mutex);
      continue;
    }
    arg->acceptFd = client_fd;
    QUEUE_INIT(&arg->q);

    code = evtAsyncSend(pThrd->notifyNewConnHandle, &arg->q);
    if (code != 0) {
      tError("failed to send async msg since %s", tstrerror(code));
    }
  }
  return NULL;
}

void evtNewConnNotify(void *async, int32_t status) {
  int32_t code = 0;

  SAsyncHandle *handle = async;
  SEvtMgt      *pEvtMgt = handle->data;

  queue wq;
  QUEUE_INIT(&wq);

  taosThreadMutexLock(&handle->mutex);
  QUEUE_MOVE(&handle->q, &wq);
  taosThreadMutexUnlock(&handle->mutex);

  if (QUEUE_IS_EMPTY(&wq)) {
    return;
  }
  while (!QUEUE_IS_EMPTY(&wq)) {
    queue *el = QUEUE_HEAD(&wq);
    QUEUE_REMOVE(el);

    SFdArg *pArg = QUEUE_DATA(el, SFdArg, q);

    SFdCbArg arg = {.evtType = EVT_CONN_T, .arg = pArg, .fd = pArg->acceptFd};
    code = evtMgtAdd(pEvtMgt, pArg->acceptFd, EVT_READ, &arg);
    taosMemoryFree(pArg);
  }

  return;
}
void evtHandleReq(void *async, int32_t status) {
  int32_t code = 0;

  SAsyncHandle *handle = async;
  SEvtMgt      *pEvtMgt = handle->data;

  queue wq;
  QUEUE_INIT(&wq);
  taosThreadMutexLock(&handle->mutex);
  QUEUE_MOVE(&handle->q, &wq);
  taosThreadMutexUnlock(&handle->mutex);

  if (QUEUE_IS_EMPTY(&wq)) {
    return;
  }
  while (!QUEUE_IS_EMPTY(&wq)) {
    queue *el = QUEUE_HEAD(&wq);
    QUEUE_REMOVE(el);

    SFdArg *pArg = QUEUE_DATA(el, SFdArg, q);
    //  code = evtMgtAdd(pEvtMgt, pArg->acceptFd, EVT_READ, NULL);
    taosMemoryFree(pArg);
  }

  return;
}

void *transWorkerThread(void *arg) {
  int32_t code = 0;
  int32_t line = 0;

  struct timeval tv = {5, 0};
  setThreadName("trans-svr-work");
  SWorkThrd2 *pThrd = (SWorkThrd2 *)arg;

  SEvtMgt *pOpt = NULL;

  code = evtMgtCreate(&pOpt);
  if (code != 0) {
    tError("failed to create select op since %s", tstrerror(code));
    TAOS_CHECK_GOTO(code, &line, _end);
  }
  code = evtAsyncInit(pOpt, pThrd->pipe_fd, &pThrd->notifyNewConnHandle, evtNewConnNotify, EVT_CONN_T);
  if (code != 0) {
    tError("failed to create select op since %s", tstrerror(code));
    TAOS_CHECK_GOTO(code, &line, _end);
  }

  code = evtAsyncInit(pOpt, pThrd->pipe_queue_fd, &pThrd->handle, evtHandleReq, EVT_ASYNC_T);
  if (code != 0) {
    tError("failed to create select op since %s", tstrerror(code));
    TAOS_CHECK_GOTO(code, &line, _end);
  }

  while (!pThrd->quit) {
    code = evtMgtDispath(pOpt, &tv);
    if (code != 0) {
      tError("failed to dispatch since %s", tstrerror(code));
      continue;
    }
  }
_end:
  if (code != 0) {
    tError("failed to do work %s", tstrerror(code));
  }
  evtMgtDestroy(pOpt);
  return NULL;
}
static int32_t addHandleToAcceptloop(void *arg) {
  // impl later
  int32_t      code = 0;
  SServerObj2 *srv = arg;
  int32_t      server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    return TAOS_SYSTEM_ERROR(errno);
  }
  srv->serverFd = server_fd;

  int32_t opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  server_addr.sin_port = htons(srv->port);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    return TAOS_SYSTEM_ERROR(errno);
  }
  if (listen(server_fd, 128) < 0) {
    return TAOS_SYSTEM_ERROR(errno);
  }
  code = taosThreadCreate(&srv->thread, NULL, transAcceptThread, srv);
  return code;
}
void *transInitServer(uint32_t ip, uint32_t port, char *label, int numOfThreads, void *fp, void *pInit) {
  int32_t code = 0;

  SServerObj2 *srv = taosMemoryCalloc(1, sizeof(SServerObj2));
  if (srv == NULL) {
    code = terrno;
    tError("failed to init server since: %s", tstrerror(code));
    return NULL;
  }

  srv->ip = ip;
  srv->port = port;
  srv->numOfThreads = numOfThreads;
  srv->workerIdx = 0;
  srv->numOfWorkerReady = 0;
  srv->pThreadObj = (SWorkThrd2 **)taosMemoryCalloc(srv->numOfThreads, sizeof(SWorkThrd2 *));
  if (srv->pThreadObj == NULL) {
    code = terrno;
    return NULL;
  }
  for (int i = 0; i < srv->numOfThreads; i++) {
    SWorkThrd2 *thrd = (SWorkThrd2 *)taosMemoryCalloc(1, sizeof(SWorkThrd2));
    thrd->pInst = pInit;
    thrd->quit = false;
    thrd->pInst = pInit;
    thrd->connRefMgt = transOpenRefMgt(50000, transDestroyExHandle);
    if (thrd->connRefMgt < 0) {
      code = thrd->connRefMgt;
      goto End;
    }

    QUEUE_INIT(&thrd->fdQueue.q);
    if (pipe(thrd->pipe_fd) < 0) {
      code = TAOS_SYSTEM_ERROR(errno);
      goto End;
    }

    int err = taosThreadCreate(&(thrd->thread), NULL, transWorkerThread, (void *)(thrd));
    if (err == 0) {
      tDebug("success to create worker-thread:%d", i);
    } else {
      // TODO: clear all other resource later
      tError("failed to create worker-thread:%d", i);
      goto End;
    }
    thrd->inited = 1;
  }
  code = addHandleToAcceptloop(srv);
  if (code != 0) {
    goto End;
  }
  return NULL;
End:
  return NULL;
}

void transCloseServer(void *arg) {
  // impl later
  return;
}

// int32_t transReleaseSrvHandle(void *handle) { return 0; }
// void    transRefSrvHandle(void *handle) { return; }

// void    transUnrefSrvHandle(void *handle) { return; }
// int32_t transSendResponse(STransMsg *msg) {
//   //
//   int32_t code = 0;
//   if (rpcIsReq(msg->info.msgType) && msg->info.msgType != 0) {
//     msg->msgType = msg->info.msgType + 1;
//   }
//   if (msg->info.noResp) {
//     rpcFreeCont(msg->pCont);
//     return 0;
//   }
//   int32_t svrVer = 0;
//   code = taosVersionStrToInt(td_version, &svrVer);
//   msg->info.cliVer = svrVer;
//   msg->type = msg->info.connType;
//   return transSendResp(msg);
// }
// int32_t transRegisterMsg(const STransMsg *msg) { return 0; }
// int32_t transSetIpWhiteList(void *thandle, void *arg, FilteFunc *func) { return 0; }
