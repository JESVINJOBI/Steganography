#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    decInfo->stego_image_fname = argv[2];

    int len;
    len = strlen(argv[2]);
    if (len < 4 ||
        strcmp(argv[2] + len - 4, ".bmp") != 0)
    {
        printf("ERROR: Source image must have .bmp extension\n");
        return e_failure;
    }
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        printf("ERROR: Cannot open stego image\n");
        return e_failure;
    }
    printf("Stego_image opened successfully.\n");
    if (argv[3]!=NULL)
    {
        char *dot;

        if (strlen(argv[3])>=sizeof(decInfo->test_fname))
        {
            printf("ERROR: Output filename is too long\n");
            return e_failure;
        }
        strcpy(decInfo->test_fname,argv[3]);
        dot = strrchr(decInfo->test_fname,'.');
        if (dot!=NULL)
        {
            *dot='\0';
        }
    }
    else
    {
        strcpy(decInfo->test_fname,"decoded");
    }
    return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if (fseek(decInfo->fptr_stego_image,54,SEEK_SET) != 0)
    {
        printf("ERROR: Cannot skip BMP header\n");
        return e_failure;
    }    
    
   
    return e_success;
}

