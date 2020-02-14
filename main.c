// Copyright (c) 2020 Davi Poyastro
// Licensed under the MIT License.

#include <stdio.h>
#include "hsnr.h"

int main(int argc, char* argv[])
{
    float f;
    unsigned int u;

    if (argc == 1)
    {
        printf("Usage:\n  %s <float|hex>\n", argv[0]);
        return 0;
    }

    if (sscanf(argv[1], "0x%x", &u) == 1)
    {
        if (u > __UINT16_MAX__)
        {
            printf("ERROR: Value out of range\n");
            return 2;
        }
        f = hsnr_to_float((uint16_t)u);
        printf(">>> 0x%04x -> %+.24f\n", u, f);
        return 0;
    }

    if (sscanf(argv[1], "%f", &f) == 1)
    {
        uint16_t x;
        if (float_to_hsnr(f, &x) != HSNR_OK)
        {
            printf("ERROR: Value out of range\n");
            return 2;
        }
        printf(">>> 0x%04x <- %+.24f\n", x, f);
        return 0;
    }

    printf("ERROR: Input argument isn't float or hexadecimal\n");
    return 2;
}
