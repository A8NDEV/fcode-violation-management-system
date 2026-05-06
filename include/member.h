#ifndef MEMBER_H
#define MEMBER_H

#include "types.h"

int Delete_member(const int index,char studentId[],int *member_size,Member member_list[],Account account_list[],int *violation_size,Violation violation_list[]);
void main_menu_CRUD(int *member_size,Member member_list[],Account account_list[],int *violation_size,Violation violation_list[]);

#endif