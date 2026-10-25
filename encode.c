#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[],
                                     EncodeInfo *encInfo)
{
    encInfo->src_image_fname = argv[2];
    encInfo->secret_fname = argv[3];

    int len;
    len = strlen(argv[2]);
    if (len < 4 ||
        strcmp(argv[2] + len - 4, ".bmp") != 0)
    {
        printf("ERROR: Source image must have .bmp extension\n");
        return e_failure;
    }
    

    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");

    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open source file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");

    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open text file %s\n", encInfo->secret_fname);

    	return e_failure;
    }


    if (argv[4] != NULL)
    {
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "steged_img.bmp";
    }
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open output file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(check_capacity(encInfo)==e_failure)
    {
        printf("ERROR: Insufficient image capacity.\n");
        return e_failure;
    }
    else
        printf("Image capacity is sufficient.\n");

    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf("ERROR: BMP header coping is failed.\n");
        return e_failure;
    }
    else
        printf("BMP header copied successfully.\n");

    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
    {
        return e_failure;
    }
return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    long capacity=((encInfo -> size_secret_file)+14)*8;
    //14->secret_file_extension(4)+image_extension(4)+extension_size(4)+magic_string(2)

    if(get_image_size_for_bmp(encInfo->fptr_src_image)>=capacity)
        return e_success;

    return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];
    rewind(fptr_src_image);
    if(fread(header,1,54,fptr_src_image)!=54)
    {
        printf("ERROR : ERROR: Cannot read header from source image.\n");
        return e_failure;
    }
    if(fwrite(header,1,54,fptr_dest_image)!=54)
    {
        printf("ERROR : Cannot write header to output image.\n");
        return e_failure;
    }
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len=strlen(magic_string);
    if(encode_data_to_image((char *)magic_string, len,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: Magic string encoding failed.\n");
        return e_failure;
    }
    printf("Magic string encoded successfully.\n");
    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for(int i=0;i<size;i++)
    {
        if(fread(buffer,1,8,fptr_src_image)!=8)
        {
            printf("ERROR: Cannot read 8 bytes from source image.\n");
            return e_failure;
        }
        if(encode_byte_to_lsb(data[i],buffer)==e_failure)
        {
            printf("ERROR: Byte encoding failed.\n");
            return e_failure;
        }
        if(fwrite(buffer,1,8,fptr_stego_image)!=8)
        {
            printf("ERROR: Cannot write image data.\n");
            return e_failure;
        }

    }
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    unsigned char byte = (unsigned char)data;

    for (int i = 0; i < 8; i++)
    {
        unsigned char image_byte = (unsigned char)image_buffer[i];
        unsigned char bit = (byte >> (7 - i)) & 1;

        image_byte = (image_byte & 0xFE) | bit;

        image_buffer[i] = (char)image_byte;
    }

    return e_success;
}