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
**                              include cmd define header                       **
*********************************************************************************/

#ifndef _COMMAND_DEFINE_H_
#define _COMMAND_DEFINE_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define  CMD_MAGIC_NUMBER          0xA785
#define  CMD_VERSION               0x01
#define  CMD_DEFAULT_PRIORITY      0x00

enum cmdType {
/*
*  system command between 0x00 to 0xFF
*/

    CMD_SYSTEM_MIN                 = 0x00,
    CMD_REQ_EXE_SYSCTL             = 0x00,
    CMD_RSP_EXE_SYSCTL             = 0x01,
    CMD_REQ_SET_SYSCFG             = 0x02,
    CMD_RSP_SET_SYSCFG             = 0x03,
    CMD_REQ_GET_SYSCFG             = 0x04,
    CMD_RSP_GET_SYSCFG             = 0x05,
    CMD_REQ_SET_LOGCFG             = 0x06,
    CMD_RSP_SET_LOGCFG             = 0x07,
    CMD_REQ_GET_LOGCFG             = 0x08,
    CMD_RSP_GET_LOGCFG             = 0x09,
    CMD_REQ_GET_SYSTATE            = 0x0A,
    CMD_RSP_GET_SYSTATE            = 0x0B,
    CMD_REQ_OPEN_SESSION           = 0x0C,
    CMD_RSP_OPEN_SESSION           = 0x0D,
    CMD_REQ_CLOSE_SESSION          = 0x0E,
    CMD_RSP_CLOSE_SESSION          = 0x0F,

    CMD_EVT_REPORT_SYSTATE         = 0x20,
    CMD_SYSTEM_MAX                 = 0xFF,

/*
*  vcodec command between 0x100 to 0x1FF
*/
    CMD_VCODEC_MIN                 = 0x100,
    CMD_REQ_RUN_CMDBUF             = 0x100,
    CMD_RSP_RUN_CMDBUF             = 0x101,
    CMD_EVT_REPORT_CMDBUF_READY    = 0x102,
    CMD_REQ_ABORT_CMDBUF           = 0x103,
    CMD_RSP_ABORT_CMDBUF           = 0x104,
    CMD_REQ_GET_VCMDSTATE          = 0x105,
    CMD_RSP_GET_VCMDSTATE          = 0x106,

    CMD_VCODEC_MAX                 = 0x1FF,
};

typedef struct __attribute__((packed, aligned(8))) {
    uint16_t       magic;// magic number, default 0xA785
    uint8_t        version;// version, default 0x01
    uint8_t        prior;// priority level, default 0x00
    uint32_t       cmdType;// command type  enum cmdType 
    uint32_t       cmdSize;// command size  (include cmd header and body)
    uint32_t       sessionID;// session id
    uint64_t       procObj;// process object id
    uint64_t       timeStamp;// time stamp, default current time in ms
    uint32_t       seqNum;// sequence number, from 0 to 0xFFFFFFFF
    uint32_t       crc32;// crc32 checksum of include cmd header and body, crc32 set 0x00 before crc32 calculation
    uint8_t        data[0]; //cmdbody
} cmdMsg_t;

#define CMD_MSG_MIN_SIZE       sizeof(cmdMsg_t)

#define CMD_MSG_MALLOC(payload_len) \
    pvPortMalloc(CMD_MSG_MIN_SIZE + (payload_len))

#define CMD_MSG_FREE(pMsg) \
    vPortFree(pMsg)


typedef struct __attribute__((packed, aligned(8))) {//CMD_REQ_EXE_SYSCTL
    uint32_t       control;// 0 - hardware reset    1 - software reset
} cmdReqSysctl_Body_t;


typedef struct __attribute__((packed, aligned(8))) {//CMD_RSP_EXE_SYSCTL
    uint32_t       ackNum;// seqNum
    uint32_t       code;// 0 - success, > 0 - fail
} cmdRspSysctl_Body_t;


#define VCMD_MGR_ID_ENC  0
#define VCMD_MGR_ID_DEC  1
#define VCMD_MGR_ID_MAX  2

typedef struct __attribute__((packed, aligned(8))) {//CMD_REQ_RUN_CMDBUF
    uint64_t       interrupt_ctrl;
    uint32_t       vcmdmgr_id;
    uint16_t       module_type;
    uint16_t       cmdbuf_size;
    uint16_t       cmdbuf_id;
    uint16_t       core_id;
    uint16_t       core_mask;
    uint16_t       input_mask;
} cmdReqRunCmdBuf_Body_t;


typedef struct __attribute__((packed, aligned(8))) {//CMD_RSP_RUN_CMDBUF
    uint32_t       ackNum;// seqNum
    uint32_t       code;// 0 - success, > 0 - fail
    uint32_t       vcmdmgr_id;
    uint16_t       cmdbuf_id;
    uint16_t       core_id;
} cmdRspRunCmdBuf_Body_t;


typedef struct __attribute__((packed, aligned(8))) {//CMD_EVT_REPORT_CMDBUF_READY
    uint32_t       status;// 0 - success, > 0 - fail
    uint32_t       vcmdmgr_id;
    uint16_t       cmdbuf_id;
} cmdEvtRepCmdBufReady_Body_t;

enum cmdError {
    CMD_ERR_SUCCESS                = 0x00,

    CMD_ERR_INVALID_POINTER        = 0x01,
    CMD_ERR_INVALID_MAGIC          = 0x02,
    CMD_ERR_INVALID_VERSION        = 0x03,
    CMD_ERR_CHECKSUM_CRC32         = 0x04,
    CMD_ERR_INVALID_CMDBUF         = 0x05,
    CMD_ERR_INVALID_PARAM          = 0x06,
};

#ifdef __cplusplus
}
#endif

#endif /*_COMMAND_DEFINE_H_*/
