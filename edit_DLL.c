#include "apc.h"
#include"edit_DLL.h"

Status save_oprd(APC_Info *apc_info,char *oprd1_str,char *oprd2_str)
{
    /*
    ============================================================================
    Function    : save_oprd
    Description : Validates the two command-line operand strings and stores
                their signs and decimal digits in the corresponding doubly
                linked lists.

    Logic       :
        1. Checks the first character of operand 1.
        2. If operand 1 begins with '-', inserts '-' as the first list node,
        advances the input string pointer, and stores the remaining string
        length in len_oprd1.
        3. If operand 1 begins with '+', advances the input string pointer
        without storing a plus-sign node and stores the remaining digit
        count in len_oprd1.
        4. If no sign is present, stores the complete string length.
        5. Checks that a sign is not the only character. If no digit follows,
        displays an error and exits with FAILURE.
        6. Traverses operand 1 character by character. Each decimal character
        is converted from its character code to its numeric digit and
        appended using insert_last(). Any non-digit character causes an
        error and exit(FAILURE).
        7. Repeats the same parsing and validation process for operand 2.
        8. Prints both stored operands using print_list().
        9. Returns SUCCESS.

    Benefit     : Converts command-line integer strings into the linked-list
                representation used by all arithmetic functions and rejects
                non-integer input before calculation.

    Parameters  :
        apc_info  - Structure receiving the two operand lists and their lengths.
        oprd1_str - Command-line string for operand 1.
        oprd2_str - Command-line string for operand 2.

    Return      : Status
        SUCCESS - Both strings were processed and stored.
        FAILURE - Not returned for invalid characters or a sign without digits;
                those cases call exit(FAILURE).
    ============================================================================
    */
    //operand 2 saving,cheking in DLL

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
    /*
    ============================================================================
    Function    : insert_first
    Description : Dynamically creates a new DLL node and inserts it at the
                beginning of the supplied doubly linked list.

    Logic       :
        1. Allocates memory for a new DLL node using malloc().
        2. If allocation fails, displays an allocation error and exits with
        FAILURE.
        3. Stores the supplied data and initializes the new node's prev and
        next pointers to NULL.
        4. If the list is empty, assigns the new node to both head and tail.
        5. Otherwise, links the new node before the current head, updates the
        old head's prev pointer, and updates head to the new node.
        6. Returns SUCCESS.

    Benefit     : Supports most-significant-digit insertion and insertion of
                carry/result digits at the front of a list.

    Parameters  :
        head - Address of the list's head pointer.
        tail - Address of the list's tail pointer.
        data - Integer digit or sign value to store in the new node.

    Return      : Status
        SUCCESS - Node was allocated and inserted.
        FAILURE - Allocation failure terminates the process with exit().
    ============================================================================
    */
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
    /*
    ============================================================================
    Function    : insert_last
    Description : Dynamically creates a new DLL node and appends it at the end
                of the supplied doubly linked list.

    Logic       :
        1. Allocates memory for a new DLL node using malloc().
        2. If allocation fails, displays an allocation error and exits with
        FAILURE.
        3. Stores the supplied data and initializes prev and next to NULL.
        4. If the list is empty, assigns the new node to both head and tail.
        5. Otherwise, links the current tail to the new node, sets the new
        node's prev pointer to the old tail, and updates tail.
        6. Returns SUCCESS.

    Benefit     : Builds operand lists in input order and appends digits or
                temporary quotient digits to existing lists.

    Parameters  :
        head - Address of the list's head pointer.
        tail - Address of the list's tail pointer.
        data - Integer digit or sign value to store in the new node.

    Return      : Status
        SUCCESS - Node was allocated and appended.
        FAILURE - Allocation failure terminates the process with exit().
    ============================================================================
    */
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
    /*
    ============================================================================
    Function    : delete_first
    Description : Deletes the first node of a doubly linked list and updates
                the head and tail pointers.

    Logic       :
        1. Checks whether the list is empty. If so, returns FAILURE.
        2. If head and tail point to the same node, frees that node, sets both
        pointers to NULL, and returns SUCCESS.
        3. Otherwise, advances head to the next node.
        4. Frees the old first node through the new head's prev pointer.
        5. Sets the new head's prev pointer to NULL.
        6. Returns SUCCESS. The tail remains unchanged for a multi-node list.

    Benefit     : Removes a leading sign node or deletes leading zero/result
                nodes while keeping the list links consistent.

    Parameters  :
        head - Address of the list's head pointer.
        tail - Address of the list's tail pointer.

    Return      : Status
        SUCCESS - First node was deleted.
        FAILURE - The list was empty.
    ============================================================================
    */
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
    /*
    ============================================================================
    Function    : delete_list
    Description : Frees every node in a doubly linked list and clears its
                head and tail pointers.

    Logic       :
        1. If head is NULL, returns FAILURE.
        2. If the list contains one node (head == tail), frees that node,
        sets head and tail to NULL, and returns SUCCESS.
        3. For a multi-node list, advances head to the second node.
        4. Traverses forward. At each step, frees the previous node and moves
        head to the next node.
        5. After the traversal reaches NULL, frees the original tail node and
        sets tail to NULL. Head is already NULL after the traversal.
        6. Returns SUCCESS.

    Benefit     : Releases dynamically allocated operand, intermediate, and
                result nodes when a list is no longer required.

    Parameters  :
        head - Address of the list's head pointer.
        tail - Address of the list's tail pointer.

    Return      : Status
        SUCCESS - All nodes in the list were freed.
        FAILURE - The list was already empty.
    ============================================================================
    */
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
    /*
    ============================================================================
    Function    : print_list
    Description : Prints the contents of a linked list from head to tail.

    Logic       :
        1. Checks whether the supplied head pointer is NULL.
        2. If the list is empty, prints an empty-list information message.
        3. Otherwise, traverses the list using next pointers.
        4. If a node contains '-', prints it as a character.
        5. For every other node, prints the stored integer value as a digit.
        6. Continues until the next pointer becomes NULL.

    Benefit     : Displays operands, intermediate lists when debugging, and
                final arithmetic results in their normal digit order.

    Parameters  :
        head - Head pointer of the list to print.

    Return      : void
        Does not return a value.
    ============================================================================
    */

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

