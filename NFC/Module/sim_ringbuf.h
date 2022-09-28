//
// @brief  通用环形缓冲区
// Created by Ryan on 2021/12/31.
//

#ifndef __SIM_RINGBUFR_H
#define __SIM_RINGBUFR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#define SIM_MALLOC  malloc
#define SIM_FREE    free
#define SIM_MEMSET  memset
#define SIM_MEMCPY  memcpy

#define RB_MAX_SIZE 1024 /* 使用动态内存初始化时最大大小的限制 */

/* 环形缓冲区控制结构体 */
typedef struct
{
    /* 缓冲区容量 */
    size_t  size;
    /* 指向接收数据头部 */
    size_t  head;
    /* 指向接收数据末尾 */
    size_t  tail;
    /* 缓冲区指针 */
    void    *buff;
    /* 缓冲区满标志 */
    uint8_t    is_full;
}sringbuf_t;

/*!
 * @brief 初始化环形缓冲区，申请缓冲区内存
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   size    需要缓冲区的大小
 *
 * @return  成功或失败
 * @retval  1       成功
 * @retval  0       失败
 */
uint8_t srb_init(sringbuf_t *rb, size_t size);

/*!
 * @brief 初始化环形缓冲区，使用静态内存
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   buff    申请好内存的静态数组
 * @param[in]   size    需要缓冲区的大小
 * 
 * @return  成功或失败
 * @retval  1       成功
 * @retval  0       失败
 */
uint8_t srb_static_init(sringbuf_t *rb, uint8_t *buff, size_t size);

/*!
 * @brief 解初始化环形缓冲区，释放被申请的内存
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 *
 * @return  成功或失败
 * @retval  1       成功
 * @retval  0       失败
 */
uint8_t srb_deinit(sringbuf_t *rb);

/*!
 * @brief 解初始化环形缓冲区，使用静态内存
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 *
 * @return  成功或失败
 * @retval  1       成功
 * @retval  0       失败
 */
uint8_t srb_static_deinit(sringbuf_t *rb);

/*!
 * @brief 刷新缓冲区，也就是清空缓冲区
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 *
 * @return  无
 */
uint8_t srb_fflush(sringbuf_t *rb);

/*!
 * @brief 获取缓冲区可写入的大小
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 *
 * @return 可写入的大小
 */
size_t srb_unwrite_bytes(sringbuf_t *rb);

/*!
 * @brief 获取缓冲区可读取的大小
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 *
 * @return 可读取的大小
 */
size_t srb_unread_bytes(sringbuf_t *rb);

/*!
 * @brief 向缓冲区写入一字节
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   data    需要写入的一字节数据
 *
 * @return 实际写入的大小, 0 为写入失败
 */
size_t srb_write_byte(sringbuf_t *rb, const uint8_t data);

/*!
 * @brief 向缓冲区写入指定大小的数据
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   data    需要写入数据的指针
 * @param[in]   size    需要写入数据的大小
 *
 * @return 实际写入的大小, 0 为写入失败
 */
size_t srb_write(sringbuf_t *rb ,const void *data, size_t size);

/*!
 * @brief 从缓冲区读出一字节
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   data    储存读出数据的指针
 *
 * @return 实际读取的大小, 0 为读取失败
 */
size_t srb_read_byte(sringbuf_t *rb, void *data);

/*!
 * @brief 从缓冲区读出指定大小的数据
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   data    储存读出数据的指针
 * @param[in]   size    需要读取数据的大小
 *
 * @return 实际读取的大小, 0 为读取失败
 */
size_t srb_read(sringbuf_t *rb, void *data, size_t size);

/*!
 * @brief 从缓冲区窥探指定大小的数据，该数据不会从缓存区删除
 *
 * @param[in]   rb	    环形缓冲区控制块地址指针
 * @param[in]   data    储存读出数据的指针
 * @param[in]   size    需要读取数据的大小
 *
 * @return 实际窥探的大小, 0 为读取失败
 */
size_t srb_peek(sringbuf_t* rb, void* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif  /* __SIM_RINGBUFR_H */
