#include "menu.h"
#include "auth.h"
#include "member.h"
#include "utils.h"
#include "violation.h"
#include <stdio.h>
#include "sort_violation.h"
#include "report.h"

int show_bcn_menu(Account *session, Member memberList[], int *memberCount, Account accountList[], Violation violationList[], int *violationCount) {
    int choice;
    while (1) {
        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════╗\n");
        printf("║          BCN MANAGEMENT MENU           ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "ADD MEMBER\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "UPDATE MEMBER INFO\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "REMOVE MEMBER\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [4] " ANSI_COLOR_RESET "RECORD VIOLATION\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [5] " ANSI_COLOR_RESET "MARK FINE AS PAID\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [6] " ANSI_COLOR_RESET "VIEW ALL VIOLATIONS\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [7] " ANSI_COLOR_RESET "STATISTICS BY TEAM\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [8] " ANSI_COLOR_RESET "VIEW DELETED MEMBERS\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [9] " ANSI_COLOR_RESET "EXPORT REPORT\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN "[10] " ANSI_COLOR_RESET "CHANGE PASSWORD\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN "[11] " ANSI_COLOR_RESET "PROCESS MEETING ATTENDANCE\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "LOGOUT\n");
        printf("\n");
        printf(ANSI_COLOR_CYAN ANSI_BOLD
               "------------------------------------------\n" ANSI_COLOR_RESET);
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ SELECT FUNCTION: " ANSI_COLOR_RESET);
        char buf[32];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &choice) != 1) choice = -1;
        } else {
            choice = -1;
        }
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


        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════╗\n");
        printf("║              MEMBER MENU               ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "VIEW PROFILE\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "VIEW VIOLATION HISTORY\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "VIEW TOTAL DEBT\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [4] " ANSI_COLOR_RESET "VIEW CLUB MEMBER LIST\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [5] " ANSI_COLOR_RESET "CHANGE PASSWORD\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [6] " ANSI_COLOR_RESET "VIEW MEMBER SORTED LIST BY NUMBER OF VIOLATIONS\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "LOGOUT\n");
        printf("\n");
        printf(ANSI_COLOR_CYAN ANSI_BOLD
               "------------------------------------------\n" ANSI_COLOR_RESET);
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ SELECT FUNCTION: " ANSI_COLOR_RESET);
        char buf[32];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &choice) != 1) choice = -1;
        } else {
            choice = -1;
        }




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
