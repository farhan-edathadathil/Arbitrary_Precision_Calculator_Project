#ifndef APC_H
#define APC_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>


typedef enum
{
    SUCCESS,
    FAILURE

}Status;

typedef struct node
{
    int data;
    struct node *prev;
    struct node *next;
}DLL;

typedef struct 
{
    DLL *OPRD1_head;
    DLL *OPRD1_tail;
    DLL *OPRD2_head;
    DLL *OPRD2_tail;
    DLL *RESULT_head;
    DLL *RESULT_tail;
}APC_Info;

#endif