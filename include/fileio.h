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


#endif