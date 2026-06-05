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
**                      include command session source                          **
*********************************************************************************/

#include "cmd_session.h"
#include "vcx_vcmd_priv.h"
#include "vcx_cmdbuf_obj.h"


int32_t cmd_session_init(cmd_session_t *session, struct proc_obj *proc) {
    session->proc = init_process_object(proc);
    proc->session = session;
    session->sessionID = 0xFFFFFFFF;
    session->seqRNum   = 0x00000000;
    session->seqSNum   = 0x00000000;
    session->status    = CMD_SESSION_STATUS_IDLE;
    session->procObj   = 0x00000000;
    return 0;
}

int32_t        cmd_session_check(cmd_session_t *session, cmdMsg_t *cmdMsg) {
    if (session->seqRNum != cmdMsg->seqNum) {
        return -1;
    }
    session->seqRNum++;
    return 0;
}

static int32_t cmd_system_open_session(cmd_session_t *session, cmdMsg_t *cmdMsg) {
    return 0;
}

static int32_t cmd_system_close_session(cmd_session_t *session, cmdMsg_t *cmdMsg) {
    return 0;
}

int32_t        cmd_session_system(cmd_session_t *session, cmdMsg_t *cmdMsg) {
    switch (cmdMsg->cmdType) {
    case CMD_REQ_EXE_SYSCTL:
        //return cmd_system_echo(cmdMsg);
        break;
    case CMD_REQ_OPEN_SESSION:
        return cmd_system_open_session(session, cmdMsg);
        break;
    case CMD_REQ_CLOSE_SESSION:
        return cmd_system_close_session(session, cmdMsg);
        break;
    default:
        break;
    }
    return 0;
}

int32_t        cmd_session_vcodec(cmd_session_t *session, cmdMsg_t *cmdMsg) {
    switch (cmdMsg->cmdType) {
    case CMD_REQ_RUN_CMDBUF:
        return vcodec_run_cmdbuf(cmdMsg);
        break;
    default:
        break;
    }
    return 0;
}
