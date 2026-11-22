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
            display_result(apc_info,'*');
            break;

        case 'x':
            multiplication(apc_info);
            display_result(apc_info,'x');
            break;

        case '/':
            division(apc_info);
            display_result(apc_info,'/');
            break;

        case '%':
            reminder(apc_info);
            display_result(apc_info,'%');
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
    int carry=0,sign=1,data,flag=0;
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
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
        flag=1;
        subtraction(apc_info);
        insert_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail),'-');
        
    }
    else if((apc_info->OPRD1_head)->data!='-' && (apc_info->OPRD2_head)->data=='-')
    {
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
        subtraction(apc_info);
        insert_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail),'-');
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
    if(flag==1)
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
    return SUCCESS;
}

Status subtraction(APC_Info *apc_info)
{

    DLL *oprd1_temp,*oprd2_temp;
    int data,borrow=0,sign=1,flag=0;
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
            int cmp =compare(apc_info);
            if(cmp == 0)
            {
                insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),0);
                return SUCCESS;
            }
            if((apc_info->len_oprd2>apc_info->len_oprd1) || cmp<0)
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

                flag=1;
                sign=-1;
                if(((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-') )
                    sign =1;
            }
            else
            {
                if(((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-') )
                    sign =-1;
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
                    data=data+10;
                    data=data-oprd2_temp->data;
                    borrow=1;
                }
                else
                    data=data - oprd2_temp->data;
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
                oprd1_temp=apc_info->RESULT_head;
                if(oprd1_temp->data==0)            
                    delete_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail));
                else
                    break;
                
            }
            if(flag==1)
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
            if(sign==-1)
                insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        }
    }
    return SUCCESS;
}

Status multiplication(APC_Info *apc_info)
{
    int data,carry=0,count=0,sign=1;
    APC_Info temp_mull={0};
    if((apc_info->OPRD1_head)->data!='-' && (apc_info->OPRD2_head)->data=='-')
        sign=-1;
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data!='-')
        sign=-1;

    DLL *temp_oprd2=apc_info->OPRD2_tail;
    while(temp_oprd2)
    {
        if(temp_oprd2->data=='-')
            break;
        count++;
        DLL *temp_oprd1=apc_info->OPRD1_tail;
        int i=count-1;
        while(i)
        {
            insert_first(&(temp_mull.OPRD2_head),&(temp_mull.OPRD2_tail),0);
            i--;
        }
        while(temp_oprd1)
        {
            if(temp_oprd1->data=='-')
                break;
            data=temp_oprd2->data*temp_oprd1->data+carry;
            if(carry!=0)
                carry=0;
            if(data>9)
            {
                carry=data/10;
                data %= 10;
            }
            if(count==1)
                insert_first(&(temp_mull.OPRD1_head),&(temp_mull.OPRD1_tail),data);
            else
                insert_first(&(temp_mull.OPRD2_head),&(temp_mull.OPRD2_tail),data);
            temp_oprd1=temp_oprd1->prev;
        }
        if(carry!=0)
        {
            if(count==1)
                insert_first(&(temp_mull.OPRD1_head),&(temp_mull.OPRD1_tail),carry);
            else
                insert_first(&(temp_mull.OPRD2_head),&(temp_mull.OPRD2_tail),carry);
            carry=0;
        }
        if(count==1)
        {
            temp_oprd2=temp_oprd2->prev;
            continue;
        }

        addition(&temp_mull);
        temp_oprd1=temp_mull.RESULT_head;
        delete_list(&(temp_mull.OPRD1_head),&(temp_mull.OPRD1_tail));
        delete_list(&(temp_mull.OPRD2_head),&(temp_mull.OPRD2_tail));
        temp_mull.OPRD1_head=temp_mull.RESULT_head;
        temp_mull.OPRD1_tail=temp_mull.RESULT_tail;

        temp_mull.RESULT_head=NULL;
        temp_mull.RESULT_tail=NULL;

        temp_oprd2=temp_oprd2->prev;
    }

    apc_info->RESULT_head=temp_mull.OPRD1_head;
    apc_info->RESULT_tail=temp_mull.OPRD1_tail;
    if(carry!=0)
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),carry);
        carry=0;
    }
    if(sign==-1)
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
   
    delete_list(&(temp_mull.OPRD2_head),&(temp_mull.OPRD2_tail));
    delete_list(&(temp_mull.RESULT_head),&(temp_mull.RESULT_tail));

    return SUCCESS;
}

Status division(APC_Info *apc_info)
{
    int sign=1;
    /* taking a backup of real operands */
    APC_Info backup={0};
    DLL *temp=apc_info->OPRD1_head;
    while(temp)
    {
        insert_last(&(backup.OPRD1_head),&(backup.OPRD1_tail),temp->data);
        temp=temp->next;
    }
    backup.len_oprd1=apc_info->len_oprd1;

    temp=apc_info->OPRD2_head;
    while(temp)
    {
        insert_last(&(backup.OPRD2_head),&(backup.OPRD2_tail),temp->data);
        temp=temp->next;
    }
    backup.len_oprd2=apc_info->len_oprd2;

    /* asigning sign for output and deleting sign from input */
    if((apc_info->OPRD1_head)->data!='-' && (apc_info->OPRD2_head)->data=='-')
    {
        sign=-1;
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    }
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data!='-')
    {
        sign=-1;
        delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
    }
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-')
    {
        delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    }

    if((apc_info->OPRD2_head->data==1) && (apc_info->OPRD2_head->next==NULL))
    {
        apc_info->RESULT_head=apc_info->OPRD1_head;
        apc_info->RESULT_tail=apc_info->OPRD1_tail;
        if(sign==-1)
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    if((apc_info->OPRD2_head->data==0) && (apc_info->OPRD2_head->next==NULL))
    {
        display_error("Division by zero detected");
        exit(FAILURE);
    }

     /* comparing oparends */
    int cmp=compare(apc_info);
    
    if(cmp<0)
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),0);
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    if(cmp == 0)
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),1);
        if(sign ==-1)
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    
    APC_Info temp_info={0};
    APC_Info temp_rmd={0};
    //APC_Info backup=*apc_info;

    temp=apc_info->OPRD1_head;
    while(temp)
    {
        insert_last(&(temp_rmd.OPRD1_head),&(temp_rmd.OPRD1_tail),temp->data);
        temp=temp->next;
    }
    temp_rmd.len_oprd1=apc_info->len_oprd1;

    temp=apc_info->OPRD2_head;
    while(temp)
    {
        insert_last(&(temp_info.OPRD1_head),&(temp_info.OPRD1_tail),temp->data);
        temp=temp->next;
    }
    temp_info.len_oprd1=apc_info->len_oprd2;

    delete_list(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
    delete_list(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    insert_last(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail),0);

    int count =0,len,mul=1;

    /* for comparing initialy the operands*/
    temp_rmd.OPRD2_head=temp_info.OPRD1_head;
    temp_rmd.OPRD2_tail=temp_info.OPRD1_tail;
    temp_rmd.len_oprd2=temp_info.len_oprd1;

    while(compare(&temp_rmd)>=0)
    {
        temp_rmd.OPRD2_head=NULL;
        temp_rmd.OPRD2_tail=NULL;
        mul=1;
        if(temp_rmd.OPRD1_head->data < temp_info.OPRD1_head->data)
        {
            mul=(temp_rmd.OPRD1_head)->data*10+((temp_rmd.OPRD1_head)->next)->data;
            mul=mul/temp_info.OPRD1_head->data;
        }
        retry :
        insert_last(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail),mul);
        len=temp_rmd.len_oprd1-temp_info.len_oprd1-1;
        while(len>0)
        {
            insert_last(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail),0);
            len--;
        }
        multiplication(&temp_info);
    
        temp_rmd.OPRD2_head=temp_info.RESULT_head;
        temp_rmd.OPRD2_tail=temp_info.RESULT_tail;
        temp_info.RESULT_head=NULL;
        temp_info.RESULT_tail=NULL;
        
        //print_list(temp_info.OPRD2_head);

        temp=temp_rmd.OPRD2_head;
        len=0;
        while(temp)
        {
            len++;
            temp=temp->next;
        }
        temp_rmd.len_oprd2=len;

        // printf("- count\n");
        subtraction(&temp_rmd);
        /* checking selected mul is making a larger output */
        if(temp_rmd.RESULT_head->data=='-')
        {
            mul=1;
            delete_list(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail));
            delete_list(&(temp_rmd.OPRD2_head),&(temp_rmd.OPRD2_tail));
            delete_list(&(temp_rmd.RESULT_head),&(temp_rmd.RESULT_tail));
            goto retry;
        }
        // print_list(temp_rmd.RESULT_head);
        // printf("- rmd");
        temp=temp_rmd.RESULT_head;
        len=0;
        while(temp)
        {
            len++;
            temp=temp->next;
        }
        temp_rmd.len_oprd1=len;
        // printf(" - %d",len);
        // printf("\n");
        delete_list(&(temp_rmd.OPRD1_head),&(temp_rmd.OPRD1_tail));
        temp_rmd.OPRD1_head=temp_rmd.RESULT_head;
        temp_rmd.OPRD1_tail=temp_rmd.RESULT_tail;
        temp_rmd.RESULT_head=NULL;
        temp_rmd.RESULT_tail=NULL;
        delete_list(&(temp_rmd.OPRD2_head),&(temp_rmd.OPRD2_tail));

        apc_info->OPRD2_head=temp_info.OPRD2_head;
        apc_info->OPRD2_tail=temp_info.OPRD2_tail;
        addition(apc_info);
        delete_list(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
        apc_info->OPRD1_head=apc_info->RESULT_head;
        apc_info->OPRD1_tail=apc_info->RESULT_tail;
        apc_info->RESULT_head=NULL;
        apc_info->RESULT_tail=NULL;
        delete_list(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail));
        apc_info->OPRD2_head=NULL;
        apc_info->OPRD2_tail=NULL;

        temp_rmd.OPRD2_head=temp_info.OPRD1_head;
        temp_rmd.OPRD2_tail=temp_info.OPRD1_tail;
        temp_rmd.len_oprd2=temp_info.len_oprd1;
        
    }
    apc_info->RESULT_head=apc_info->OPRD1_head;
    apc_info->RESULT_tail=apc_info->OPRD1_tail;
    if(sign==-1)
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
    apc_info->OPRD1_head=backup.OPRD1_head;
    apc_info->OPRD1_tail=backup.OPRD1_tail;
    apc_info->OPRD2_head=backup.OPRD2_head;
    apc_info->OPRD2_tail=backup.OPRD2_tail;
    
    return SUCCESS;
}

Status reminder(APC_Info *apc_info)
{
    int sign=1;
    /* taking a backup of real operands */
    APC_Info backup={0};
    DLL *temp=apc_info->OPRD1_head;
    while(temp)
    {
        insert_last(&(backup.OPRD1_head),&(backup.OPRD1_tail),temp->data);
        temp=temp->next;
    }
    backup.len_oprd1=apc_info->len_oprd1;

    temp=apc_info->OPRD2_head;
    while(temp)
    {
        insert_last(&(backup.OPRD2_head),&(backup.OPRD2_tail),temp->data);
        temp=temp->next;
    }
    backup.len_oprd2=apc_info->len_oprd2;

    /* asigning sign for output and deleting sign from input */
    if((apc_info->OPRD1_head)->data!='-' && (apc_info->OPRD2_head)->data=='-')
    {
        sign=-1;
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    }
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data!='-')
    {
        sign=-1;
        delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
    }
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-')
    {
        delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    }

    if((apc_info->OPRD2_head->data==1) && (apc_info->OPRD2_head->next==NULL))
    {
        apc_info->RESULT_head=apc_info->OPRD1_head;
        apc_info->RESULT_tail=apc_info->OPRD1_tail;
        if(sign==-1)
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    if((apc_info->OPRD2_head->data==0) && (apc_info->OPRD2_head->next==NULL))
    {
        display_error("Division by zero detected");
        exit(FAILURE);
    }

     /* comparing oparends */
    int cmp=compare(apc_info);
    
    if(cmp<0)
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),0);
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    if(cmp == 0)
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),1);
        if(sign ==-1)
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    
    APC_Info temp_info={0};
    APC_Info temp_rmd={0};
    //APC_Info backup=*apc_info;

    temp=apc_info->OPRD1_head;
    while(temp)
    {
        insert_last(&(temp_rmd.OPRD1_head),&(temp_rmd.OPRD1_tail),temp->data);
        temp=temp->next;
    }
    temp_rmd.len_oprd1=apc_info->len_oprd1;

    temp=apc_info->OPRD2_head;
    while(temp)
    {
        insert_last(&(temp_info.OPRD1_head),&(temp_info.OPRD1_tail),temp->data);
        temp=temp->next;
    }
    temp_info.len_oprd1=apc_info->len_oprd2;

    delete_list(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
    delete_list(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    insert_last(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail),0);

    int count =0,len,mul=1;

    /* for comparing initialy the operands*/
    temp_rmd.OPRD2_head=temp_info.OPRD1_head;
    temp_rmd.OPRD2_tail=temp_info.OPRD1_tail;
    temp_rmd.len_oprd2=temp_info.len_oprd1;

    while(compare(&temp_rmd)>=0)
    {
        temp_rmd.OPRD2_head=NULL;
        temp_rmd.OPRD2_tail=NULL;
        mul=1;
        if(temp_rmd.OPRD1_head->data < temp_info.OPRD1_head->data)
        {
            mul=(temp_rmd.OPRD1_head)->data*10+((temp_rmd.OPRD1_head)->next)->data;
            mul=mul/temp_info.OPRD1_head->data;
        }
        retry :
        insert_last(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail),mul);
        len=temp_rmd.len_oprd1-temp_info.len_oprd1-1;
        while(len>0)
        {
            insert_last(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail),0);
            len--;
        }
        multiplication(&temp_info);
    
        temp_rmd.OPRD2_head=temp_info.RESULT_head;
        temp_rmd.OPRD2_tail=temp_info.RESULT_tail;
        temp_info.RESULT_head=NULL;
        temp_info.RESULT_tail=NULL;
        
        // print_list(temp_info.OPRD2_head);

        temp=temp_rmd.OPRD2_head;
        len=0;
        while(temp)
        {
            len++;
            temp=temp->next;
        }
        temp_rmd.len_oprd2=len;

        // printf("- count\n");
        subtraction(&temp_rmd);
        if(temp_rmd.RESULT_head->data=='-')
        {
            mul=1;
            delete_list(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail));
            delete_list(&(temp_rmd.OPRD2_head),&(temp_rmd.OPRD2_tail));
            delete_list(&(temp_rmd.RESULT_head),&(temp_rmd.RESULT_tail));
            goto retry;
        }
        // print_list(temp_rmd.RESULT_head);
        // printf("- rmd");
        temp=temp_rmd.RESULT_head;
        len=0;
        while(temp)
        {
            len++;
            temp=temp->next;
        }
        temp_rmd.len_oprd1=len;
        // printf(" - %d",len);
        // printf("\n");
        delete_list(&(temp_rmd.OPRD1_head),&(temp_rmd.OPRD1_tail));
        temp_rmd.OPRD1_head=temp_rmd.RESULT_head;
        temp_rmd.OPRD1_tail=temp_rmd.RESULT_tail;
        temp_rmd.RESULT_head=NULL;
        temp_rmd.RESULT_tail=NULL;
        delete_list(&(temp_rmd.OPRD2_head),&(temp_rmd.OPRD2_tail));

        apc_info->OPRD2_head=temp_info.OPRD2_head;
        apc_info->OPRD2_tail=temp_info.OPRD2_tail;
        addition(apc_info);
        delete_list(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
        apc_info->OPRD1_head=apc_info->RESULT_head;
        apc_info->OPRD1_tail=apc_info->RESULT_tail;
        apc_info->RESULT_head=NULL;
        apc_info->RESULT_tail=NULL;
        delete_list(&(temp_info.OPRD2_head),&(temp_info.OPRD2_tail));
        apc_info->OPRD2_head=NULL;
        apc_info->OPRD2_tail=NULL;

        temp_rmd.OPRD2_head=temp_info.OPRD1_head;
        temp_rmd.OPRD2_tail=temp_info.OPRD1_tail;
        temp_rmd.len_oprd2=temp_info.len_oprd1;
        
    }
    apc_info->RESULT_head=temp_rmd.OPRD1_head;
    apc_info->RESULT_tail=temp_rmd.OPRD1_tail;
    if(sign==-1)
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
    apc_info->OPRD1_head=backup.OPRD1_head;
    apc_info->OPRD1_tail=backup.OPRD1_tail;
    apc_info->OPRD2_head=backup.OPRD2_head;
    apc_info->OPRD2_tail=backup.OPRD2_tail;
    
    return SUCCESS;
}

int compare(APC_Info *apc_info)
{
    if(apc_info->len_oprd1 != apc_info->len_oprd2)
        return apc_info->len_oprd1 - apc_info->len_oprd2;
    DLL *oprd1_temp=apc_info->OPRD1_head,*oprd2_temp=apc_info->OPRD2_head;
    while(oprd1_temp && oprd2_temp)
    {
        if(oprd1_temp->data == '-')
        {
            oprd1_temp=oprd1_temp->next;
            continue;
        }
        if(oprd2_temp->data == '-')
        {
            oprd2_temp=oprd1_temp->next;
            continue;
        }
        if(oprd1_temp->data != oprd2_temp->data)
            return oprd1_temp->data - oprd2_temp->data;
        oprd1_temp=oprd1_temp->next;
        oprd2_temp=oprd2_temp->next;
    }
    return 0;
    
}


