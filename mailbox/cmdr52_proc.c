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
**                        *.c cmdr52 proc source code                           **
*********************************************************************************/

#include "libc.h"
#include "crc32.h"
#include "vcodec.h"
#include "cmdr52_mgr.h"
#include "cmdr52_proc.h"




int32_t cmdr52_recv(void) {
    int32_t retCode = 0;
    size_t  cmdSize = CMD_MSG_MAX_SIZE;
    cmdMsg_t *cmdMsg = cmdr52_mgr_dequeue_cmdMsg();
    if (cmdMsg == NULL) {
        return -1;
    }
// mailbox_recv(cmdMsg);
    retCode = mhu_receive_data((uint8_t *)cmdMsg, &cmdSize);
    cmdr52_mgr_queue_cmdMsg(cmdMsg);
    return retCode;
}

int32_t cmdr52_send(cmdMsg_t *cmdMsg) {
    int32_t retCode = 0;
    cmdMsg->crc32 = crc32_calc((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
// mailbox_send(cmdMsg);
    retCode = mhu_send_data((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
    cmdr52_mgr_release_cmdMsg(cmdMsg);
    return retCode;
}

int32_t cmdr52_proc(void) {
    cmdMsg_t *cmdMsg = cmdr52_mgr_dequeue_cmdMsg();
    if (cmdMsg == NULL) {
        return -1;
    }
    cmdr52_mgr_proc_cmdMsg(cmdMsg);
    cmdr52_mgr_release_cmdMsg(cmdMsg);
    return 0;

}

int32_t cmdr52_wait(uint32_t mgrid) {
    cmdMsg_t *cmdMsg = cmdr52_mgr_dequeue_cmdMsg();
    if (cmdMsg == NULL) {
        return -1;
    }

    cmdr52_mgr_wait_cmdMsg(cmdMsg, mgrid);
    cmdr52_mgr_release_cmdMsg(cmdMsg);
    return 0;
}