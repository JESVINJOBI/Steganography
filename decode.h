#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include "common.h"
#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    long size_secret_file;

} DecodeInfo;

/* Check operation type */
OperationType check_operation_type(char opt);

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo);

/* Decode hidden information */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);
Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo);
Status decode_secret_file_extn(int size, DecodeInfo *decInfo);
Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Low-level decoding functions */
Status decode_data_from_image(char *data, int size,FILE *fptr_stego_image);
Status decode_byte_from_lsb(char *data, char *image_buffer);


#endif