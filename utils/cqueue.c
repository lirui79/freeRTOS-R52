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
**                            *.c queue source code                             **
*********************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "cqueue.h"

#ifdef __FREERTOS__
#include "osal_freertos.h" /* needed for the _IOW etc stuff used later */
#endif

typedef struct {
    uint8_t*    data;
    uint32_t    front;
    uint32_t    rear;
    uint32_t    qSize;
    uint32_t    count;
    uint32_t    iSize;
} CQueue_t;


CQueueHandle_t CQueueCreate(uint32_t qSize, uint32_t iSize) {
    CQueue_t* queue = NULL;
    if (qSize < 1 || iSize < 1) {
        return NULL;
    }

    queue = (CQueue_t*)vmalloc(sizeof(CQueue_t));
    if (queue == NULL) {
        return NULL;
    }
    queue->front = 0;
    queue->rear  = 0;
    queue->qSize = qSize;
    queue->count = 0;
    queue->iSize = iSize;
    queue->data  = vmalloc(qSize * iSize);
    if (queue->data == NULL) {
        vfree(queue);
        return NULL;
    }
    return queue;
}

void CQueueDelete(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    if (queue == NULL) {
        return;
    }
    if (queue->data != NULL) {
        vfree(queue->data);
    }
    vfree(queue);
}

uint32_t  CQueueSize(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    if (queue == NULL) {
        return 0;
    }
    return queue->qSize;
}

uint32_t  CQueueCount(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    if (queue == NULL) {
        return 0;
    }
    return queue->count;
}


uint32_t  CQueueItemSize(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    if (queue == NULL) {
        return 0;
    }
    return queue->iSize;
}

int32_t  CQueueEnqueue(CQueueHandle_t handle, void* item) {
    CQueue_t* queue = (CQueue_t*)handle;
    taskENTER_CRITICAL();
    if ((queue == NULL) || (queue->count >= queue->qSize)) {
        taskEXIT_CRITICAL();
        return -1;
    }
    memcpy(queue->data + queue->rear * queue->iSize, item, queue->iSize);
    queue->rear = (queue->rear + 1) % queue->qSize;
    queue->count++;
    taskEXIT_CRITICAL();
    return 0;
}

void*     CQueueDequeue(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    void *item = NULL;
    taskENTER_CRITICAL();
    if ((queue == NULL) || (queue->count <= 0)) {
        taskEXIT_CRITICAL();
        return NULL;
    }
    item = queue->data + queue->front * queue->iSize;
    queue->front = (queue->front + 1) % queue->qSize;
    queue->count--;
    taskEXIT_CRITICAL();
    return item;
}

void*     CQueuePeek(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    void *item = NULL;
    taskENTER_CRITICAL();
    if ((queue == NULL) || (queue->count <= 0)) {
        taskEXIT_CRITICAL();
        return NULL;
    }
    item = queue->data + queue->front * queue->iSize;
    taskEXIT_CRITICAL();
    return item;
}


int32_t  CQueueEnqueueFromISR(CQueueHandle_t handle, void* item) {
    CQueue_t* queue = (CQueue_t*)handle; 
    UBaseType_t flags = taskENTER_CRITICAL_FROM_ISR();
    if ((queue == NULL) || (queue->count >= queue->qSize)) {
        taskEXIT_CRITICAL_FROM_ISR(flags);
        return -1;
    }
    memcpy(queue->data + queue->rear * queue->iSize, item, queue->iSize);
    queue->rear = (queue->rear + 1) % queue->qSize;
    queue->count++;
    taskEXIT_CRITICAL_FROM_ISR(flags);
    return 0;
}

void*     CQueueDequeueFromISR(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    void *item = NULL;
    UBaseType_t flags = taskENTER_CRITICAL_FROM_ISR();
    if ((queue == NULL) || (queue->count <= 0)) {
        taskEXIT_CRITICAL_FROM_ISR(flags);
        return NULL;
    }
    item = queue->data + queue->front * queue->iSize;
    queue->front = (queue->front + 1) % queue->qSize;
    queue->count--;
    taskEXIT_CRITICAL_FROM_ISR(flags);
    return item;
}

void*     CQueuePeekFromISR(CQueueHandle_t handle) {
    CQueue_t* queue = (CQueue_t*)handle;
    void *item = NULL;
    UBaseType_t flags = taskENTER_CRITICAL_FROM_ISR();
    if ((queue == NULL) || (queue->count <= 0)) {
        taskEXIT_CRITICAL_FROM_ISR(flags);
        return NULL;
    }
    item = queue->data + queue->front * queue->iSize;
    taskEXIT_CRITICAL_FROM_ISR(flags);
    return item;
}
