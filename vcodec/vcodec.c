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

#include "vcodec.h"

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
