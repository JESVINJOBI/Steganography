#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if (argc < 2)
    {
        printf("ERROR: Insufficient arguments\n");
        return 1;
    }
    OperationType op_type=check_operation_type(argv[1][1]);
    if(op_type==e_encode)
    {
        if(argc<4)
        {
            printf("ERROR: Insufficient arguments\n");
            return 1;
        }

        if(argc>5)
        {
            printf("ERROR: Excess arguments\n");
            return 1;
        }

        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("Validation sucesssful.\n");
            printf("Encoding selected.\n");
            if(do_encoding(&encInfo)==e_success)
            {
                printf("Encoding successful.\n");
            }
            else
            {
                printf("Encoding failed.\n");
                return 1;
            }
        }
        else
        {
            printf("Validation failed.\n");
            return 1;
        }    
    }

    else if(op_type==e_decode)
    {
        if(argc<3)
        {
            printf("ERROR: Insufficient arguments\n");
            return 1;
        }

        if(argc>4)
        {
            printf("ERROR: Excess arguments\n");
            return 1;
        }

        if(read_and_validate_decode_args(argv,&decInfo)==e_success)
        {
            printf("Validation sucesssful.\n");
            printf("Decoding selected.\n");
           /* if(do_decoding(&decInfo)==e_success)
            {
                printf("Decoding successful.\n");
            }
            else
            {
                printf("Decoding failed.\n");
                return 1;
            }*/
        }
        else
        {
            printf("Validation failed.\n");
            return 1;
        }   
    }
    else
    {
        printf("Unsupported operation\n");
        return 1;
    }
    return 0;
}

OperationType check_operation_type(char opt)
{
    if(opt=='e')
        return e_encode;
    else if(opt=='d')
        return e_decode;  
    else
        return e_unsupported; 

}
