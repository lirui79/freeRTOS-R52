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
**                      include command private source                          **
*********************************************************************************/

#include "cmd.h"
#include "crc32.h"
#include "cmd_priv.h"
#include "cmd_session.h"
#include "vcx_vcmd_priv.h"



static cmd_priv_t g_cmd_priv;



int             cmd_init_priv(cmd_priv_t *priv) {
    int32_t i = 0;
    priv->r52coreID = 0;// r52 core ID  from 0
    priv->vtb_size  = CMD_SESSION_MAX; // r52 core number
    for (i = 0; i < priv->vtb_size; i++) {
        cmd_session_init(&priv->vtb[i], &priv->ptb[i]);
    }

    for (i = 0; i < VCMD_MGR_ID_MAX; i++) {
        priv->mtb[i] = NULL;
    }
    return 0;
}

vcmd_mgr_t*    cmd_get_mgr(uint32_t mgrID) {
    if (mgrID >= VCMD_MGR_ID_MAX) {
        return NULL;
    }
    return cmd_get_priv()->mtb[mgrID];
}

cmd_session_t*    cmd_get_session(uint32_t sessionID) {
    if (sessionID >= CMD_SESSION_MAX) {
        return NULL;
    }
    return &cmd_get_priv()->vtb[sessionID];
}

struct proc_obj * cmd_get_proc(uint32_t sessionID) {
    if (sessionID >= CMD_SESSION_MAX) {
        return NULL;
    }
    return &cmd_get_priv()->ptb[sessionID];   
}

struct proc_obj * cmd_get_proc(uint32_t sessionID);

cmd_priv_t *cmd_get_priv(void) {
    return &g_cmd_priv;
}

int32_t cmd_proc(cmdMsg_t *cmdMsg) {
    cmd_session_t *session = NULL;
    if (cmdMsg == NULL) {
        return -0x100;
    }

    if (cmdMsg->magic != CMD_MAGIC_NUMBER) {
        return -1;
    }

    if (cmdMsg->version != CMD_VERSION) {
        return -2;
    }

    {
        uint32_t crc32 = 0, crc32Now = 0;

        crc32 = cmdMsg->crc32;
        cmdMsg->crc32 = 0;
        
        crc32Now = crc32_calc((const uint8_t *)cmdMsg, cmdMsg->cmdSize);
        if (crc32 != crc32Now) {
            return -3;
        }
    }

    {
        uint32_t  r52ID  = ((cmdMsg->sessionID & 0xFFFF0000) >> 16);
        uint32_t  sesID  = (cmdMsg->sessionID & 0xFFFF);
        if (r52ID != cmd_get_priv()->r52coreID) {
            return -4;
        }

        session = cmd_get_session(sesID);
        if (session == NULL) {
            return -5;
        }

        if (cmd_session_check(session, cmdMsg) < 0) {
            return -6;
        }
    }

    if (cmdMsg->cmdType <= CMD_SYSTEM_MAX) {
        return cmd_session_system(session, cmdMsg);
    }

    if ((cmdMsg->cmdType >= CMD_VCODEC_MIN) && (cmdMsg->cmdType <= CMD_VCODEC_MAX)) {
        return cmd_session_vcodec(session, cmdMsg);
    }
    return -7;
}
