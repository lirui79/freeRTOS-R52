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
**                             *.c cmd source code                              **
*********************************************************************************/

#include "cmd.h"
#include "libc.h"
#include "crc32.h"
#include "vcodec.h"
#include "cmd_mgr.h"
#include <mhu_v3_x_private.h>
#include <mhu_v3_x.h>
#include <mhu.h>


void cmd_init(cmdMsg_t *cmdMsg) {
    memset(cmdMsg, 0, sizeof(cmdMsg_t));
    cmdMsg->magic   = CMD_MAGIC_NUMBER;
    cmdMsg->version = CMD_VERSION;
    cmdMsg->prior   = CMD_DEFAULT_PRIORITY;
    cmdMsg->crc32   = 0;
}



int32_t cmd_recv(void) {
    int32_t retCode = 0;
    size_t  cmdSize = CMD_MSG_MAX_SIZE;
    cmdMsg_t *cmdMsg = cmd_dequeue_cmdMsg();
    if (cmdMsg == NULL) {
        return -1;
    }
// mailbox_recv(cmdMsg);
    retCode = mhu_receive_data((uint8_t *)cmdMsg, &cmdSize);
    cmd_queue_cmdMsg(cmdMsg);
    return retCode;
}

int32_t cmd_send(cmdMsg_t *cmdMsg) {
    int32_t retCode = 0;
    cmdMsg->crc32 = crc32_calc((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
// mailbox_send(cmdMsg);
    retCode = mhu_send_data((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
    cmd_release_cmdMsg(cmdMsg);
    return retCode;
}

int32_t cmd_proc(void) {
    cmdMsg_t *cmdMsg = cmd_acquire_cmdMsg();
    if (cmdMsg == NULL) {
        return -1;
    }
    cmd_proc_cmdMsg(cmdMsg);
    cmd_release_cmdMsg(cmdMsg);
    return 0;

}