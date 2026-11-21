#ifndef OPERATION_H
#define OPERATION_H

#include"apc.h"

Status do_operation(APC_Info *apc_info,char *op);
Status addition(APC_Info *apc_info);
Status subtraction(APC_Info *apc_info);
Status multiplication(APC_Info *apc_info);
Status division(APC_Info *apc_info);
Status reminder(APC_Info *apc_info);
int compare(APC_Info *apc_info);

#endif
