#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

int Count_members_dat();
int Count_accounts_dat();
int Count_violations_dat();

int Read_members_dat(const int size,Member member_list[]);
int Read_accounts_dat(const int size,Account account_list[]);
int Read_violations_dat(const int size,Violation violation_list[]);

bool Append_members_dat(const Member *member);
bool Append_accounts_dat(const Account *account);
bool Append_violations_dat(const Violation *violation);

// bool Update_member_dat(int idx, const Member *member);
// bool Update_account_dat(int idx, const Account *account);
// bool Update_violation_dat(int idx, const Violation *violation);

// bool Delete_member_dat(int idx);
// bool Delete_account_dat(int idx);
// bool Delete_violation_dat(int idx);

#endif