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
**                         include vcx cmdbuf oby header                        **
*********************************************************************************/

#ifndef _VCX_CMDBUF_OBJ_H_
#define _VCX_CMDBUF_OBJ_H_

#ifdef __FREERTOS__
#include "osal_freertos.h" /* needed for the _IOW etc stuff used later */
#endif

#include "vcx_vcmd.h"
#include "vcx_vcmd_priv.h"


#ifdef __cplusplus
extern "C" {
#endif


void vcmd_init_objs(vcmd_mgr_t *vcmd_mgr);

void vcmd_init_nodes(vcmd_mgr_t *vcmd_mgr);

void vcmd_init_free_obj_list(vcmd_mgr_t *vcmd_mgr);

void return_cmdbuf(vcmd_mgr_t *vcmd_mgr, u32 id);

bi_list_node *get_cmdbuf_node_by_addr(vcmd_mgr_t *vcmd_mgr,
					struct hantrovcmd_dev *dev, ptr_t pa);

struct proc_obj *create_process_object(void);

void free_process_object(struct proc_obj *po);

void proc_add_done_job(vcmd_mgr_t *vcmd_mgr, struct cmdbuf_obj *obj);

int dev_delink_job(struct hantrovcmd_dev *dev,
							bi_list_node *node);

int dev_remove_job(vcmd_mgr_t *vcmd_mgr, struct hantrovcmd_dev *dev,
						bi_list_node *node);

void vcmd_start(struct hantrovcmd_dev *dev, int irq);

int vcmd_abort(vcmd_mgr_t *vcmd_mgr, struct hantrovcmd_dev *dev,
					u32 *aborted_id);

long reserve_cmdbuf(vcmd_mgr_t *vcmd_mgr, struct proc_obj *po,
							struct exchange_parameter *param);

long release_cmdbuf(vcmd_mgr_t *vcmd_mgr,
							struct proc_obj *po, u16 cmdbuf_id);

long link_and_run_cmdbuf(vcmd_mgr_t *vcmd_mgr, struct proc_obj *po,
								struct exchange_parameter *param);

long wait_cmdbuf_ready(vcmd_mgr_t *vcmd_mgr, struct proc_obj *po,
								u16 cmdbuf_id, u16 *done_id);

int _is_abnormal_run_done(struct cmdbuf_obj *obj);

void _abnormal_run_done_clear(struct cmdbuf_obj *obj);

void vcmd_reset_asic(vcmd_mgr_t *vcmd_mgr);

int abort_vce(volatile u8 *reg_base);

int abort_vcd(volatile u8 *reg_base);

u32 vcmd_abort_mode_set(vcmd_mgr_t *vcmd_mgr, struct hantrovcmd_dev *dev,
							struct cmdbuf_obj *obj);

void vcmd_get_executing_cmdbuf(vcmd_mgr_t *vcmd_mgr, struct hantrovcmd_dev *dev,
									bi_list_node **node);

#ifdef __cplusplus
}
#endif

#endif /*_VCX_CMDBUF_OBJ_H_*/
