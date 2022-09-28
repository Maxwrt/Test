//
// @brief  NFC模组通信加解密
// Created by Ryan on 2022/01/27.
//

#ifndef __ENNCYPT_H
#define __ENNCYPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 单表替换加密表结构 */
typedef struct
{
    uint8_t plaintext[16];  /* 明文 */
    uint8_t ciphertext[16]; /* 密文 */
} encrypt_table_t;

/**
 * @brief 加密，data字段将被加密后的内容覆盖
 *
 * @param table     明文密文替换表
 * @param data      数据
 * @param size      大小
 */
int encrypt_table_replace(encrypt_table_t *table, uint8_t* data, uint32_t size);

/**
 * @brief 解密，data字段将被解密后的内容覆盖
 *
 * @param table     明文密文替换表
 * @param data      数据
 * @param size      大小
 */
int decrypt_table_replace(encrypt_table_t *table, uint8_t* data, uint32_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __ENNCYPT_H */
