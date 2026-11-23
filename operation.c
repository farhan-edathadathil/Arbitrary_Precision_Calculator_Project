#include "apc.h"
#include"edit_DLL.h"
#include "operation.h"

Status do_operation(APC_Info *apc_info,char *op)
{
    /*
    ============================================================================
    Function    : do_operation
    Description : Selects and executes the arithmetic routine corresponding
                to the command-line operator, then displays the result.

    Logic       :
        1. Examines the first character of the operator string.
        2. For '+', calls addition() and displays the result with '+'
        using display_result().
        3. For '-', calls subtraction() and displays the result with '-'.
        4. For '*' or 'x', calls multiplication() and displays the result
        using the operator character supplied by the user.
        5. For '/', calls division() and displays the result with '/'.
        6. For '%', calls reminder() and displays the result with '%'.
        7. For any other character, displays an unsupported-operator error
        and terminates with exit(FAILURE).
        8. Returns SUCCESS after a supported operation completes.

    Benefit     : Acts as the operation dispatcher between command-line input
                and the arithmetic implementation functions.

    Parameters  :
        apc_info - Structure containing the two operands and receiving result.
        op       - Operator string supplied through the command line.

    Return      : Status
        SUCCESS - A supported operation was executed.
        FAILURE - Unsupported operator terminates the process using exit().
    ============================================================================
    */
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
            display_help();
            exit(FAILURE);

    }
    return SUCCESS;  
}

Status addition(APC_Info *apc_info)
{
    /*
    ============================================================================
    Function    : addition
    Description : Adds two signed arbitrary-precision integer operands using
                digit-wise addition and carry propagation. For operands with
                different signs, it delegates magnitude subtraction to
                subtraction().

    Logic       :
        1. Initializes carry, sign, data, and flag variables.
        2. If operand 1 is negative and operand 2 is positive:
            a. Swaps the operand head pointers, tail pointers, and lengths so
                the positive operand is first.
            b. Deletes the negative sign node from the second operand.
            c. Sets flag to remember that the operand order must be restored.
            d. Calls subtraction() to subtract the magnitudes and obtain the
                correct signed result.
            e. Reinserts the negative sign into operand 2.
        3. Else, if operand 1 is positive and operand 2 is negative:
            a. Deletes the negative sign node from operand 2.
            b. Calls subtraction() to calculate the difference of magnitudes.
            c. Reinserts the negative sign into operand 2.
        4. Otherwise, both operands have the same sign (both positive or both
        negative):
            a. Sets the result sign negative when both operands are negative.
            b. Starts from both operand tails and moves backward through prev.
            c. Adds corresponding digits and the current carry.
            d. If the sum is greater than 9, stores sum % 10 and sets carry.
            e. Inserts each generated digit at the front of RESULT.
            f. Processes any remaining digits from either operand, including
                the carry.
            g. Inserts a final carry if one remains.
            h. Inserts '-' at the result head when both inputs were negative.
        5. If the operands were swapped in step 2, swaps their heads, tails,
        and lengths back to their original order.
        6. Returns SUCCESS.

    Benefit     : Performs signed arbitrary-precision addition without converting
                the complete operands into a native C integer.

    Parameters  :
        apc_info - Structure containing both operands and receiving the result.

    Return      : Status
        SUCCESS - Addition or delegated subtraction completed.
    ============================================================================
    */
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
    /*
    ============================================================================
    Function    : subtraction
    Description : Subtracts two signed arbitrary-precision integer operands.
                It handles sign combinations first, then subtracts the smaller
                magnitude from the larger magnitude using borrow propagation.

    Logic       :
        1. Initializes borrow to zero, result sign to positive, and swap flag
        to zero.
        2. If operand 1 is positive and operand 2 is negative:
            a. Deletes the negative sign from operand 2.
            b. Calls addition() to add the two magnitudes.
            c. Reinserts the negative sign into operand 2.
        3. Else, if operand 1 is negative and operand 2 is positive:
            a. Deletes the negative sign from operand 1.
            b. Calls addition() to add the two magnitudes.
            c. Reinserts '-' at the head of operand 1 and at the head of
                RESULT, making the sum negative.
        4. Otherwise, both operands have the same sign or are both positive:
            a. Calls compare() to compare their magnitudes.
            b. If the magnitudes are equal and the stored lengths are equal,
                inserts zero into RESULT and returns SUCCESS.
            c. If operand 2 has a greater magnitude (cmp < 0, or its stored
                length is greater), swaps both operand heads, tails, and lengths.
                Sets flag so the original operand order can be restored. Sets
                the result sign according to the original signs and swap.
            d. Starts from both tails and subtracts digits from right to left.
            e. Subtracts the incoming borrow from the current minuend digit.
            f. If the adjusted digit is smaller than the subtrahend digit,
                adds 10, subtracts the subtrahend digit, and sets borrow to 1.
            g. Inserts each difference digit at the front of RESULT.
            h. Processes remaining digits in either list while propagating
                borrow. The code stops each traversal when it reaches '-'.
            i. Removes leading zero nodes from RESULT using delete_first().
            j. If operands were swapped, restores their original head pointers,
                tail pointers, and lengths.
            k. Inserts '-' at the result head when the calculated sign is
                negative.
        5. Returns SUCCESS.

    Benefit     : Supports subtraction across different operand lengths,
                sign combinations, and borrow chains without using native
                integer conversion.

    Parameters  :
        apc_info - Structure containing both operands and receiving the result.

    Return      : Status
        SUCCESS - Subtraction or delegated addition completed.
    ============================================================================
    */
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
            insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),'-');
        }
        else 
        {
            oprd1_temp=apc_info->OPRD1_head;
            oprd2_temp=apc_info->OPRD2_head;
            int cmp =compare(apc_info);
            //printf("%d - cmd\n",cmp);
            if((cmp == 0) && (apc_info->len_oprd2==apc_info->len_oprd1))
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
    /*
    ============================================================================
    Function    : multiplication
    Description : Multiplies two signed arbitrary-precision integers using
                digit-wise multiplication, shifted partial products, and
                addition of those partial products.

    Logic       :
        1. Initializes carry, digit data, digit-position count, and result sign.
        2. Checks whether either operand is represented by exactly one zero
        node. If so, inserts zero into RESULT and returns SUCCESS.
        3. Determines the result sign. If exactly one operand begins with '-',
        sets the result sign negative. If both are negative, the result
        remains positive.
        4. Starts traversing operand 2 from its tail (least significant digit)
        toward its head. Stops when it reaches a negative-sign node.
        5. For each digit of operand 2:
            a. Increments count to record the current digit position.
            b. Starts traversing operand 1 from its tail.
            c. Inserts count - 1 zero nodes at the front of the shifted
                partial-product list (OPRD2 of temp_mull).
            d. Multiplies the current operand-2 digit by each operand-1 digit
                and adds the carry.
            e. Extracts the new carry and stores the units digit.
            f. For the first multiplier digit, inserts partial-product digits
                into temp_mull.OPRD1; for later digits, inserts them into
                temp_mull.OPRD2.
            g. Inserts any final carry into the corresponding partial product.
            h. For the first partial product, advances to the next multiplier
                digit without addition.
            i. For each later partial product, calls addition() to add it to
                the accumulated value in temp_mull.
            j. Deletes the old temporary operand lists and transfers the
                addition result into temp_mull.OPRD1.
        6. Transfers the accumulated temp_mull.OPRD1 list into apc_info.RESULT.
        7. Inserts any remaining carry at the result head.
        8. Inserts '-' at the result head if exactly one original operand was
        negative.
        9. Deletes the remaining temporary OPRD2 and RESULT lists in temp_mull.
    10. Returns SUCCESS.

    Benefit     : Produces arbitrary-length products using a grade-school
                multiplication approach implemented with linked lists.

    Parameters  :
        apc_info - Structure containing both operands and receiving the result.

    Return      : Status
        SUCCESS - Product was generated.
    ============================================================================
    */
    int data,carry=0,count=0,sign=1;
    APC_Info temp_mull={0};
    if(((apc_info->OPRD2_head->data==0) && (apc_info->OPRD2_head->next==NULL)) || ((apc_info->OPRD1_head->data==0) && (apc_info->OPRD1_head->next==NULL)))
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),0);
        return SUCCESS;
    }
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
    /*
    ============================================================================
    Function    : division
    Description : Performs signed integer division using a repeated-subtraction
                approach. It estimates a partial quotient, multiplies the
                divisor by that partial quotient, subtracts the product from
                the current remainder, and accumulates accepted partial
                quotients.

    Logic       :
        1. Creates deep-copy backups of both original operand lists and stores
        their lengths in a local APC_Info structure.
        2. Determines the quotient sign from the operand signs:
            - Different signs produce a negative quotient.
            - Equal signs produce a positive quotient.
        Deletes the working sign nodes from the operands.
        3. If the divisor is exactly the single digit 1, transfers the working
        numerator list to RESULT, applies the quotient sign if needed,
        restores the backed-up operands, and returns SUCCESS.
        4. If the divisor is exactly the single digit 0, displays a division-
        by-zero error and exits with FAILURE.
        5. Calls compare() on the absolute operands:
            - If numerator < divisor, stores zero in RESULT, restores the
            original operands, and returns.
            - If numerator == divisor, stores one in RESULT, applies the
            quotient sign if needed, restores the original operands, and
            returns.
        6. Copies the absolute numerator to temp_rmd.OPRD1 (the current
        remainder) and the absolute divisor to temp_info.OPRD1.
        7. Deletes the working operand lists in apc_info and initializes
        apc_info.OPRD1 with a single zero node. This list is used as the
        quotient accumulator.
        8. Assigns the copied divisor to temp_rmd.OPRD2 for the initial
        remainder-versus-divisor comparison.
        9. While current remainder >= divisor:
            a. Clears the previous temp_rmd.OPRD2 references and initializes
                the estimated multiplier to 1.
            b. If the first remainder digit is smaller than the first divisor
                digit, forms a two-digit value from the first two remainder
                nodes and divides it by the first divisor digit to estimate
                the multiplier. Otherwise, the multiplier remains 1.
            c. At retry, appends the multiplier to temp_info.OPRD2 and appends
                the number of trailing zero nodes calculated as
                remainder_length - divisor_length - 1.
            d. Calls multiplication() to multiply the copied divisor by this
                partial quotient.
            e. Transfers the product list into temp_rmd.OPRD2 and counts its
                nodes to set temp_rmd.len_oprd2.
            f. Calls subtraction() to calculate current remainder minus the
                product.
            g. If the subtraction result begins with '-', the candidate
                product was too large. Decrements mul, deletes the candidate,
                product, and rejected subtraction result, then retries.
            h. Otherwise, counts the new remainder length, deletes the old
                remainder list, and transfers the subtraction result into
                temp_rmd.OPRD1.
            i. Assigns the accepted partial quotient to apc_info.OPRD2 and
                calls addition() to add it to the quotient accumulator.
            j. Transfers the addition result back to apc_info.OPRD1, deletes
                the old accumulator and temporary partial quotient, and
                restores the divisor reference in temp_rmd.OPRD2 for the next
                loop comparison.
    10. When remainder < divisor, transfers the accumulated quotient list
        from apc_info.OPRD1 to RESULT.
    11. Inserts the negative sign if the quotient sign is negative.
    12. Restores the original operand pointers from the deep-copy backups.
    13. Returns SUCCESS.

    Benefit     : Calculates an integer quotient for large signed operands
                without converting the complete values to a native integer.

    Parameters  :
        apc_info - Structure containing both operands and receiving the result.

    Return      : Status
        SUCCESS - Quotient was generated.
        FAILURE - The zero-divisor path terminates with exit(FAILURE).
    ============================================================================
    */
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

    int len,mul=1;

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

        //printf("- count\n");
        subtraction(&temp_rmd);
        /* checking selected mul is making a larger output */
        if(temp_rmd.RESULT_head->data=='-')
        {
            mul--;
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
    /*
    ============================================================================
    Function    : reminder
    Description : Calculates the remainder of two signed arbitrary-precision
                integers using partial quotient estimation, multiplication
                and repeated subtraction.

    Logic       :
        1. Creates deep-copy backup lists of both original operands using
        insert_last().
        Stores the corresponding operand lengths in the backup structure.
        2. Determines the sign of the remainder based on the sign of operand 1.
            - If both operands are positive, keeps the result positive.
            - If operand 1 is positive and operand 2 is negative, removes
                the negative sign from operand 2 and keeps the result positive.
            - If operand 1 is negative and operand 2 is positive, sets the
                result sign to negative and removes the negative sign from
                operand 1.
            - If both operands are negative, sets the result sign to
                negative and removes both negative sign nodes.
        3. Checks whether the divisor is exactly 1.
            - Inserts zero directly into RESULT.
            - Restores the original operand head and tail pointers
                from the backup lists.
            - Returns SUCCESS.
        4. Checks whether the divisor is zero.
            - Displays a division-by-zero error message.
            - Terminates the program with FAILURE.
        5. Compares the absolute magnitudes of both operands using compare().

            - If the numerator is smaller than the divisor:
                    - Transfers the numerator linked list directly into RESULT.
                    - Inserts the negative sign if the dividend is negative.
                    - Restores the original operand head and tail pointers.
                    - Returns SUCCESS.

            - If both operands are equal:
                    - Inserts zero into RESULT.
                    - Restores the original operand head and tail pointers.
                    - Returns SUCCESS.
        6. Initializes two temporary APC_Info structures:
            - temp_rmd  : Maintains the current remainder.
            - temp_info : Maintains the divisor and partial quotient.
        7. Copies the absolute numerator into temp_rmd.OPRD1.
        8. Copies the absolute divisor into temp_info.OPRD1.
        9. Deletes the working operand lists from apc_info and initializes
        apc_info.OPRD1 with a single zero node to maintain the partial
        quotient accumulator.

    10. Assigns the divisor linked list to temp_rmd.OPRD2 and initializes
        len_oprd2 for the initial remainder-versus-divisor comparison.
    11. Repeats the remainder calculation while the current remainder
        is greater than or equal to the divisor.

        a) Clears the previous temp_rmd.OPRD2 references and initializes
            the multiplier to 1.
        b) Estimates the partial quotient multiplier:
                - Compares the first digit of the current remainder with
                    the first digit of the divisor.
                - If the remainder's first digit is smaller, combines the
                    first two remainder digits and divides the combined
                    value by the first divisor digit.
                - Otherwise, retains the multiplier as 1.
        c) At the retry label, inserts the estimated multiplier into
            temp_info.OPRD2.
        d) Calculates the number of trailing zeros using:
                Current remainder length - Divisor length - 1
        e) Inserts the calculated number of zero nodes after the
            multiplier to represent its decimal position.
        f) Calls multiplication() to multiply the divisor by the
            estimated partial quotient.
        g) Transfers the multiplication result into temp_rmd.OPRD2
            and calculates its actual node count to update len_oprd2.
        h) Calls subtraction() to subtract the multiplication result
            from the current remainder.
        i) Checks whether the subtraction result begins with '-'.
                - If negative, the estimated multiplier is too large.
                - Decrements the multiplier by one.
                - Deletes the invalid partial quotient, multiplication
                    result and subtraction result.
                - Uses goto retry to repeat multiplication and subtraction
                    with the reduced multiplier.
        j) If subtraction produces a non-negative result:
                - Calculates the new remainder length by traversing
                    the subtraction result.
                - Deletes the previous remainder linked list.
                - Transfers the subtraction result into temp_rmd.OPRD1.
                - Clears the temporary RESULT pointers.
                - Deletes the temporary multiplication result.
        k) Assigns the accepted partial quotient to apc_info.OPRD2.
        l) Calls addition() to accumulate the accepted partial quotient
            in apc_info.OPRD1.
        m) Deletes the previous quotient accumulator and transfers
            the addition result back into apc_info.OPRD1.
        n) Deletes the temporary partial quotient list and clears
            the corresponding operand pointers.
        o) Restores the divisor reference in temp_rmd.OPRD2 for
            the next remainder comparison.
    12. Repeats the process until the current remainder becomes
        smaller than the divisor.
    13. Transfers the final remainder linked list from temp_rmd.OPRD1
        into apc_info.RESULT.
    14. Inserts '-' at the beginning of RESULT if the dividend
        was negative.
    15. Restores the original operand head and tail pointers from
        the backup lists.
    16. Returns SUCCESS.

    Benefit     : Calculates the remainder of large signed integers using
                linked-list multiplication and subtraction without
                converting complete operands into built-in integer types.

    Parameters  :
        apc_info - Structure containing both operands and receiving
                the calculated remainder.

    Return      : Status
        SUCCESS - Remainder calculation completed successfully.
        FAILURE - Division by zero terminates the program.

    ============================================================================
    */
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
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    }
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data!='-')
    {
        sign=-1;
        delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
    }
    else if((apc_info->OPRD1_head)->data=='-' && (apc_info->OPRD2_head)->data=='-')
    {
        sign=-1;
        delete_first(&(apc_info->OPRD1_head),&(apc_info->OPRD1_tail));
        delete_first(&(apc_info->OPRD2_head),&(apc_info->OPRD2_tail));
    }

    if((apc_info->OPRD2_head->data==1) && (apc_info->OPRD2_head->next==NULL))
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),0);
    
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
    if(cmp == 0)
    {
        insert_first(&(apc_info->RESULT_head),&(apc_info->RESULT_tail),0);
        
        apc_info->OPRD1_head=backup.OPRD1_head;
        apc_info->OPRD1_tail=backup.OPRD1_tail;
        apc_info->OPRD2_head=backup.OPRD2_head;
        apc_info->OPRD2_tail=backup.OPRD2_tail;
        return SUCCESS;
    }
    
    APC_Info temp_info={0};
    APC_Info temp_rmd={0};

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

    int len,mul=1;

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
            mul--;
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
    /*
    ============================================================================
    Function    : compare
    Description : Compares the magnitudes of two operands represented by
                linked lists. Negative sign nodes are ignored during digit
                comparison.

    Logic       :
        1. Compares len_oprd1 and len_oprd2.
        2. If the lengths differ, returns len_oprd1 - len_oprd2. This makes
        the operand with more stored digits the greater magnitude.
        3. If lengths are equal, initializes traversal pointers at both heads.
        4. If operand 1's current node contains '-', advances operand 1's
        pointer to the next node and continues.
        5. If operand 2's current node contains '-', advances operand 2's
        pointer to the next node and continues.
        6. Compares corresponding digit nodes from most significant to least
        significant.
        7. At the first differing digit, returns operand1_digit - operand2_digit.
        8. If all traversed digits match, returns zero.

    Benefit     : Provides magnitude ordering for subtraction, division, and
                remainder calculations without converting operands to a
                built-in integer type.

    Parameters  :
        apc_info - Structure containing both operands and their stored lengths.

    Return      : int
        Positive value - Operand 1 has a greater magnitude.
        Negative value - Operand 1 has a smaller magnitude.
        Zero          - Magnitudes are equal according to the stored lengths
                        and the traversed digit nodes.
    ============================================================================
    */
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
            oprd2_temp=oprd2_temp->next;
            continue;
        }
        if(oprd1_temp->data != oprd2_temp->data)
            return oprd1_temp->data - oprd2_temp->data;
        oprd1_temp=oprd1_temp->next;
        oprd2_temp=oprd2_temp->next;
    }
    return 0;
    
}

void display_help()
{
    /*
    ============================================================================
    Function    : display_help
    Description : Displays the user manual for the Arbitrary Precision
                  Calculator, including its purpose, command-line syntax,
                  supported operators, examples, and input requirements.

    Benefit     : Helps users understand how to provide operands and select
                  arithmetic operations from the command line.

    Parameters  :
                  No parameters.

    Return      : void
                  Does not return any value.
    ============================================================================
    */
    printf("\n");
    printf("************************************************************\n");
    printf("              ARBITRARY PRECISION CALCULATOR\n");
    printf("                       USER MANUAL\n");
    printf("************************************************************\n");

    printf("\nDESCRIPTION:\n");
    printf("  This project performs arithmetic operations on large signed\n");
    printf("  integers using dynamically allocated doubly linked lists.\n");
    printf("  Each decimal digit is stored in a separate linked-list node,\n");
    printf("  allowing calculations beyond the range of built-in integer\n");
    printf("  data types, subject to available memory.\n");

    printf("\nUSAGE:\n");
    printf("  ./a.out <operand1> <operator> <operand2>\n");

    printf("\nSUPPORTED OPERATORS:\n");
    printf("  +    Addition of two signed integers.\n");
    printf("  -    Subtraction of operand 2 from operand 1.\n");
    printf("  x    Multiplication of two signed integers.\n");
    printf("  *    Multiplication (quote '*' when required by the shell).\n");
    printf("  /    Integer division; quotient is truncated toward zero.\n");
    printf("  %%    Remainder operation (see implementation note below).\n");

    printf("\nEXAMPLES:\n");
    printf("  ./a.out 1234 + 567\n");
    printf("  ./a.out -1234 - 567\n");
    printf("  ./a.out 12345678901234567890 x 9\n");
    printf("  ./a.out 19999 / 2345\n");
    printf("  ./a.out 17 %% 5\n");
    printf("  ./a.out 12 '*' 3\n");

    printf("\nINPUT REQUIREMENTS:\n");
    printf("  * Provide exactly two operands and one operator.\n");
    printf("  * Each operand must contain decimal digits only, with an\n");
    printf("    optional leading '+' or '-' sign.\n");
    printf("  * Decimal/floating-point values are not supported.\n");
    printf("  * The program expects the operator as a single character.\n");

    printf("\nIMPLEMENTATION:\n");
    printf("  * Operands are stored as doubly linked lists.\n");
    printf("  * The most significant digit is stored at the head.\n");
    printf("  * The least significant digit is stored at the tail.\n");
    printf("  * A negative sign is stored as a separate head node.\n");
    printf("  * Arithmetic is performed digit by digit.\n");


    printf("\n************************************************************\n\n");
}