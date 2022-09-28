//
// @brief  NFC协议
// Created by Ryan on 2022/01/27.
//

#ifndef __FARME_H
#define __FARME_H

 #ifdef __cplusplus
 extern "C" {
 #endif /* __cplusplus */

#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define FARME_MAX_LEN           256 /* 帧最大长度 */
#define FARME_STREAM_BUF_LEN    512 /* 流式解码缓存大小 */

#define FARME_HEAD 0xAA     /* 帧头 */
#define FARME_TAIL 0x55     /* 帧尾 */

#define FARME_NO_DATA_SIZE      7u /* 不包含数据帧最大长度 */
#define FARME_CMD_CMD_MASK      0x1F                            /* CMD命令掩码 */
#define FARME_CMD_CMD_SHIFT     0u                              /* CMD命令偏移 */
#define FARME_CMD_CHANNEL_MASK  0xE0                            /* CMD通道掩码 */
#define FARME_CMD_CHANNEL_SHIFT 5u                              /* CMD通道偏移 */

/* 错误返回 */
typedef enum
{
    FERR_OK = 0,    /* 无错误 */
    FERR_PARA,      /* 传入参数错误 */
    FERR_LEN,       /* 编码或解码长度错误 */
    FERR_MEM,       /* 缓存不足 */
    FERR_CRC,       /* CRC校验错误 */
    FERR_FORMAT,    /* 帧格式错误 */
    FERR_ENCRYPT,   /* 加解密失败 */
} farme_err_t;

/**
 * @brief 帧cmd字段命令类型定义
 */
#define FARME_CMD_ECHO                      0x00    /* 回显指令 */
#define FARME_CMD_REPORT_ACTIVE             0x01    /* 激活卡片通知 */
#define FARME_CMD_REQ_SECTOR_READ           0x02    /* 扇区读卡 */
#define FARME_CMD_RESP_SECTOR_READ          0x03    /* 扇区读卡返回 */
#define FARME_CMD_REQ_SECTOR_WRITE          0x04    /* 扇区写卡 */
#define FARME_CMD_RESP_SECTOR_WRITE         0x05    /* 扇区写卡返回 */
#define FARME_CMD_REQ_SECTOR_VERIFY_KEY     0x06    /* 扇区校验密钥 */
#define FARME_CMD_RESP_SECTOR_VERIFY_KEY    0x07    /* 扇区校验密钥返回 */
#define FARME_CMD_REQ_SECTOR_MODIFY_KEY     0x08    /* 扇区修改密钥 */
#define FARME_CMD_RESP_SECTOR_MODIFY_KEY    0x09    /* 扇区修改密钥返回 */
#define FARME_CMD_REQ_FIRMWARE_VERSION      0xB0    /* 获取版本号*/
#define FARME_CMD_RESP_FIRMWARE_VERSION     0xB1    /* 从机版本号返回*/
#define FARME_CMD_REQ_IAP_RESET             0xB2    /* 跳转进入Ymodem传输*/
#define FARME_CMD_RESP_IAP_RESET            0xB3    /* 进入Ymodem传输模式返回*/

/**
 * @brief 帧state字段状态类型定义
 */
#define FARME_STATE_NORMAL          0x01    /* 正常 */
#define FARME_STATE_ERR_OTHER       0x02    /* 其他错误 */
#define FARME_STATE_ERR_FOUND       0x03    /* 寻卡失败/卡片不存在 */
#define FARME_STATE_ERR_KEY         0x04    /* 密钥校验失败 */
#define FARME_STATE_ERR_WRITE       0x05    /* 写入错误 */
#define FARME_STATE_ERR_WRVERIFY    0x06    /* 写入校验失败 */
#define FARME_STATE_ERR_READ        0x07    /* 读取错误 */
#define FARME_STATE_ERR_NFCCHIP     0x08    /* NFC芯片通信故障 */
#define FARME_STATE_ERR_CRC         0x09    /* CRC校验失败 */
#define FARME_STATE_ON_BOOTLOADER   0x0A    /* 在Bootloader中*/

/* 帧数据结构 */
typedef struct farme_s
{
    uint8_t head;       /* 头 */
    uint8_t cmd;        /* 命令 */
    uint8_t channel;    /* 通道 */
    uint8_t state;      /* 状态 */
    uint8_t size;       /* 大小 */
    uint8_t data[FARME_MAX_LEN];  /* 数据 */
    uint16_t crc16;     /* Modbus CRC16 */
    uint8_t tail;       /* 帧尾 */
} farme_t;

/* 数据流式解码一帧回调函数格式 */
typedef void(*farme_stream_decode_cb_t)(farme_t *farme);

 /**
  * @brief 以默认数据创建帧
  *
  * @param farme        需要编码的帧
  * @param cmd          需要编码的帧
  * @param channel      需要编码的帧
  * @param state        需要编码的帧
  * @param data_size    传入data数组的大小，返回编码后的大小
  * @param data         编码后的数据储存位置
  * @return farme_err_t
  */
farme_err_t farme_create(farme_t* farme, uint8_t cmd, uint8_t channel,
                          uint8_t state, uint32_t data_size, uint8_t *data);

/**
 * @brief 编码帧
 *
 * @param data      编码后的数据储存位置
 * @param size      传入data数组的大小，返回编码后的大小
 * @param farme     需要编码的帧
 * @return farme_err_t
 */
farme_err_t farme_encode(uint8_t* data, uint32_t* size, farme_t* farme);

/**
 * @brief 解码帧
 *
 * @param data      需要解码的数据
 * @param size      需要解码的数据大小
 * @param farme     解码后帧的储存位置
 * @return farme_err_t
 */
farme_err_t farme_decode(const uint8_t* data, uint32_t size, farme_t* farme);

/**
 * @brief 流式解码帧，尽管往该接口填数据，内部会缓存数据直到解码成功释放，解码成功后回调用回调
 *
 * @param data      需要加入流式解码缓存的数据
 * @param size      需要加入流式解码缓存的大小
 * @param cb        解码成功的回调函数
 * @return farme_err_t
 */
farme_err_t farme_stream_decode(uint8_t* data, uint32_t size, farme_stream_decode_cb_t cb);

/**
 * @brief 帧数据加密
 *
 * @param data 输入输出数组
 * @param size 大小
 * @return farme_err_t
 */
farme_err_t farme_data_encrypt(uint8_t* data, uint32_t size);

/**
 * @brief 帧数据解密
 *
 * @param data 输入输出数组
 * @param size 大小
 * @return farme_err_t
 */
farme_err_t farme_data_decrypt(uint8_t* data, uint32_t size);

/**
 * @brief 错误码转换为错误说明
 *
 * @param err      错误码
 * @return const char* 错误说明
 */
const char* farme_err_code(farme_err_t err);

 #ifdef __cplusplus
 }
 #endif /* __cplusplus */

#endif  /* __FARME_H */
