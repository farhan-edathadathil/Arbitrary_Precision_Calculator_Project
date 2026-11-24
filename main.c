/*

NAME : FARHAN E
ID : 26018_199
DATE : 23/11/2016

============================================================================
Project Name : ARBITRARY PRECISION CALCULATOR (APC)

Description  :
    This project implements an Arbitrary Precision Calculator in C to
    perform arithmetic operations on signed integers of any practical
    length without depending on the storage limits of built-in integer
    data types.

    The project uses a Doubly Linked List (DLL) to store the operands
    and results. Each node stores one decimal digit, while a negative
    sign is stored as a separate node at the beginning of the operand.

    Arithmetic operations are performed digit by digit using linked
    list traversal, carry propagation, borrow propagation and temporary
    linked lists.

Supported Operations :
    '+'  - Addition
    '-'  - Subtraction
    'x'  - Multiplication
    '*'  - Multiplication
    '/'  - Integer Division
    '%'  - Remainder

Working Logic :
    1. Receives two operands and an arithmetic operator through
       command-line arguments.

    2. Validates the number of command-line arguments and checks
       whether the operator contains only one character.

    3. Calls save_oprd() to validate the operand strings and convert
       them into doubly linked lists.

    4. Stores each decimal digit in an individual DLL node.
       If an operand is negative, stores '-' as the first node.
       The operand lengths are maintained separately, excluding
       the negative sign.

    5. Calls do_operation() to identify the requested arithmetic
       operation and invoke the corresponding function.

    6. Performs the selected arithmetic operation:
           - Addition       : Uses carry propagation.
           - Subtraction    : Uses borrow propagation and magnitude
                              comparison.
           - Multiplication : Uses digit-wise multiplication, shifted
                              partial products and addition.
           - Division       : Uses partial quotient estimation,
                              multiplication, subtraction and quotient
                              accumulation.
           - Remainder      : Uses the division same approach without accumulation.

    7. Stores the calculated result in the result linked list.

    8. Calls display_result() to display both original operands,
       the selected operator and the calculated result.

    9. Registers cleanup() using atexit() to release the operand and
       result linked lists when the program terminates.

Output :
    Displays the original operands, arithmetic operator and calculated
    result in the following format:

        [OPERAND 1] : <operand1>
        [OPERAND 2] : <operand2>

        [OUTPUT]    : <operand1> <operator> <operand2> = <result>

    Displays an error message when invalid input, an unsupported
    operator or division by zero is encountered.

    Sample output:

    $ ./a.out 1234 + 123
    [OPERAND 1] : 1234
    [OPERAND 2] : 123

    [OUTPUT]    : 1234 + 123 = 1357

    [CLEAN UP]  : Memory deallocation successfull

    $ ./a.out 19999 / 2345
    [OPERAND 1] : 19999
    [OPERAND 2] : 2345

    [OUTPUT]    : 19999 / 2345 = 8

    [CLEAN UP]  : Memory deallocation successfull

Data Structure :
    DLL:
        Stores one decimal digit or a negative sign.
        Contains prev and next pointers for bidirectional traversal.

    APC_Info:
        Maintains head and tail pointers for both operands and result.
        Also maintains the lengths of both operands, excluding signs.

============================================================================
*/

#include "apc.h"
#include "edit_DLL.h"
#include "operation.h"

APC_Info apc_info={NULL};

void cleanup(void)
{
    /*
    ============================================================================
    Function    : cleanup
    Description : Releases the operand and result linked lists held by the
                global apc_info structure. It is registered with atexit().

    Logic       :
        1. Checks whether OPRD1_head is NULL. If it is NULL, returns immediately
        without attempting the other list deletions or printing the cleanup
        message.
        2. Calls delete_list() for operand 1.
        3. Calls delete_list() for operand 2.
        4. Calls delete_list() for the result.
        5. Prints the memory-deallocation success message.

    Benefit     : Centralizes release of the lists still owned by the global
                calculator structure when the program exits.

    Parameters  :
        No parameters. Uses the global apc_info object.

    Return      : void
        Does not return a value.
    ============================================================================
    */
    if(apc_info.OPRD1_head==NULL)
        return;
    delete_list(&(apc_info.OPRD1_head),&(apc_info.OPRD1_tail));
    delete_list(&(apc_info.OPRD2_head),&(apc_info.OPRD2_tail));
    delete_list(&(apc_info.RESULT_head),&(apc_info.RESULT_tail));
    printf("\n%-11s : Memory deallocation successfull\n\n","[CLEAN UP]");
}

int main(int argc,char **argv)
{
    if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        display_help();
        return SUCCESS;
    }
    atexit(cleanup);
    if(argc!=4)
    {
        display_error("Not Enough arguments given (use -h / --help for user manual)");
        exit(FAILURE);
    }
    if(strlen(argv[2])>1)
    {
        display_error("Unsupported operator (use -h / --help for user manual)");
        exit(FAILURE);
    }
    save_oprd(&apc_info,argv[1],argv[3]);
    do_operation(&apc_info,argv[2]);
    
    return (SUCCESS);
}