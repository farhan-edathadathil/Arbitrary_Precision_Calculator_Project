#include "apc.h"
#include"edit_DLL.h"
#include "operation.h"

Status do_operation(APC_Info *apc_info,char *op)
{
    switch(op[0])
    {
        case '+':
            addition(apc_info);
            break;

        case '-':
            subtraction(apc_info);
            break;
        
        case '*':
            multiplication(apc_info);
            break;

        case '/':
            division(apc_info);
            break;

        case '%':
            reminder(apc_info);
            break;

        default :
            display_error("Unsupported operator");
            exit(FAILURE);

    }
    return SUCCESS;  
}

Status addition(APC_Info *apc_info)
{
    printf("Addition choosed\n");
    int carry;
    

    return SUCCESS;
}

Status subtraction(APC_Info *apc_info)
{
    return SUCCESS;
}

Status multiplication(APC_Info *apc_info)
{
    return SUCCESS;
}

Status division(APC_Info *apc_info)
{
    return SUCCESS;
}

Status reminder(APC_Info *apc_info)
{
    return SUCCESS;
}




