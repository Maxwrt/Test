//
// @brief  NFC模组通信加解密
// Created by Ryan on 2022/01/27.
//

#include "encrypt.h"

/* 检查加密表完整性 */
static int encrypt_table_check(encrypt_table_t* table)
{
    uint16_t check_table;
    for (int i = 0; i < 16; i++)
    {
        check_table |= 1 << table->plaintext[i];
    }
    if (check_table != 0xFFFF)
    {
        return -1;
    }
    for (int i = 0; i < 16; i++)
    {
        check_table |= 1 << table->ciphertext[i];
    }
    if (check_table != 0xFFFF)
    {
        return -1;
    }
    return 0;
}

/* 单字节加密 */
static uint8_t encrypt_table_byte(encrypt_table_t* table, uint8_t ch)
{
    uint8_t temp1, temp2;
    temp1 = (ch & 0xF0) >> 4;
    for (int i = 0; i < 16; i++)
    {
        if (temp1 == table->plaintext[i])
        {
            temp1 = table->ciphertext[i];
            break;
        }
    }
    temp2 = ch & 0x0F;
    for (int i = 0; i < 16; i++)
    {
        if (temp2 == table->plaintext[i])
        {
            temp2 = table->ciphertext[i];
            break;
        }
    }
    return temp1 << 4 | temp2;
}
/* 单字节解密 */
static uint8_t decrypt_table_byte(encrypt_table_t* table, uint8_t ch)
{
    uint8_t temp1, temp2;
    temp1 = (ch & 0xF0) >> 4;
    for (int i = 0; i < 16; i++)
    {
        if (temp1 == table->ciphertext[i])
        {
            temp1 = table->plaintext[i];
            break;
        }
    }
    temp2 = ch & 0x0F;
    for (int i = 0; i < 16; i++)
    {
        if (temp2 == table->ciphertext[i])
        {
            temp2 = table->plaintext[i];
            break;
        }
    }
    return temp1 << 4 | temp2;
}

/**
 * @brief 加密，data字段将被加密后的内容覆盖
 */
int encrypt_table_replace(encrypt_table_t* table, uint8_t* data, uint32_t size)
{
    /* 检查表完整性 */
    if (encrypt_table_check(table))
    {
        return -1;
    }
    /* 加密 */
    for (int i = 0; i < size; i++)
    {
        data[i] = encrypt_table_byte(table, data[i]);
    }
    return 0;
}

/**
 * @brief 解密，data字段将被解密后的内容覆盖
 */
int decrypt_table_replace(encrypt_table_t* table, uint8_t* data, uint32_t size)
{
    /* 检查表完整性 */
    if (encrypt_table_check(table))
    {
        return -1;
    }
    /* 解密 */
    for (int i = 0; i < size; i++)
    {
        data[i] = decrypt_table_byte(table, data[i]);
    }
    return 0;
}
