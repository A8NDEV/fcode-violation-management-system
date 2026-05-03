#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

int Count_members_dat();
int Count_accounts_dat();
int Count_violations_dat();

bool Read_members_dat(const int size,Member member_list[]);
bool Read_accounts_dat(const int size,Account account_list[]);
bool Read_violations_dat(const int size,Violation violation_list[]);

bool Append_members_dat(const Member member);
bool Append_accounts_dat(const Account account);
bool Append_violations_dat(const Violation violation);
bool Append_deleted_members_dat(const Member record);
bool Append_deleted_accounts_dat(const Account record);

bool Update_members_dat(const int index,const Member record);
bool Update_accounts_dat(const int index,const Account record);
bool Update_violations_dat(const int index,const Violation record);

bool Rewrite_members_dat(const int member_size,Member member_list[]);
bool Rewrite_accounts_dat(const int member_size,Account account_list[]);
bool Rewrite_violations_dat(const int violation_size,Violation violation_list[]);

#endif