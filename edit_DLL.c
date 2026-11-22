#include "apc.h"
#include"edit_DLL.h"

Status save_oprd(APC_Info *apc_info,char *oprd1_str,char *oprd2_str)
{
    //operand 2 saving,cheking in DLL
    //printf("%s - %s \n",oprd1_str,oprd2_str);

    if(*oprd1_str=='-')
    {
        insert_last(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail),(*oprd1_str));
        oprd1_str++;
        apc_info->len_oprd1=strlen(oprd1_str);
        if(*oprd1_str=='\0')
        {
            display_error("In Operand 1 no integer found");
            exit(FAILURE);
        }
    }
    else if(*oprd1_str=='+')
    {
        oprd1_str++;
        apc_info->len_oprd1=strlen(oprd1_str);
        if(*oprd1_str=='\0')
        {
            display_error("In Operand 1 no integer found");
            exit(FAILURE);
        }
    }
    else
        apc_info->len_oprd1=strlen(oprd1_str);
    while(*oprd1_str)
    {
        if(*oprd1_str>='0' && *oprd1_str<='9')
            insert_last(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail),(*oprd1_str-'0'));
        else
        {
            display_error("Operand 1 is not an integer");
            exit(FAILURE);
        }
        oprd1_str++;

    }

    //operand 2 saving,validation in DLL
    if(*oprd2_str=='-')
    {
        insert_last(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail),(*oprd2_str));
        oprd2_str++;
        apc_info->len_oprd2=strlen(oprd2_str);
        if(*oprd2_str=='\0')
        {
            display_error("In Operand 2 no integer found");
            exit(FAILURE);
        }
    }
    else if(*oprd2_str=='+')
    {
        oprd2_str++;
        apc_info->len_oprd2=strlen(oprd2_str);
        if(*oprd2_str=='\0')
        {
            display_error("In Operand 1 no integer found");
            exit(FAILURE);
        }
    }
    else
        apc_info->len_oprd2=strlen(oprd2_str);
    while(*oprd2_str)
    {
        if(*oprd2_str>='0' && *oprd2_str<='9')
            insert_last(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail),(*oprd2_str-'0'));
        else
        {
            display_error("Operand 2 is not an integer");
            exit(FAILURE);
        }
        oprd2_str++;
    }
    printf("%-11s : ","[OPERAND 1]");
    print_list(apc_info->OPRD1_head);
    printf("\n");
    printf("%-11s : ","[OPERAND 2]");
    print_list(apc_info->OPRD2_head);
    printf("\n");
    return SUCCESS;
}

Status insert_first(DLL **head, DLL **tail, int data)
{
    DLL *new=malloc(sizeof(DLL));
    if(new==NULL)
    {
        display_error("Dynamic memmory allocation failed");
        exit(FAILURE);
    }
    new->data=data;
    new->prev=NULL;
    new->next=NULL;
    if(*head==NULL)
    {
        *head=new;
        *tail=new;
        return SUCCESS;
    }
    new->next=(*head);
    (*head)->prev=new;
    *head=new;
    return SUCCESS;
}

Status insert_last(DLL **head, DLL **tail, int data)
{
    DLL *new=malloc(sizeof(DLL));
    if(new==NULL)
    {
        display_error("Dynamic memmory allocation failed");
        exit(FAILURE);
    }
    new->data=data;
    new->prev=NULL;
    new->next=NULL;
    if(*head==NULL)
    {
        *head=new;
        *tail=new;
        return SUCCESS;
    }
    (*tail)->next=new;
    new->prev=(*tail);
    *tail=new;
    return SUCCESS;

}

Status delete_first(DLL **head, DLL **tail)
{
    if(*head==NULL)
        return FAILURE;
    if(*tail==*head)
    {
        free(*head);
        *head=NULL;
        *tail=NULL;
        return SUCCESS;
    }
    *head=(*head)->next;
    free((*head)->prev);
    (*head)->prev=NULL;
    return SUCCESS;

}

Status delete_list(DLL **head, DLL **tail)
{
    if(*head==NULL)
        return FAILURE;
    if(*tail==*head)
    {
        free(*head);
        *head=NULL;
        *tail=NULL;
        return SUCCESS;
    }
    *head=(*head)->next;
    while(*head!=NULL)
    {
        free((*head)->prev);
        *head=(*head)->next;
    }
    free(*tail);
    *tail=NULL;
    return SUCCESS;
}

void print_list(DLL *head)
{
	/* Cheking the list is empty or not */
	if (head == NULL)
	{
		printf("INFO : List is empty\n");
	}
	else
	{
	    while (head)		
	    {
            if(head->data=='-')
            {
                printf("%c", head -> data);
                head = head -> next;
                continue;
            }
		    /* Printing the list */
		    printf("%d", head -> data);

		    /* Travering in forward direction */
		    head = head -> next;
		    
	    }
    }
}

void display_error(char *error)
{
    printf("\n%-11s : %s !!\n","[ERROR]",error);
    printf("\n************** Programm Terminated **************\n\n");
}

void display_result(APC_Info *apc_info,char ch)
{
    printf("\n%-11s : ","[OUTPUT]");
    print_list(apc_info->OPRD1_head);
    printf(" %c ",ch);
    print_list(apc_info->OPRD2_head);
    printf(" = ");
    print_list(apc_info->RESULT_head);
    printf("\n");
}