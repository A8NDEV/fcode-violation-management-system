#ifndef MEMBER_H
#define MEMBER_H
#include "types.h"



void view_Profile(Member currentuser);
void view_CLB_Profile(Member member[], int size_memberlist);
void Menu_create(int *member_size, Member member_list[], int *account_size, Account account_list[]);
void Menu_remove(int *member_size, Member member_list[], int *account_size, Account account_list[], int *violation_size, Violation violation_list[]);
void Menu_update(const int member_size, Member member_list[], Account account_list[]);
void Menu_view_deleted_members(void);
int Delete_member(const int index, char studentId[], int *member_size, Member member_list[], int *account_size, Account account_list[], int *violation_size, Violation violation_list[]);
void Print_Deleted_Members(const int member_size, Member member_list[]);

#endif
