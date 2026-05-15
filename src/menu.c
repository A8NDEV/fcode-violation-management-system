#include "menu.h"
#include "auth.h"
#include "member.h"
#include "utils.h"
#include "violation.h"
#include <stdio.h>

int show_bcn_menu(Account *session, Member memberList[], int *memberCount, Account accountList[], Violation violationList[], int *violationCount) {
    int choice;
    while (1) {
        printf("\n====================================\n");
        printf("         BCN MANAGEMENT MENU        \n");
        printf("====================================\n");
        printf("1. Add member\n");
        printf("2. Update member info\n");
        printf("3. Remove member\n");
        printf("4. Record violation\n");
        printf("5. Mark fine as paid\n");
        printf("6. View all violations\n");
        printf("7. Statistics by team\n");
        printf("8. Export report\n");
        printf("9. Change password\n");
        printf("0. Logout\n");
        printf("====================================\n");
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
        case 8:
            break;
        case 9:
            changePassword(accountList, session->role, memberCount, session);
            break;
        case 0:
            printf("Logged out successfully.\n");
            return 0;
        default:
            Announcement_unaivailable_option();
        }
    }
}

int show_member_menu(Account *session, Member memberList[], int memberCount, Account accountList[], int accountCount, Violation violationList[], int violationCount) {
    int choice;
    while (1) {

        printf("\n======================================\n");
        printf("           MEMBER MENU                \n");
        printf("======================================\n");
        printf("1. View profile\n");
        printf("2. View violation history\n");
        printf("3. View total debt\n");
        printf("4. View club member list\n");
        printf("5. Change password\n");
        printf("0. Logout\n");
        printf("======================================\n");
        Input_user_choose(&choice);

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
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clear_buffer();

        switch (choice) {
        case 1: {
            int idx = Find_studentId(memberCount, memberList, session->studentId);
            if (idx == -1) {
                printf("Member not found!\n");
                break;
            }
            Member *m = &memberList[idx];
            printf("\n--- PROFILE ---\n");
            printf("Student ID : %s\n", m->studentId);
            printf("Full Name  : %s\n", m->fullName);
            printf("Email      : %s\n", m->email);
            printf("Phone      : %s\n", m->phone);
            char *teams[] = {"Academic", "Planning", "HR", "Media"};
            char *roles[] = {"Member", "Leader/Vice", "Ban Chu Nhiem"};
            printf("Team       : %s\n", teams[m->team]);
            printf("Role       : %s\n", roles[m->role]);
            break;
        }
        case 2:
        case 3:
            break;
        case 4: {
            char *teams[] = {"Academic", "Planning", "HR", "Media"};
            char *roles[] = {"Member", "Leader/Vice", "BCN"};
            printf("\n%-12s %-25s %-10s %-14s\n", "StudentID", "Full Name", "Team", "Role");
            printf("%-12s %-25s %-10s %-14s\n", "---------", "---------", "----", "----");
            for (int i = 0; i < memberCount; i++) {
                printf("%-12s %-25s %-10s %-14s\n", memberList[i].studentId, memberList[i].fullName, teams[memberList[i].team], roles[memberList[i].role]);
            }
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
            if (scanf("%d",&choice) == 1) {
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
            printf("Logged out successfully.\n");
            return 0;
        default:
            Announcement_unaivailable_option();
        }
    }
}
