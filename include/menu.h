#ifndef MENU_H
#define MENU_H

#include "types.h"

// show_bcn_menu - Vong lap menu danh cho Ban Chu Nhiem (role = 1)
// Tra ve 0 khi user chon Logout, 1 neu can tiep tuc (khong dung trong truong hop nay)
int show_bcn_menu(Account *session,
                  Member memberList[], int *memberCount,
                  Account accountList[],
                  Violation violationList[], int *violationCount);

// show_member_menu - Vong lap menu danh cho thanh vien thuong (role = 0)
// Tra ve 0 khi user chon Logout
int show_member_menu(Account *session,
                     Member memberList[], int memberCount,
                     Account accountList[], int accountCount,
                     Violation violationList[], int violationCount);

#endif
