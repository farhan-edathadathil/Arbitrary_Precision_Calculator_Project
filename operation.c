#include "apc.h"
#include"edit_DLL.h"
#include "operation.h"

Status do_operation(APC_Info *apc_info,char *op)
{
    switch(op[0])
    {
        case '+':
            addition(apc_info);
            display_result(apc_info,'+');
            break;

        case '-':
            subtraction(apc_info);
            display_result(apc_info,'-');
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
    //printf("Addition choosed\n");
    int carry=0,sign=1,data;
    DLL *oprd1_temp,*oprd2_temp;
    if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data!='-')
    {
        oprd1_temp=apc_info->OPRD1_head;
        apc_info->OPRD1_head=apc_info->OPRD2_head;
        apc_info->OPRD2_head=oprd1_temp;

        oprd1_temp=apc_info->OPRD1_tail;
        apc_info->OPRD1_tail=apc_info->OPRD2_tail;
        apc_info->OPRD2_tail=oprd1_temp;

        data=apc_info->len_oprd1;
        apc_info->len_oprd1=apc_info->len_oprd2;
        apc_info->len_oprd2=data;

        subtraction(apc_info);
        
    }
    else if((apc_info->OPRD1_head)->data!='-' && (apc_info->OPRD2_head)->data=='-')
    {
        subtraction(apc_info);
    }
    else 
    {
        if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-')
        {
            sign=-1;
        }
        oprd1_temp=apc_info->OPRD1_tail;
        oprd2_temp=apc_info->OPRD2_tail;

        while(oprd1_temp && oprd2_temp)
        {
            if(oprd1_temp->data=='-')
            {
                oprd1_temp=NULL;
                break;
            }
            if(oprd2_temp->data=='-')
            {
                oprd2_temp=NULL;
                break;
            }
            data=oprd1_temp->data + oprd2_temp->data + carry;
            if(carry==1)
                carry=0;
            if(data>9)
            {
                carry=1;
                data %= 10;
            }
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),data);
            oprd1_temp = oprd1_temp->prev;
            oprd2_temp = oprd2_temp->prev;
        }
        while(oprd1_temp)
        {
            if(oprd1_temp->data=='-')
                break;
            
            data=oprd1_temp->data + carry;
            if(carry==1)
                carry=0;
            if(data>9)
            {
                carry=1;
                data %= 10;
            }
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),data);
            oprd1_temp = oprd1_temp->prev;
        }
        while(oprd2_temp)
        {
            if(oprd2_temp->data=='-')
                break;
            
            data=oprd2_temp->data + carry;
            if(carry==1)
                carry=0;
            if(data>9)
            {
                carry=1;
                data %= 10;
            }
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),data);
            oprd2_temp = oprd2_temp->prev;
        }
        if(carry==1)
        {
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),carry);
            carry=0;
        }
        if(sign==-1)
        {
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
    
        }
    }

    return SUCCESS;
}

Status subtraction(APC_Info *apc_info)
{

    DLL *oprd1_temp,*oprd2_temp;
    int data,borrow=0,sign=1;
    if((apc_info->OPRD1_head)->data!='-' && (apc_info->OPRD2_head)->data=='-')
    {
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
        addition(apc_info);
        insert_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail),'-');
    }
    else 
    {
        if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data!='-')
        {
            delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
            addition(apc_info);
            insert_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail),'-');
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_head),'-');
        }
        else 
        {
            oprd1_temp=apc_info->OPRD1_head;
            oprd2_temp=apc_info->OPRD2_head;
            if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-' )
            {
                if((apc_info->len_oprd2>apc_info->len_oprd1) || ((apc_info->len_oprd2==apc_info->len_oprd1) && (oprd1_temp->next->data < oprd2_temp->next->data)))
                {
                    oprd1_temp=apc_info->OPRD1_head;
                    apc_info->OPRD1_head=apc_info->OPRD2_head;
                    apc_info->OPRD2_head=oprd1_temp;

                    oprd1_temp=apc_info->OPRD1_tail;
                    apc_info->OPRD1_tail=apc_info->OPRD2_tail;
                    apc_info->OPRD2_tail=oprd1_temp;

                    data=apc_info->len_oprd1;
                    apc_info->len_oprd1=apc_info->len_oprd2;
                    apc_info->len_oprd2=data;
                }
                else
                    sign=-1;
            }
            else
            {
                if((apc_info->len_oprd2>apc_info->len_oprd1) || ((apc_info->len_oprd2==apc_info->len_oprd1) && (oprd1_temp->next->data < oprd2_temp->next->data)))
                {
                    oprd1_temp=apc_info->OPRD1_head;
                    apc_info->OPRD1_head=apc_info->OPRD2_head;
                    apc_info->OPRD2_head=oprd1_temp;

                    oprd1_temp=apc_info->OPRD1_tail;
                    apc_info->OPRD1_tail=apc_info->OPRD2_tail;
                    apc_info->OPRD2_tail=oprd1_temp;

                    data=apc_info->len_oprd1;
                    apc_info->len_oprd1=apc_info->len_oprd2;
                    apc_info->len_oprd2=data;

                    sign =-1;
                }
            }

            oprd1_temp=apc_info->OPRD1_tail;
            oprd2_temp=apc_info->OPRD2_tail;
            while(oprd1_temp && oprd2_temp)
            {
                if(oprd1_temp->data=='-')
                {
                    oprd1_temp=NULL;
                    break;
                }
                if(oprd2_temp->data=='-')
                {
                    oprd2_temp=NULL;
                    break;
                }
                data=oprd1_temp->data - borrow;
                if(borrow==1)
                    borrow=0;
                if(data<oprd2_temp->data)
                {
                    data=oprd1_temp->data+10;
                    data=data-oprd2_temp->data;
                    borrow=1;
                }
                else
                    data=oprd1_temp->data - oprd2_temp->data;
                if(data==-1)
                {
                    borrow=1;
                    data=9;
                }
                insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),data);
                oprd1_temp = oprd1_temp->prev;
                oprd2_temp = oprd2_temp->prev;
            }
            while(oprd1_temp)
            {
                if(oprd1_temp->data=='-')
                    break;
                
                data=oprd1_temp->data - borrow;
                if(borrow==1)
                    borrow=0;
                if(data==-1)
                {
                    borrow=1;
                    data=9;
                }
                insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),data);
                oprd1_temp = oprd1_temp->prev;
            }
            while(oprd2_temp)
            {
                if(oprd2_temp->data=='-')
                    break;
                
                data=oprd2_temp->data - borrow;
                if(borrow==1)
                    borrow=0;
                if(data==-1)
                {
                    borrow=1;
                    data=9;
                }
                insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),data);
                oprd2_temp = oprd2_temp->prev;
            }
            oprd1_temp=apc_info->RESULT_head;
            while(oprd1_temp)
            {
                if(oprd1_temp->data==0)
                    delete_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail));
                else
                    break;
            }
            if(sign==-1)
                insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        }

    }
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




