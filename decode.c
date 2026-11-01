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
    char image_buffer[8];
    char data[20];
    int size = strlen(MAGIC_STRING);
    if (fseek(decInfo->fptr_stego_image,54,SEEK_SET) != 0)
    {
        printf("ERROR: Cannot skip BMP header\n");
        return e_failure;
    }    
    for (int i=0;i<size;i++)
    {
        if (fread(image_buffer,1,8,decInfo->fptr_stego_image)!=8)
        {
            printf("ERROR: Cannot read image bytes\n");
            return e_failure;
        }
        if (decode_byte_from_lsb(&data[i],image_buffer)==e_failure)
        {
            printf("ERROR: Cannot decode byte from LSB\n");
            return e_failure;
        }
        
    }
    data[size]='\0';
    printf("Decoded character: %s\n", data);
    if (strcmp(data, MAGIC_STRING) != 0)
    {
        printf("ERROR: Magic string does not match\n");
        return e_failure;
    }

    printf("Magic string verified successfully.\n");
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    unsigned char ch = 0;
    for (int i=0;i<8;i++)
    {
        unsigned char bit=image_buffer[i]&1;
        ch=ch<<1;
        ch=ch|bit;
    }
    *data = ch;
    return e_success;
}