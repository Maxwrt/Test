//
// @brief  通用环形缓冲区
// Created by Ryan on 2021/12/30.
//

#include "sim_ringbuf.h"

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

/* 初始化环形缓冲区，申请缓冲区内存 */
uint8_t srb_init(sringbuf_t *rb, size_t size)
{
    if (rb == NULL)
    {
        return false;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = false;
    rb->buff = NULL;
    if (size > RB_MAX_SIZE)
    {
        return false;
    }
    rb->size = size;
    rb->buff = (void *)SIM_MALLOC(rb->size);
    if (rb->buff == NULL)
    {
        return false;
    }
    return true;
}

/* 初始化环形缓冲区，使用静态内存 */
uint8_t srb_static_init(sringbuf_t *rb, uint8_t *buff, size_t size)
{
    if (rb == NULL || buff == NULL)
    {
        return false;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = false;
    rb->buff = NULL;
    
    rb->size = size;
    rb->buff = buff;
    if (rb->buff == NULL)
    {
        return false;
    }

    return true;
}

/* 解初始化环形缓冲区，释放被申请的内存 */
uint8_t srb_deinit(sringbuf_t *rb)
{
    if (rb == NULL || rb->buff == NULL)
    {
        return false;
    }
    SIM_FREE(rb->buff);
    rb->buff = NULL;
    rb->size = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = false;
    return true;
}

/* 解初始化环形缓冲区，使用静态内存 */
uint8_t srb_static_deinit(sringbuf_t *rb)
{
    if (rb == NULL || rb->buff == NULL)
    {
        return false;
    }
    rb->buff = NULL;
    rb->size = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = false;
    return true;
}

/* 刷新缓冲区，也就是清空缓冲区 */
uint8_t srb_fflush(sringbuf_t *rb)
{
    if (rb == NULL || rb->buff == NULL)
    {
        return false;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->is_full = false;
    
    SIM_MEMSET(rb->buff, 0, rb->size);
    
    return true;
}

/* 获取缓冲区可写入的大小 */
size_t srb_unwrite_bytes(sringbuf_t *rb)
{
    if (rb == NULL || rb->buff == NULL)
    {
        return 0;
    }
    if (rb->is_full == true)
    {
        return 0;
    }
    else 
    {
        if (rb->tail >= rb->head)
        {
            return (rb->size - rb->tail + rb->head);
        }
        else
        {
            return (rb->head - rb->tail);
        }
    }
}

/* 获取缓冲区可读取的大小 */
size_t srb_unread_bytes(sringbuf_t *rb)
{
    if (rb == NULL || rb->buff == NULL)
    {
        return 0;
    }
    if (rb->tail > rb->head)
    {
        return (rb->tail - rb->head);
    }
    else if (rb->tail < rb->head)
    {
        return (rb->size - rb->head + rb->tail);
    }
    else
    {
        if (rb->is_full == true)
        {
            return rb->size;
        }
        else
        {
            return 0;
        }
    }
}

/* 向缓冲区写入一字节 */
size_t srb_write_byte(sringbuf_t *rb, const uint8_t data)
{
    size_t write_size = 0;
    size_t unwrite_bytes = srb_unwrite_bytes(rb);
    if (rb == NULL || rb->buff == NULL || unwrite_bytes == 0)
    {
        return write_size;
    }
    ((uint8_t *)rb->buff)[rb->tail] = data;
    write_size++;
    if (rb->tail + 1 == rb->size)
    {
        rb->tail = 0;
    }
    else
    {
        rb->tail++;
    }
    if (rb->tail == rb->head)
    {
        rb->is_full = true;
    }
    return write_size;
}

/* 向缓冲区写入指定大小的数据 */
size_t srb_write(sringbuf_t *rb ,const void *data, size_t size)
{
    size_t write_size = 0, front_size = 0, back_size = 0;
    size_t unwrite_bytes = srb_unwrite_bytes(rb);
    if (rb == NULL || rb->buff == NULL || unwrite_bytes == 0 )
    {
        return write_size;
    }
    if (unwrite_bytes >= size)
    {
        write_size = size;
    } else
    {
        write_size = unwrite_bytes;
        rb->is_full = true;
    }
    if (rb->tail + write_size <= rb->size)
    {
        SIM_MEMCPY( (void *)((uintptr_t)rb->buff + rb->tail), data, write_size);
        if (rb->tail + write_size < rb->size)
        {
            rb->tail += write_size;
        } else
        {
            rb->tail = 0;
        }
    } else
    {
        front_size = rb->size - rb->tail;
        back_size = write_size - front_size;
        SIM_MEMCPY((void *)((uintptr_t)rb->buff + rb->tail) , data, front_size);
        SIM_MEMCPY(rb->buff, (void *)((uintptr_t)data + front_size), back_size);
        rb->tail = back_size;
    }
    return write_size;
}

/* 从缓冲区读出一字节 */
size_t srb_read_byte(sringbuf_t *rb, void *data)
{
    size_t read_size = 0;
    size_t unread_bytes = srb_unread_bytes(rb);
    if (rb == NULL || rb->buff == NULL || unread_bytes == 0)
    {
        return read_size;
    }
    if (rb->is_full == true)
    {
        rb->is_full = false;
    }
    *(uint8_t *)data = ((uint8_t *)rb->buff)[rb->head];
    read_size++;
    if (rb->head + 1 == rb->size)
    {
        rb->head = 0;
    }
    else
    {
        rb->head++;
    }

    return read_size;
}

/* 从缓冲区读出指定大小的数据 */
size_t srb_read(sringbuf_t *rb, void *data, size_t size)
{
    size_t read_size = 0, front_size = 0, back_size = 0;
    size_t unread_bytes = srb_unread_bytes(rb);
    if (rb == NULL || rb->buff == NULL || unread_bytes == 0)
    {
        return read_size;
    }
    if (rb->is_full == true)
    {
        rb->is_full = false;
    }
    if (unread_bytes >= size)
    {
        read_size = size;
    }
    else
    {
        read_size = unread_bytes;
    }
    if (rb->head + read_size <= rb->size)
    {
        SIM_MEMCPY(data, (void *)((uintptr_t)rb->buff + rb->head), read_size);
        if (rb->head + read_size < rb->size)
        {
            rb->head += read_size;
        }
        else
        {
            rb->head = 0;
        }
    } else
    {
        front_size = rb->size - rb->head;
        back_size = read_size - front_size;
        SIM_MEMCPY(data, (void *)((uintptr_t)rb->buff + rb->head), front_size);
        SIM_MEMCPY( (void *)((uintptr_t)data + front_size), rb->buff, back_size);
        rb->head = back_size;
    }
    return read_size;
}

/* 从缓冲区窥探指定大小的数据，该数据不会从缓存区删除 */
size_t srb_peek(sringbuf_t* rb, void* data, size_t size)
{
    size_t read_size = 0, front_size = 0, back_size = 0;
    size_t unread_bytes = srb_unread_bytes(rb);
    if (rb == NULL || rb->buff == NULL || unread_bytes == 0)
    {
        return read_size;
    }
    if (rb->is_full == true)
    {
        rb->is_full = false;
    }
    if (unread_bytes >= size)
    {
        read_size = size;
    }
    else
    {
        read_size = unread_bytes;
    }
    if (rb->head + read_size <= rb->size)
    {
        SIM_MEMCPY(data, (void*)((uintptr_t)rb->buff + rb->head), read_size);
        // if (rb->head + read_size < rb->size)
        // {
        //     rb->head += read_size;
        // }
        // else
        // {
        //     rb->head = 0;
        // }
    }
    else
    {
        front_size = rb->size - rb->head;
        back_size = read_size - front_size;
        SIM_MEMCPY(data, (void*)((uintptr_t)rb->buff + rb->head), front_size);
        SIM_MEMCPY((void*)((uintptr_t)data + front_size), rb->buff, back_size);
        // rb->head = back_size;
    }
    return read_size;
}