//
// @brief  NFC协议
// Created by Ryan on 2022/01/27.
//

#include "encrypt.h"
#include "farme.h"


/* 加解密替换表 */
encrypt_table_t encrypt_table =
{
    {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF},
    {0xA, 0xC, 0xE, 0xF, 0xD, 0xB, 0x9, 0x7, 0x5, 0x3, 0x1, 0x0, 0x2, 0x4, 0x6, 0x8},
};

static farme_t temp_farme;
static uint8_t stream_buff[FARME_STREAM_BUF_LEN] = { 0 };
static uint32_t stream_index = 0;

static uint16_t farme_crc16(const uint8_t* pFrame, uint16_t Len)
{
    static const uint16_t crc16Table[] =
    {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    uint8_t buf;
    uint16_t crc16 = 0xFFFF;
    while (Len--)
    {
        buf = *pFrame++ ^ crc16;
        crc16 >>= 8;
        crc16 ^= crc16Table[buf];
    }
    return crc16;
}

/**
 * @brief 以默认数据创建帧
 */
farme_err_t farme_create(farme_t* farme, uint8_t cmd, uint8_t channel,
    uint8_t state, uint32_t data_size, uint8_t* data)
{
    if (data_size + FARME_NO_DATA_SIZE >= FARME_MAX_LEN)
    {
        return FERR_LEN;
    }
    if (data_size != 0 && data == NULL)
    {
        return FERR_PARA;
    }
    farme->head = FARME_HEAD;
    farme->tail = FARME_TAIL;
    farme->cmd = cmd;
    farme->channel = channel;
    farme->state = state;
    farme->size = data_size;
    for (int i = 0; i < data_size;i++)
    {
        farme->data[i] = data[i];
    }
    return FERR_OK;
}

static uint8_t is_mainly_frame(uint8_t data);
/**
 * @brief 编码帧
 */
farme_err_t farme_encode(uint8_t* data, uint32_t* size, farme_t* farme)
{
    if (data == NULL || size == NULL || farme == NULL)
    {
        return FERR_PARA;
    }
    if (farme->size + FARME_NO_DATA_SIZE >= FARME_MAX_LEN ||
        *size < FARME_NO_DATA_SIZE ||
        farme->size + FARME_NO_DATA_SIZE >= *size)
    {
        return FERR_LEN;
    }
    int index = 0;
    data[index++] = FARME_HEAD;
    if(is_mainly_frame(farme->cmd) != 1)
    {
        data[index++] = farme->cmd;
    }
    else
    {
        data[index++] = (farme->cmd << FARME_CMD_CMD_SHIFT & FARME_CMD_CMD_MASK) | (farme->channel << FARME_CMD_CHANNEL_SHIFT & FARME_CMD_CHANNEL_MASK);
    }
    data[index++] = farme->state;
    data[index++] = farme->size;
    for (int i = 0; i < farme->size; i++)
    {
        data[index++] = farme->data[i];
    }
    farme->crc16 = farme_crc16(data, index);
    data[index++] = (farme->crc16 >> 8) & 0xFF;
    data[index++] = (farme->crc16 >> 0) & 0xFF;
    data[index++] = farme->tail;

    *size = index;

    return FERR_OK;
}

/**
 * @brief 解码帧
 */
farme_err_t farme_decode(const uint8_t* data, uint32_t size, farme_t* farme)
{
    if (data == NULL || farme == NULL)
    {
        return FERR_PARA;
    }
    if (size < FARME_NO_DATA_SIZE)
    {
        return FERR_LEN;
    }

    farme->head = data[0];
    if(is_mainly_frame(data[1]) != 1)
    {
        farme->cmd = data[1];
    }
    else
    {
        farme->cmd = (data[1] & FARME_CMD_CMD_MASK) >> FARME_CMD_CMD_SHIFT;
    }
    farme->channel = (data[1] & FARME_CMD_CHANNEL_MASK) >> FARME_CMD_CHANNEL_SHIFT;
    farme->state = data[2];
    farme->size = data[3];
    if (farme->size + FARME_NO_DATA_SIZE != size)
    {
        return FERR_LEN;
    }

    for (int i = 0;i < farme->size; i++)
    {
        farme->data[i] = data[4 + i];
    }

    uint16_t crc16 = farme_crc16(data, 4 + farme->size);

    farme->crc16 = data[4 + farme->size + 0] << 8 | data[4 + farme->size + 1];
    farme->tail = data[4 + farme->size + 2];

    if (crc16 != farme->crc16)
    {
        return FERR_CRC;
    }

    if (farme->head != FARME_HEAD ||
        farme->tail != FARME_TAIL)
    {
        return FERR_FORMAT;
    }

    return FERR_OK;
}

/**
 * @brief 流式解码帧，尽管往该接口填数据，内部会缓存数据直到解码成功释放，解码成功后回调用回调
 */
farme_err_t farme_stream_decode(uint8_t* data, uint32_t size, farme_stream_decode_cb_t cb)
{
    farme_err_t ret;
    uint32_t find_index = 0;
    uint32_t copy_size = FARME_STREAM_BUF_LEN - stream_index > size ? size : FARME_STREAM_BUF_LEN - stream_index;

    for (int i = 0; i < copy_size; i++)
    {
        stream_buff[stream_index + i] = data[i];
    }
    stream_index += copy_size;

    while (stream_index - find_index >= FARME_NO_DATA_SIZE)
    {
        uint32_t stream_remain = stream_index - find_index; /* 剩余未遍历大小 */
        if (stream_buff[find_index] == FARME_HEAD)
        {
            /* 找到帧头 */
            uint8_t data_len = stream_buff[find_index + 3];
            if (data_len <= stream_remain - FARME_NO_DATA_SIZE)
            {
                /* 长度合法 */
                if (stream_buff[find_index + data_len + FARME_NO_DATA_SIZE - 1] == FARME_TAIL)
                {
                    /* 帧尾合法 */
                    ret = farme_decode(&stream_buff[find_index], data_len + FARME_NO_DATA_SIZE, &temp_farme);
                    if (ret == FERR_OK)
                    {
                        /* 解码成功 */

                        cb(&temp_farme);
                        /* stream中清除已解码成功的 */
                        for (int i = 0; i < stream_index - find_index - data_len - FARME_NO_DATA_SIZE; i++)
                        {
                            stream_buff[i] = stream_buff[find_index + data_len + FARME_NO_DATA_SIZE + i];
                        }
                        find_index = 0;
                        stream_index = stream_index - find_index - data_len - FARME_NO_DATA_SIZE;
                        continue;
                    }
                }
            }
        }
        find_index++;
    }

    return FERR_OK;
}

/**
 * @brief 帧数据加密
 */
farme_err_t farme_data_encrypt(uint8_t* data, uint32_t size)
{
    if (data == NULL || size > FARME_MAX_LEN)
    {
        return FERR_PARA;
    }
    int ret = encrypt_table_replace(&encrypt_table, data, size);
    if (ret)
    {
        return FERR_ENCRYPT;
    }
    return FERR_OK;
}

/**
 * @brief 帧数据解密
 */
farme_err_t farme_data_decrypt(uint8_t* data, uint32_t size)
{
    if (data == NULL || size > FARME_MAX_LEN)
    {
        return FERR_PARA;
    }
    int ret = decrypt_table_replace(&encrypt_table, data, size);
    if (ret)
    {
        return FERR_ENCRYPT;
    }
    return FERR_OK;
}

/**
 * @brief 错误码转换为错误说明
 */
const char* farme_err_code(farme_err_t err)
{
    switch (err) {
    case FERR_OK:
        return "No error";
        break;
    case FERR_PARA:
        return "Parameter error";
        break;
    case FERR_LEN:
        return "Len error";
        break;
    case FERR_MEM:
        return "Memory error";
        break;
    case FERR_FORMAT:
        return "Format error";
        break;
    case FERR_CRC:
        return "CRC error";
        break;
    case FERR_ENCRYPT:
        return "Encrypt error";
        break;
    default:
        return "Unknow error";
        break;
    }
}


/**
 * @brief 关键帧
 */
static uint8_t is_mainly_frame(uint8_t data)
{
//    if(data == FARME_CMD_REQ_FIRMWARE_VERSION || \
//       data == FARME_CMD_RESP_FIRMWARE_VERSION || \
//       data == FARME_CMD_REQ_IAP_RESET || \
//       data == FARME_CMD_RESP_IAP_RESET){
//        return 0;
//       }

    return 1;
}
