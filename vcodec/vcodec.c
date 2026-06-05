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
**                           *.c vcodec source code                             **
*********************************************************************************/

#include "cmd.h"
#include "vcodec.h"
#include "cmd_priv.h"
#include "vcx_cmdbuf_obj.h"

#ifdef __FREERTOS__
#include "osal_freertos.h" /* needed for the _IOW etc stuff used later */
#endif

static vcodec_config_t g_vcodec_config = {
    .vcmd_isr_polling = 1,
    .vsi_kloglvl = LOGLVL_ERROR,
    .arbiter_weight = 0x1d,
    .arbiter_urgent = 0x00,
    .arbiter_timewindow = 0x1d,
    .arbiter_bw_overflow = 0x00,
    .sw_timeout_time = 1000 * 1000, //SW_TIMEOUT_TIME_FOR_ARBITER,
    .ddr_offset = 0,
#ifdef SUPPORT_MMU
    .mmu_enable = 0,
#endif
};




vcodec_config_t *vcodec_get_config(void) {
    return &g_vcodec_config;
}

int32_t          vcodec_init(cmdMsg_t *cmdMsg) {
    cmd_init_priv(cmd_get_priv());

    return 0;
}

int32_t          vcodec_init_vcmd(cmdMsg_t *cmdMsg) {
    return 0;
}

static int32_t vcodec_run_cmdbuf_0(cmdMsg_t *cmdMsg) {
    vcmd_mgr_t *vcmd_mgr = NULL;
    struct proc_obj *po = NULL;
	struct exchange_parameter param = {0};
    cmdReqRunCmdBuf_Body_t *pbody = (cmdReqRunCmdBuf_Body_t *)cmdMsg->data;
    int32_t ret = 0;
    vcmd_mgr = cmd_get_mgr(pbody->vcmdmgr_id);
    if (vcmd_mgr == NULL) {
        return -1;
    }

    param.interrupt_ctrl = pbody->interrupt_ctrl;
    param.module_type = pbody->module_type;
    param.cmdbuf_size = pbody->cmdbuf_size;
    param.cmdbuf_id = pbody->cmdbuf_id;
    param.core_id = pbody->core_id;
    param.core_mask = pbody->core_mask;
    param.input_mask = pbody->input_mask;
    ret = link_and_run_cmdbuf(vcmd_mgr, po, &param);
    pbody->core_id = param.core_id;
    {//
        uint8_t bufMsg[64] = {0};
        cmdMsg_t *cmdMsg1 = (cmdMsg_t *)bufMsg;
        cmdRspRunCmdBuf_Body_t *pbody1 = (cmdRspRunCmdBuf_Body_t *)cmdMsg1->data;
        cmd_init(cmdMsg1);
        cmdMsg1->cmdType = CMD_RSP_RUN_CMDBUF;
        cmdMsg1->sessionID = cmdMsg->sessionID;
        cmdMsg1->procObj = cmdMsg->procObj;
        cmdMsg1->timeStamp = cmdMsg->timeStamp;
        cmdMsg1->seqNum = cmdMsg->seqNum;
        cmdMsg1->cmdSize = CMD_MSG_MIN_SIZE + sizeof(cmdRspRunCmdBuf_Body_t);
        pbody1->ackNum     = cmdMsg->seqNum;
        pbody1->code       = ret;
        pbody1->vcmdmgr_id = pbody->vcmdmgr_id;
        pbody1->cmdbuf_id = pbody->cmdbuf_id;
        pbody1->core_id = pbody->core_id;
        ret = cmd_send(cmdMsg1);
    }

    return ret;
}


int32_t          vcodec_run_cmdbuf(cmdMsg_t *cmdMsg){
#if 0
    return vcodec_run_cmdbuf_0(cmdMsg);
#else
    vcmd_mgr_t *vcmd_mgr = NULL;
    cmdReqRunCmdBuf_Body_t *cmdBody = (cmdReqRunCmdBuf_Body_t *)cmdMsg->data;
    int32_t ret = 0;
    vcmd_mgr = cmd_get_mgr(cmdBody->vcmdmgr_id);
    if (vcmd_mgr == NULL) {
        return -1;
    }

    ret = vcmd_link_and_rum_cmdbuf(vcmd_mgr, cmdBody);
    {//
        uint8_t bufMsg[64] = {0};
        cmdMsg_t *cmdMsg1 = (cmdMsg_t *)bufMsg;
        cmdRspRunCmdBuf_Body_t *cmdBody1 = (cmdRspRunCmdBuf_Body_t *)cmdMsg1->data;
        cmd_init(cmdMsg1);
        cmdMsg1->cmdType = CMD_RSP_RUN_CMDBUF;
        cmdMsg1->sessionID = cmdMsg->sessionID;
        cmdMsg1->procObj = cmdMsg->procObj;
        cmdMsg1->timeStamp = cmdMsg->timeStamp;
        cmdMsg1->seqNum = cmdMsg->seqNum;
        cmdMsg1->cmdSize = CMD_MSG_MIN_SIZE + sizeof(cmdRspRunCmdBuf_Body_t);
        cmdBody1->ackNum     = cmdMsg->seqNum;
        cmdBody1->code       = ret;
        cmdBody1->vcmdmgr_id = cmdBody->vcmdmgr_id;
        cmdBody1->cmdbuf_id = cmdBody->cmdbuf_id;
        cmdBody1->core_id = cmdBody->core_id;
        ret = cmd_send(cmdMsg1);
    }

    return ret;
#endif
}