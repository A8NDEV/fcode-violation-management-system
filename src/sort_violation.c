#include <stdio.h>
#include <stdlib.h>
#include "sort_violation.h"
#include "auth.h"
#include "utils.h"



void shellSortMembers(Member arr[], int size, int isAscending) {
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            Member temp = arr[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                
                int condition = isAscending ? (arr[j - gap].violationCount > temp.violationCount) 
                                            : (arr[j - gap].violationCount < temp.violationCount);
                if (condition) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
}



void sort_CLB_Violations(Member member[], int size, int isAscending) {
    if (size <= 0) {
        printf("No members in the club to sort.\n");
        printf("\nPress Enter to return...");
        getchar();
        system("cls");
        return;
    }

    Member hehe[size];

    for (int i = 0; i < size; i++) {
        hehe[i] = member[i];
    }

    shellSortMembers(hehe, size, isAscending);
    
    system("cls");
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    if (isAscending == 1) {
        printf("╔═════════════════════════════════════════════════════════════════════════╗\n");
        printf("║       LIST OF MEMBERS SORTED BY VIOLATIONS (ASCENDING: LEAST -> MOST)   ║\n");
        printf("╚═════════════════════════════════════════════════════════════════════════╝\n");
    } else {
        printf("╔═════════════════════════════════════════════════════════════════════════╗\n");
        printf("║       LIST OF MEMBERS SORTED BY VIOLATIONS (DESCENDING: MOST -> LEAST)  ║\n");
        printf("╚═════════════════════════════════════════════════════════════════════════╝\n");
    }
    printf(ANSI_COLOR_RESET "\n");
    
    printf(ANSI_COLOR_CYAN "%-4s %-12s %-25s %-12s %-16s %-12s\n",
           "NO", "STUDENT ID", "FULL NAME", "TEAM", "ROLE", "VIOLATIONS");
    printf("%-4s %-12s %-25s %-12s %-16s %-12s\n" ANSI_COLOR_RESET,
           "---", "----------", "---------", "----", "----", "----------");

    const char *teams[] = {"Academic", "Planning", "HR", "Media"};
    const char *roles[] = {"Member", "Leader/Vice", "Ban Chu Nhiem"};

    for (int i = 0; i < size; i++) {
        printf("%-4d %-12s %-25s %-12s %-16s " ANSI_BRIGHT_YELLOW "%-12d\n" ANSI_COLOR_RESET,
               i + 1,
               hehe[i].studentId,
               hehe[i].fullName,
               teams[hehe[i].team],
               roles[hehe[i].role],
               hehe[i].violationCount);
    }

    printf("\nPress Enter to return...");
    getchar();
    system("cls");
}

void Menu_sort_violations(Member memberList[], int memberCount) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║         SORT MEMBERS BY VIOLATION      ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");
    printf("SELECT SORT ORDER:\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "ASCENDING (Least -> Most)\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "DESCENDING (Most -> Least)\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "Exit\n");
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);

    int sortChoice;
    Input_user_choose(&sortChoice);

    if (sortChoice == 0) {
        system("cls");
        return;
    } else if (sortChoice == 1) {
        sort_CLB_Violations(memberList, memberCount, 1);
    } else if (sortChoice == 2) {
        sort_CLB_Violations(memberList, memberCount, 0);
    } else {
        Announcement_unaivailable_option();
    }
}