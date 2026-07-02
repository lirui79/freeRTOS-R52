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
**                      include command r52 manager header                      **
*********************************************************************************/

#ifndef _CMD_MANAGER_H_
#define _CMD_MANAGER_H_


#include "cmdef.h"
#include "bqueue.h"
#include "cmdr52_session.h"
#include "vcx_vcmd_priv.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    uint32_t           r52coreID;// r52 core ID  from 0
    uint32_t           vtb_size; //
    cmdr52_session_t   vtb[CMD_SESSION_MAX];// vcodec session table
    vcmd_mgr_t*        mtb[VCMD_MGR_ID_MAX];	// vcmd manager  0-vcmd mgr enc, 1- vcmd mgr dec
    BQueueHandle_t     cmd_queue; // command queue
} cmdr52_mgr_t;


vcmd_mgr_t*          cmdr52_get_vcmd_mgr(uint32_t mgrID);

int                  cmdr52_mgr_init(cmdr52_mgr_t *mgr);

cmdr52_mgr_t*        cmdr52_mgr_get(void);

cmdr52_session_t*    cmdr52_mgr_get_session(uint32_t sessionID);

cmdr52_session_t*    cmdr52_mgr_get_idle_session();

cmdMsg_t*            cmdr52_mgr_dequeue_cmdMsg(void);

cmdMsg_t*            cmdr52_mgr_acquire_cmdMsg(void);

int32_t              cmdr52_mgr_release_cmdMsg(cmdMsg_t* cmdMsg);

int32_t              cmdr52_mgr_queue_cmdMsg(cmdMsg_t* cmdMsg);

int32_t              cmdr52_mgr_cancel_cmdMsg(cmdMsg_t* cmdMsg);

int32_t              cmdr52_mgr_proc_cmdMsg(cmdMsg_t *cmdMsg);

int32_t              cmdr52_mgr_wait_cmdMsg(cmdMsg_t *cmdMsg, uint32_t mgrid);


#ifdef __cplusplus
}
#endif

#endif /*_CMD_MANAGER_H_*/
