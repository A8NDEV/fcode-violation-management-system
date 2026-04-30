#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

int Count_members_dat();
int Count_accounts_dat();
int Count_violations_dat();

// int Read_members_dat(Member member_list[], int max_records);
// int Read_accounts_dat(Account account_list[], int max_records);
// int Read_violations_dat(Violation violation_list[], int max_records);

// bool Append_member_dat(const Member *member);
// bool Append_account_dat(const Account *account);
// bool Append_violation_dat(const Violation *violation);

// bool Update_member_dat(int idx, const Member *member);
// bool Update_account_dat(int idx, const Account *account);
// bool Update_violation_dat(int idx, const Violation *violation);

// bool Delete_member_dat(int idx);
// bool Delete_account_dat(int idx);
// bool Delete_violation_dat(int idx);

#endif