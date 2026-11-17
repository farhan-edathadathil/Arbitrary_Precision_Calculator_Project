#ifndef DLL_H
#define DLL_H


Status save_oprd(APC_Info *apc_info,char *oprd1_str,char *oprd2_str);
Status insert_first(DLL **head, DLL **tail, int data);
Status insert_last(DLL **head, DLL **tail, int data);
Status delete_first(DLL **head, DLL **tail);
Status delete_list(DLL **head, DLL **tail);
void display_error(char *error);
void display_result(APC_Info *apc_info,char ch);
void print_list(DLL *head);


#endif