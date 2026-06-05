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
#include "cmd_priv.h"



void cmd_init(cmdMsg_t *cmdMsg) {
    memset(cmdMsg, 0, sizeof(cmdMsg_t));
    cmdMsg->magic   = CMD_MAGIC_NUMBER;
    cmdMsg->version = CMD_VERSION;
    cmdMsg->prior   = CMD_DEFAULT_PRIORITY;
    cmdMsg->crc32   = 0;
}



int32_t cmd_recv(cmdMsg_t *cmdMsg) {
   // mailbox_recv(cmdMsg);

    cmd_proc(cmdMsg);
    return 0;
}

int32_t cmd_send(cmdMsg_t *cmdMsg) {
   cmdMsg->crc32 = crc32_calc((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
   // mailbox_send(cmdMsg);
   return 0;
}