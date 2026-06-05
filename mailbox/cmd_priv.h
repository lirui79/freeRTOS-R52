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
**                      include command private header                          **
*********************************************************************************/

#ifndef _CMD_PRIVATE_H_
#define _CMD_PRIVATE_H_

#include <stdint.h>
#include <stddef.h>
#include "cmdef.h"
#include "vcx_vcmd_priv.h"
#include "cmd_session.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    uint32_t           r52coreID;// r52 core ID  from 0
    uint32_t           vtb_size; //
    cmd_session_t      vtb[CMD_SESSION_MAX];// vcodec session table
    struct proc_obj    ptb[CMD_SESSION_MAX];// process table

    vcmd_mgr_t*        mtb[VCMD_MGR_ID_MAX];	// vcmd manager  0-vcmd mgr enc, 1- vcmd mgr dec
} cmd_priv_t;


int               cmd_init_priv(cmd_priv_t *priv);

cmd_priv_t*       cmd_get_priv(void);

vcmd_mgr_t*       cmd_get_mgr(uint32_t mgrID);

cmd_session_t*    cmd_get_session(uint32_t sessionID);

struct proc_obj * cmd_get_proc(uint32_t sessionID);


int32_t           cmd_proc(cmdMsg_t *cmdMsg);





#ifdef __cplusplus
}
#endif

#endif /*_CMD_PRIVATE_H_*/
