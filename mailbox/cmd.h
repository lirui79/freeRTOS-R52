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
**                                 include cmd header                           **
*********************************************************************************/

#ifndef _FREERTOS_COMMAND_H_
#define _FREERTOS_COMMAND_H_

#include "cmdef.h"

#ifdef __cplusplus
extern "C" {
#endif

void cmd_init(cmdMsg_t *cmdMsg);


int32_t cmd_recv(cmdMsg_t *cmdMsg);

int32_t cmd_send(cmdMsg_t *cmdMsg);


#ifdef __cplusplus
}
#endif

#endif /*_FREERTOS_COMMAND_H_*/
