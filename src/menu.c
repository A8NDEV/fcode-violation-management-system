#include "menu.h"
#include "auth.h"
#include "member.h"
#include "utils.h"
#include "violation.h"
#include "sort_violation.h"
#include <stdio.h>
#include "sort_violation.h"
#include "report.h"

int show_bcn_menu(Account *session, Member memberList[], int *memberCount, Account accountList[], Violation violationList[], int *violationCount) {
    int choice;
    while (1) {
        system("cls");
        print_bcn_logo();
        UI_Header("BCN MANAGEMENT MENU", ANSI_COLOR_CYAN);
        UI_Menu_Item(1, "ADD MEMBER", ANSI_COLOR_CYAN);
        UI_Menu_Item(2, "UPDATE MEMBER INFO", ANSI_COLOR_CYAN);
        UI_Menu_Item(3, "REMOVE MEMBER", ANSI_COLOR_CYAN);
        UI_Menu_Item(4, "RECORD VIOLATION", ANSI_COLOR_CYAN);
        UI_Menu_Item(5, "MARK FINE AS PAID", ANSI_COLOR_CYAN);
        UI_Menu_Item(6, "VIEW ALL VIOLATIONS", ANSI_COLOR_CYAN);
        UI_Menu_Item(7, "STATISTICS BY TEAM", ANSI_COLOR_CYAN);
        UI_Menu_Item(8, "VIEW DELETED MEMBERS", ANSI_COLOR_CYAN);
        UI_Menu_Item(9, "EXPORT REPORT", ANSI_COLOR_CYAN);
        UI_Menu_Item(10, "CHANGE PASSWORD", ANSI_COLOR_CYAN);
        UI_Menu_Item(11, "PROCESS MEETING ATTENDANCE", ANSI_COLOR_CYAN);
        UI_Divider(50, ANSI_COLOR_CYAN);
        UI_Menu_Item(0, "LOGOUT", ANSI_COLOR_CYAN);
        UI_Card_End(ANSI_COLOR_CYAN);
        
        Input_user_choose(&choice);
        switch (choice) {
        case 1:
            Menu_create(memberCount, memberList, accountList);
            break;
        case 2:
            Menu_update(*memberCount, memberList, accountList);
            break;
        case 3:
            Menu_remove(memberCount, memberList, accountList, violationCount, violationList);
            break;
        case 4:
            Menu_record_violation(memberCount, memberList, violationCount, violationList);
            break;
        case 5:
            Menu_mark_paid(memberCount, memberList, violationCount, violationList);
            break;
        case 6:
            Menu_view_all_violations(*memberCount, memberList, *violationCount, violationList);
            break;
        case 7:
            view_Statistics_By_Team(memberList, *memberCount, violationList, *violationCount);
            break;
        case 8:
            Menu_view_deleted_members();
            break;
        case 9:
            Export_Violation_Report(*memberCount, memberList, *violationCount, violationList);
            break;
        case 10:
            changePassword(accountList, session->role, memberCount, session);
            break;
        case 11:
            Menu_batch_attendance(*memberCount, memberList, violationCount, violationList);
            break;
        case 0:
            printf(ANSI_BRIGHT_CYAN "LOGGED OUT SUCCESSFULLY!\n" ANSI_COLOR_RESET);
            return 0;
        default:
            Announcement_unaivailable_option();
        }
    }
}

int show_member_menu(Account *session, Member memberList[], int memberCount, Account accountList[], int accountCount, Violation violationList[], int violationCount) {
    int choice;
    while (1) {
        system("cls");
        print_member_logo();
        UI_Header("MEMBER MENU", ANSI_COLOR_CYAN);
        UI_Menu_Item(1, "VIEW PROFILE", ANSI_COLOR_CYAN);
        UI_Menu_Item(2, "VIEW VIOLATION HISTORY", ANSI_COLOR_CYAN);
        UI_Menu_Item(3, "VIEW TOTAL DEBT", ANSI_COLOR_CYAN);
        UI_Menu_Item(4, "VIEW CLUB MEMBER LIST", ANSI_COLOR_CYAN);
        UI_Menu_Item(5, "CHANGE PASSWORD", ANSI_COLOR_CYAN);
        UI_Menu_Item(6, "VIEW SORTED LIST", ANSI_COLOR_CYAN);
        UI_Divider(50, ANSI_COLOR_CYAN);
        UI_Menu_Item(0, "LOGOUT", ANSI_COLOR_CYAN);
        UI_Card_End(ANSI_COLOR_CYAN);

        Input_user_choose(&choice);




        switch (choice) {
        case 1: {
            int idx = Find_studentId(memberCount, memberList, session->studentId);
            if (idx == -1) {
                printf("Member not found!\n");
                break;
            }
            Member m = memberList[idx];
            view_Profile(m);
            break;
        }
        case 2:
            view_Own_Violations(violationList,violationCount,session->studentId);
            break;
        case 3:
            check_to_Paid(violationList,violationCount,session->studentId);
            break;
        case 4: {
            view_CLB_Profile(memberList,memberCount);
            break;
        }
        case 5:
            changePassword(accountList, session->role, &accountCount, session);
            break;
        case 6:
            printf("ENTER YOUR CHOICE\n");
            printf(ANSI_BOLD ANSI_COLOR_GREEN "[1]" ANSI_COLOR_RESET " ASCENDING\n");
            printf(ANSI_BOLD ANSI_COLOR_GREEN "[2]" ANSI_COLOR_RESET " DESCENDING\n");
            int choice;
            char buf[32];
            if (fgets(buf, sizeof(buf), stdin) != NULL && sscanf(buf, "%d", &choice) == 1) {
                if (choice == 1) {
                    sort_CLB_Violations(memberList,memberCount,1);

                } else if (choice == 2 ) {
                    sort_CLB_Violations(memberList,memberCount,0);
                } else {
                    Announcement_unaivailable_option();
                }
            }
            break;
        case 0:
            printf(ANSI_BRIGHT_CYAN "LOGGED OUT SUCCESSFULLY!\n" ANSI_COLOR_RESET);
            return 0;
        default:
            Announcement_unaivailable_option();
        }
    }
}
