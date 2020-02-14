// Copyright (c) 2020 Davi Poyastro
// Licensed under the MIT License.

#ifndef _HSNR_H_
#define _HSNR_H_
#include <stdint.h>

/**
 * This module convert numbers from different floating point encoding.
 * 
 * The High-SNR encoding is a custom format meant to fit in 16 bits 
 * and implements a half precision format with soft underflow unnormalized
 * mantissa.
 * 
 * The absolute range is shown bellow but different ranges can be stored 
 * as long a multiplication factor is applied.
 * 
 *  0xf800 {1, 15,    0} -> -4.000000000000000000000000
 *  0xf801 {1, 15,    1} -> -3.999023437500000000000000
 *  0xf802 {1, 15,    2} -> -3.998046875000000000000000
 *  ...
 *  0x0ffd {1,  0, 2045} -> -0.000000178813934326171875
 *  0x0ffe {1,  0, 2046} -> -0.000000119209289550781250
 *  0x0fff {1,  0, 2047} -> -0.000000059604644775390625
 *  0x0000 {0,  0,    0} -> +0.000000000000000000000000
 *  0x0001 {0,  0,    1} -> +0.000000059604644775390625
 *  0x0002 {0,  0,    2} -> +0.000000119209289550781250
 *  ...
 *  0xf7fd {0, 15, 2045} -> +3.997070312500000000000000
 *  0xf7fe {0, 15, 2046} -> +3.998046875000000000000000
 *  0xf7ff {0, 15, 2047} -> +3.999023437500000000000000
/**

/* ------------------------------ DEFINITIONS ------------------------------- */

/** Minimum float value able to be re-encoded as High-SNR */
#define HSNR_MIN ((float)-4)
/** Maximum float value able to be re-encoded as High-SNR */
#define HSNR_MAX ((float)4 - (float)1/(float)1024) // (+3.9990234375)

/**
 * @brief Error codes.
 */
enum
{
    HSNR_OK              =  0, /**< Success */  
    HSNR_UNKNOWN_ERROR   = -1, /**< Generic error */
    HSNR_ARG_ERROR       = -2, /**< Invalid argument */
    HSNR_ARG_RANGE_ERROR = -3  /**< Input out of range */
};

/* ---------------------------- PUBLIC FUNCTIONS ---------------------------- */

/**
 * @brief Convert a single precision float to 16 bit High-SNR packed format.
 * @note  This function will fail if input is out of range therefore the 
 *        return code.
 *
 * @param in    Input floating point value.
 * @param out   Output 16 bit High-SNR packed format.
 * @return      Status in POSIX style: 0 for success or error code.
 */
extern int float_to_hsnr(float in, uint16_t* out);

/**
 * @brief Convert a 16 bit High-SNR packed format to single precision float.
 * @note  This function will never fail since every input has an valid output.
 * 
 * @param in    Input 16 bit High-SNR packed format.
 * @return      Output converted value as single precision float.
 */
extern float hsnr_to_float(uint16_t in);

#endif // _HSNR_H_
