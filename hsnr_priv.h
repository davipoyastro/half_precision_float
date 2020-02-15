// Copyright (c) 2020 Davi Poyastro
// Licensed under the MIT License.

#ifndef _HSNR_PRIV_H_
#define _HSNR_PRIV_H_

#include <stdint.h>
#include <stddef.h>
#include "hsnr.h"

/* ------------------------------ DEFINITIONS ------------------------------- */

/**
 * @brief Bit field structure used to encode the High-SNR format.
 */
typedef union
{
    unsigned int uint16:16;
    struct // Unsigned
    {
        unsigned int mantissa_uint:11;
        unsigned int signal_uint:1;
        unsigned int exponent_uint:4;
    } __attribute__((__packed__));
    struct // Signed
    {
        int mantissa_int:11;
        int signal_int:1;
        int exponent_int:4;
    } __attribute__((__packed__));
    // Verbose aliases
    struct // Unsigned
    {
        unsigned int mantissa_uint11:11;
        unsigned int signal_uint1:1;
        unsigned int exponent_uint4:4;
    } __attribute__((__packed__));
    struct // Signed
    {
        int mantissa_int11:11;
        int signal_int1:1;
        int exponent_int4:4;
    } __attribute__((__packed__));
} __attribute__((__packed__)) hsnr_struc_t;

/**
 * @brief Bit field structure used for type conversion.
 */
typedef union
{
    unsigned int uint16:16;
    struct // Unsigned
    {
        unsigned int low_uint11:11;
        unsigned int high_uint2:2;
        unsigned int __padding1:3; /* Dummy, Don't use */
    } __attribute__((__packed__));
    struct // Signed
    {
        int int13:13;
        int __padding2:3; /* Dummy, Don't use */
    } __attribute__((__packed__));
} __attribute__((__packed__)) int13_t;

/**
 * @brief Bit field structure used for type conversion.
 */
typedef struct
{
    int int12:12;
    int __padding:4; /* Dummy, Don't use */
} __attribute__((__packed__)) int12_t;

/**
 * @brief Bit field structure representing a single precision float.
 */
typedef union
{
    uint32_t uint32;
    float    float32;
    struct
    {
        unsigned int mantissa_uint:23;
        unsigned int exponent_uint:8;
        unsigned int signal_uint:1;
    } __attribute__((__packed__));
    struct
    {
        int mantissa_int:23;
        int exponent_int:8;
        int signal_int:1;
    } __attribute__((__packed__));
    // Verbose aliases
    struct
    {
        unsigned int mantissa_uint23:23;
        unsigned int exponent_uint8:8;
        unsigned int signal_uint1:1;
    } __attribute__((__packed__));
    struct
    {
        int mantissa_int23:23;
        int exponent_int8:8;
        int signal_int1:1;
    } __attribute__((__packed__));
    struct
    {
        unsigned int mantissa_low_uint12:12;
        unsigned int mantissa_high_uint11:11;
        unsigned int exponent_low_uint4:4;
        unsigned int exponent_high_uint4:4;
        unsigned int signal_low_uint1:1;
    } __attribute__((__packed__));
} __attribute__((__packed__)) ieee754_float_t;

#endif // _HSNR_PRIV_H_
