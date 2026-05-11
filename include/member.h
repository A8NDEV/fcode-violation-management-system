#ifndef MEMBER_H
#define MEMBER_H

#include "types.h"

void Menu_create(int *member_size, Member member_list[], Account account_list[]);
void Menu_remove(int *member_size, Member member_list[], Account account_list[], int *violation_size, Violation violation_list[]);
void Menu_update(const int member_size, Member member_list[], Account account_list[]);
int Delete_member(const int index, char studentId[], int *member_size, Member member_list[], Account account_list[], int *violation_size, Violation violation_list[]);

#endif