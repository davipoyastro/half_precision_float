// Copyright (c) 2020 Davi Poyastro
// Licensed under the MIT License.

#include <stdio.h>
#include <string.h>
#include "hsnr.h"
#include "hsnr_priv.h"

int main(int argc, char* argv[])
{
    int rc = 0;
    int verbose = 0;
    uint16_t hsnr1  = 0; // Encoded value
    uint16_t hsnr2  = 0; // Re-encoded value
    float    float1 = 0; // Intermediary float
    float    float2 = 0; // Re-encoded float

    hsnr_struc_t* hsnr_ptr1  = (hsnr_struc_t*)&hsnr1;
    hsnr_struc_t* hsnr_ptr2  = (hsnr_struc_t*)&hsnr2;
    ieee754_float_t* fstruct = (ieee754_float_t*)&float1;

    if ((argc > 1) && (strcmp(argv[1], "-v") == 0))
    {
        verbose = 1;
    }

    for (uint32_t i = 0 ; i <= __UINT16_MAX__ ; ++i)
    {
        hsnr1 = i;
        float1 = hsnr_to_float(hsnr1);
       
        if (float_to_hsnr(float1, &hsnr2) != HSNR_OK)
        {
            printf("ERROR: Value %f cannot be encoded as HSNR\n", float1);
            rc = 2;
            continue;
        }
        
        float2 = hsnr_to_float(hsnr2);
        
        if ((verbose) || (hsnr1 != hsnr2))
        {
            printf("%s: 0x%04x {%u, %u, %u} -> %+.24f {%u, %u, %u } -> 0x%04x {%u, %u, %u} -> %+.24f\n", 
                ((hsnr1 == hsnr2) ? "PASS" : "FAIL"),
                hsnr1,
                hsnr_ptr1->signal_uint, hsnr_ptr1->exponent_uint, hsnr_ptr1->mantissa_uint, 
                float1,
                fstruct->signal_uint, fstruct->exponent_uint, fstruct->mantissa_uint, 
                hsnr2, 
                hsnr_ptr2->signal_uint, hsnr_ptr2->exponent_uint, hsnr_ptr2->mantissa_uint,
                float2);
            rc = 2;
        }
    }

    if (rc == 0)
    {
        printf("*** All tests passed ***\n");
    }
    else
    {
        printf("*** Some test failed ***\n");
    }
        
    return rc;
}
