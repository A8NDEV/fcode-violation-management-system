#ifndef TYPES_H
#define TYPES_H

#include <time.h>
#define LONG_SIZE 50
#define SHORT_SIZE 15
typedef struct Member{
    char fullName[LONG_SIZE],email[LONG_SIZE],phone[SHORT_SIZE],studentId[SHORT_SIZE];
    int team,role,violationCount,consecutiveAbsences;
    double totalFine;
}Member;
typedef struct Account{
    char studentId[SHORT_SIZE];
    char password[LONG_SIZE];
    int role;
    int isLocked;
    int failCount;
}Account;
typedef struct Violation{
    char studentId[SHORT_SIZE],note[LONG_SIZE];
    int reason,isPaid,penalty;
    time_t violationTime;
    double fine;
}Violation;

Member Init_Member();
Account Init_Account();
Violation Init_Violation();

#endif