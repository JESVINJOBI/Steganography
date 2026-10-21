#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    if(argc<4)
    {
        printf("ERROR: Insufficient arguments\n");
        return 1;
    }
    OperationType op_type=check_operation_type(argv[1][1]);
    if(op_type==e_encode)
    {
        printf("Encoding selected\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("Validation sucesssful");
            //do_encoding(&encInfo);
        }
        else
        {
            printf("Encoding failed.");
            return 1;
        }    
    }
    else if(op_type==e_decode)
    {
        printf("Decoding selected\n");
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
