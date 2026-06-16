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
**                      include command manager header                          **
*********************************************************************************/

#ifndef _CMD_MANAGER_H_
#define _CMD_MANAGER_H_

#include <stdint.h>
#include <stddef.h>
#include "cmdef.h"
#include "vcx_vcmd_priv.h"
#include "cmd_session.h"
#include "bqueue.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    uint32_t           r52coreID;// r52 core ID  from 0
    uint32_t           vtb_size; //
    cmd_session_t      vtb[CMD_SESSION_MAX];// vcodec session table
    struct proc_obj    ptb[CMD_SESSION_MAX];// process table
    vcmd_mgr_t*        mtb[VCMD_MGR_ID_MAX];	// vcmd manager  0-vcmd mgr enc, 1- vcmd mgr dec
    BQueueHandle_t     cmd_queue; // command queue
} cmd_mgr_t;


int               cmd_init_mgr(cmd_mgr_t *mgr);

cmd_mgr_t*       cmd_get_mgr(void);

vcmd_mgr_t*       cmd_get_vcmd_mgr(uint32_t mgrID);

cmd_session_t*    cmd_get_session(uint32_t sessionID);

cmd_session_t*    cmd_get_idle_session();

struct proc_obj*  cmd_get_proc(uint32_t sessionID);

cmdMsg_t*         cmd_dequeue_cmdMsg(void);

cmdMsg_t*         cmd_acquire_cmdMsg(void);

int32_t           cmd_release_cmdMsg(cmdMsg_t* cmdMsg);

int32_t           cmd_queue_cmdMsg(cmdMsg_t* cmdMsg);

int32_t           cmd_cancel_cmdMsg(cmdMsg_t* cmdMsg);

int32_t           cmd_proc_cmdMsg(cmdMsg_t *cmdMsg);





#ifdef __cplusplus
}
#endif

#endif /*_CMD_MANAGER_H_*/
