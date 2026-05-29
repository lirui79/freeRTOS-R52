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
**                            include vcodec header                             **
*********************************************************************************/

#ifndef _VCODEC_H_
#define _VCODEC_H_

#ifdef __FREERTOS__
#include "osal_freertos.h" /* needed for the _IOW etc stuff used later */
#endif


enum {
	LOGLVL_VERBOSE = 0,		// log all
	LOGLVL_FLOW,			// log all debug msg
	LOGLVL_CONFIG,			// log ctrl/config info, mostly at beginning
	LOGLVL_BRIEF,			// log critical point
	LOGLVL_WARNING,			// log warning msg
	LOGLVL_ERROR,			// log error
};
/* declarations */

#define vcmd_klog(lvl, fmt, ...) {\
	if (lvl == LOGLVL_ERROR)	\
		printf("ERROR hantrovcmd: " fmt, ##__VA_ARGS__);	\
	else if (lvl >= vcodec_get_config()->vsi_kloglvl)		\
		printf("INFO hantrovcmd: " fmt, ##__VA_ARGS__);	\
}


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
//    uint32_t vcmd_supported;
    unsigned int     vcmd_isr_polling;
    unsigned int     vsi_kloglvl;
    unsigned int     arbiter_weight;
    unsigned int     arbiter_urgent;
    unsigned int     arbiter_timewindow;
    unsigned int     arbiter_bw_overflow;
    unsigned long    sw_timeout_time;
    unsigned long    ddr_offset;
#ifdef SUPPORT_MMU
    unsigned int     mmu_enable;
#endif
} vcodec_config_t;


vcodec_config_t *vcodec_get_config(void);


#ifdef __cplusplus
}
#endif

#endif /*_VCODEC_H_*/
