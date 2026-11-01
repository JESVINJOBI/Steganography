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
    int extn_size;
    if(decode_secret_file_extn_size(&extn_size,decInfo)==e_failure)
    {
        printf("ERROR: Cannot decode extension size\n");
        return e_failure;
    }
    printf("Extension size: %d\n",extn_size);

    if (decode_secret_file_extn(extn_size,decInfo)==e_failure)
    {
        printf("ERROR: Cannot decode secret file extension\n");
        return e_failure;
    }

    printf("Secret file extension: %s\n", decInfo->extn_test_file);
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

Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo)
{
    char buffer[sizeof(int)];
    if (decode_data_from_image(buffer,sizeof(int),decInfo->fptr_stego_image) == e_failure)
        return e_failure;
    memcpy(size,buffer,sizeof(int));
    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char image_buffer[8];

    for (int i=0;i<size;i++)
    {
        if (fread(image_buffer,1,8,fptr_stego_image)!=8)
        {
            printf("ERROR: Unable to read image data\n");
            return e_failure;
        }

        if (decode_byte_from_lsb(&data[i],image_buffer)==e_failure)
        {
            printf("ERROR: Unable to decode byte\n");
            return e_failure;
        }
    }

    return e_success;
}

Status decode_secret_file_extn(int size, DecodeInfo *decInfo)
{
    char buffer[MAX_FILE_SUFFIX + 1];
    if (decode_data_from_image(buffer, size, decInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }
    buffer[size] = '\0';
    strcpy(decInfo->extn_test_file, buffer);
    return e_success;
}