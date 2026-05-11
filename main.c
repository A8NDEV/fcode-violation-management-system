#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/auth.h"
#include "include/fileio.h"
#include "include/utils.h"
#include "include/menu.h"

int main() {
    Account   accountList[MAX_ACCOUNT];
    Member    memberList[MAX_ACCOUNT];
    Violation violationList[MAX_ACCOUNT * 10];

    int accountCount   = 0;
    int memberCount    = 0;
    int violationCount = 0;

    /* Load du lieu tu file khi khoi dong */
    accountCount   = Count_accounts_dat();
    if (accountCount   > 0) Read_accounts_dat(accountCount, accountList);
    memberCount    = Count_members_dat();
    if (memberCount    > 0) Read_members_dat(memberCount, memberList);
    violationCount = Count_violations_dat();
    if (violationCount > 0) Read_violations_dat(violationCount, violationList);

    Account session;
    int isLogin = 0;
    int choice;

    /* Vong lap chinh: chi quan ly trang thai dang nhap / thoat */
    while (1) {
        if (!isLogin) {
            printf("\n====================================\n");
            printf("    F-CODE VIOLATION MANAGEMENT     \n");
            printf("====================================\n");
            printf("1. Login\n");
            printf("0. Exit\n");
            printf("====================================\n");

            Input_user_choose(&choice);

            if (choice == 1) {
                login(accountList, &accountCount, &isLogin, NULL, &session);
            } else if (choice == 0) {
                break;
            }
        } else if (session.role == 1) {
            /* BCN menu tu quan ly vong lap noi bo, tra ve 0 khi logout */
            isLogin = show_bcn_menu(&session, memberList, &memberCount,
                                    accountList, violationList, &violationCount);
        } else {
            /* Member menu tu quan ly vong lap noi bo, tra ve 0 khi logout */
            isLogin = show_member_menu(&session, memberList, memberCount,
                                       accountList, accountCount,
                                       violationList, violationCount);
        }
    }

    return 0;
}