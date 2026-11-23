#include "apc.h"
#include "edit_DLL.h"
#include "operation.h"

APC_Info apc_info={NULL};

void cleanup(void)
{
    if(apc_info.OPRD1_head==NULL)
        return;
    delete_list(&(apc_info.OPRD1_head),&(apc_info.OPRD1_tail));
    delete_list(&(apc_info.OPRD2_head),&(apc_info.OPRD2_tail));
    delete_list(&(apc_info.RESULT_head),&(apc_info.RESULT_tail));
    printf("\n%-11s : Memory deallocation successfull\n\n","[CLEAN UP]");
}

int main(int argc,char **argv)
{
    atexit(cleanup);
    if(argc!=4)
    {
        display_error("Not Enough arguments given");
        exit(FAILURE);
    }
    if(strlen(argv[2])>1)
    {
        display_error("Unsupported operator");
        exit(FAILURE);
    }
    //APC_Info apc_info={NULL};
    save_oprd(&apc_info,argv[1],argv[3]);
    do_operation(&apc_info,argv[2]);
    
    return (SUCCESS);
}