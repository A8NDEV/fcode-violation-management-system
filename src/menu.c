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
            view_Statistics_By_Team(memberList, *memberCount, violationList, *violationCount);
            break;
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
        case 0:
            printf("Logged out successfully.\n");
            return 0;
        default:
            Announcement_unaivailable_option();
        }
    }
}
