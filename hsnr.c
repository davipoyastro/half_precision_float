// Copyright (c) 2020 Davi Poyastro
// Licensed under the MIT License.

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "hsnr.h"      // Public interface
#include "hsnr_priv.h" // Private defines for implementation and unit-test

#ifndef __ORDER_LITTLE_ENDIAN__
#error "Endianess not supported"
#endif

#if (__SIZEOF_FLOAT__ != 4U)
#error "Float size not supported"
#endif

/* ------------------------------ DEFINITIONS ------------------------------- */

/**
 * @brief Private integer constants.
 */
enum
{
    IEEE754_EXP_BIAS         =  127, /**< Bias for normalized exponent for float */
    IEEE754_MANTISSA_BIT_LEN =  23,  /**< Float mantissa size in bits */
    HSNR_EXP_BIAS            =  25,  /**< Bias for normalized exponent for High-SNR */
    HSNR_EXP_UNNORM          = -24,  /**< Unnormalized exponent for High-SNR */
    HSNR_MANTISSA_BIT_LEN    =  11,  /**< High-SNR mantissa size in bits */
};

/* ---------------------------- PUBLIC FUNCTIONS ---------------------------- */

int float_to_hsnr(float in, uint16_t* out)
{
    /* Internal defines - Not worth exposing */
    enum 
    {
        HSNR_MANT_MAX  = (1U << HSNR_MANTISSA_BIT_LEN),
        EXP_BIAS_LIMIT = (IEEE754_EXP_BIAS - 14U),
        MANT_LEN_DIFF  = IEEE754_MANTISSA_BIT_LEN - HSNR_MANTISSA_BIT_LEN,
    };
    
    hsnr_struc_t hsnr_struc = { 0 };
    ieee754_float_t* f_struct = (ieee754_float_t*)&in;

    if (out == NULL)
    {
        return HSNR_ARG_ERROR;
    }
    
    if ((in > HSNR_MAX) || (in < HSNR_MIN))
    {
        return HSNR_ARG_ERROR;
    }

    hsnr_struc.signal_uint = f_struct->signal_uint;

    // Uses the already computed ieee754 mantissa and exponent to set the HSNR fields
    if (f_struct->exponent_uint8 > EXP_BIAS_LIMIT) // Normal exponent
    {    
        hsnr_struc.exponent_int4 = f_struct->exponent_int8 - EXP_BIAS_LIMIT;
        hsnr_struc.mantissa_uint11 = 
            ((f_struct->mantissa_uint23 - 1U) >> MANT_LEN_DIFF) + 1U; // Shift and round up
        if (f_struct->signal_uint) // Reverse representation if negative
        {
            hsnr_struc.mantissa_uint11 = HSNR_MANT_MAX - hsnr_struc.mantissa_uint11; 
            if (hsnr_struc.mantissa_uint11 == 0) // Corner case
            {
                // TODO: Add explanation
                hsnr_struc.exponent_int4 = 
                    (f_struct->exponent_int8 - IEEE754_EXP_BIAS - 3);
            }
        }
    }
    else // Soft underflow exponent
    {
        hsnr_struc.exponent_uint = 0;
        hsnr_struc.mantissa_uint = in / powf(2, HSNR_EXP_UNNORM);
    }
    
    *out = hsnr_struc.uint16;
       return HSNR_OK;
}

float hsnr_to_float(uint16_t in)
{
    float ret;
    hsnr_struc_t* hsnr_ptr = (hsnr_struc_t*)&in;
    
    if (hsnr_ptr->exponent_uint4) // Normal exponent
    {
        int13_t tmp;
        int exp_without_bias = (int)hsnr_ptr->exponent_uint4 - (int)HSNR_EXP_BIAS;
        tmp.low_uint11 = hsnr_ptr->mantissa_uint11;
        tmp.high_uint2 = (hsnr_ptr->signal_uint ? 0b10 : 0b01);
        ret = ((float)tmp.int13) * powf(2U, exp_without_bias);
    }
    else // Soft underflow exponent
    {
        int tmp = ((int12_t*)&in)->int12;
        ret = ((float)tmp) * powf(2U, HSNR_EXP_UNNORM);
    }
    return ret;
}
