/********************************************************************************* 
**       This software is confidential and proprietary and may be used          **
**        only as expressly authorized by a licensing agreement from            **
**                                                                              **
**                            omnidimension                                     **
**                                                                              **
**                   (C) COPYRIGHT 2026 OMNIDIMENSION                           **
**                            ALL RIGHTS RESERVED                               **
**                                                                              **
**                 The entire notice above must be reproduced                   **
**                  on all copies and should not be removed.                    **
**                                                                              **
**********************************************************************************
**                      include command manager source                          **
*********************************************************************************/

#include "cmd.h"
#include "crc32.h"
#include "cmd_mgr.h"
#include "cmd_session.h"
#include "vcx_vcmd_priv.h"



static cmd_mgr_t g_cmd_mgr;



int             cmd_init_mgr(cmd_mgr_t *mgr) {
    int32_t i = 0;
    uint32_t sessionID = 0; // session ID from 0

    mgr->r52coreID = 0;// r52 core ID  from 0
    mgr->vtb_size  = CMD_SESSION_MAX; // r52 core number
    sessionID = ((mgr->r52coreID << 16) & 0xFFFF0000);
    for (i = 0; i < mgr->vtb_size; ++i) {
        cmd_session_init(&mgr->vtb[i], &mgr->ptb[i], sessionID + i);
    }

    for (i = 0; i < VCMD_MGR_ID_MAX; ++i) {
        mgr->mtb[i] = NULL;
    }
    mgr->cmd_queue = BQueueCreate(1024, CMD_MSG_MAX_SIZE);
    return 0;
}

vcmd_mgr_t*    cmd_get_vcmd_mgr(uint32_t mgrID) {
    if (mgrID >= VCMD_MGR_ID_MAX) {
        return NULL;
    }
    return cmd_get_mgr()->mtb[mgrID];
}

cmd_session_t*    cmd_get_session(uint32_t sessionID) {
    if (sessionID >= CMD_SESSION_MAX) {
        return NULL;
    }
    return &cmd_get_mgr()->vtb[sessionID];
}

cmd_session_t*    cmd_get_idle_session() {
    cmd_mgr_t* mgr = cmd_get_mgr();
    for (uint32_t i = 0; i < mgr->vtb_size; ++i) {
        if (mgr->vtb[i].status == CMD_SESSION_STATUS_IDLE) {
            return &mgr->vtb[i];
        }
    }

    return NULL;
}

struct proc_obj * cmd_get_proc(uint32_t sessionID) {
    if (sessionID >= CMD_SESSION_MAX) {
        return NULL;
    }
    return &cmd_get_mgr()->ptb[sessionID];   
}

struct proc_obj * cmd_get_proc(uint32_t sessionID);

cmd_mgr_t *cmd_get_mgr(void) {
    return &g_cmd_mgr;
}

cmdMsg_t*         cmd_dequeue_cmdMsg(void) {
    return (cmdMsg_t *)BQueueDequeue(cmd_get_mgr()->cmd_queue);
}

cmdMsg_t*         cmd_acquire_cmdMsg(void) {
    return (cmdMsg_t *)BQueueAcquire(cmd_get_mgr()->cmd_queue);
}

int32_t           cmd_release_cmdMsg(cmdMsg_t* cmdMsg) {
    return   BQueueRelease(cmd_get_mgr()->cmd_queue, cmdMsg);
}

int32_t           cmd_queue_cmdMsg(cmdMsg_t* cmdMsg) {
    return   BQueueQueue(cmd_get_mgr()->cmd_queue, cmdMsg);
}

int32_t           cmd_cancel_cmdMsg(cmdMsg_t* cmdMsg) {
    return   BQueueCancel(cmd_get_mgr()->cmd_queue, cmdMsg);
}

static uint32_t cmd_check(cmdMsg_t *cmdMsg, cmd_session_t **session)
{
    uint32_t crc32 = 0, crc32Now = 0, r52ID = 0, sesID = 0, retCode = CMD_ERR_SUCCESS;
    if (cmdMsg == NULL) {
        retCode = CMD_ERR_INVALID_POINTER;
        goto RETURN_ERROR;
    }

    if (cmdMsg->magic != CMD_MAGIC_NUMBER) {
        retCode = CMD_ERR_INVALID_MAGIC;
        goto RETURN_ERROR;
    }

    if (cmdMsg->version != CMD_VERSION) {
        retCode = CMD_ERR_INVALID_VERSION;
        goto RETURN_ERROR;
    }

    if (cmdMsg->cmdType > CMD_VCODEC_MAX) {
        retCode = CMD_ERR_INVALID_CMD_TYPE;
        goto RETURN_ERROR;
    }

    crc32 = cmdMsg->crc32;
    cmdMsg->crc32 = 0;
    crc32Now = crc32_calc((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
    cmdMsg->crc32 = crc32;
    if (crc32 != crc32Now) {
        retCode = CMD_ERR_INVALID_CHECKSUM;
        goto RETURN_ERROR;
    }

    r52ID  = ((cmdMsg->sessionID & 0xFFFF0000) >> 16);
    sesID  = (cmdMsg->sessionID & 0xFFFF);
    if (r52ID != cmd_get_mgr()->r52coreID) {
        retCode = CMD_ERR_INVALID_SESSIONID;
        goto RETURN_ERROR;
    }

    *session = cmd_get_session(sesID);
    if (*session == NULL) {
        retCode = CMD_ERR_INVALID_SESSIONID;
        goto RETURN_ERROR;
    }

    return CMD_ERR_SUCCESS;
RETURN_ERROR:
    {
        cmdMsg_t *cmdMsg1 = cmd_dequeue_cmdMsg();
        cmdEvtRepCmdError_Body_t *cmdBody1 = (cmdEvtRepCmdError_Body_t *)cmdMsg1->data;
        cmd_session_t *session1 = cmd_get_session(0);
        cmd_init(cmdMsg1);
        cmdMsg1->cmdType    = CMD_EVT_REPORT_CMDERROR;
        cmdMsg1->cmdSize    = CMD_MSG_MIN_SIZE + sizeof(cmdEvtRepCmdError_Body_t);
        cmdBody1->code      = retCode;
        cmdBody1->cmdType   = cmdMsg->cmdType;
        cmdBody1->seqNum    = cmdMsg->seqNum;
        cmdBody1->sessionID = cmdMsg->sessionID;
        cmdBody1->procObj   = 0x0000000000000000;//cmdMsg->procObj;
        cmdBody1->timeStamp = cmdMsg->timeStamp;
        cmd_session_send(session1, cmdMsg1);
    }

    return retCode;
}


int32_t cmd_proc_cmdMsg(cmdMsg_t *cmdMsg) {
    cmd_session_t *session = NULL;

    if (cmd_check(cmdMsg, &session) != CMD_ERR_SUCCESS) {
        return CMD_ERR_INVALID_PARAM;
    }

    if (cmd_session_check(session, cmdMsg) < 0) {
        return CMD_ERR_INVALID_SEQUENCEID;
    }

    if (cmdMsg->cmdType <= CMD_SYSTEM_MAX) {
        return cmd_session_system(session, cmdMsg);
    }

    return cmd_session_vcodec(session, cmdMsg);
}
