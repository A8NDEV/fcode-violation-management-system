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
        UI_Return_Prompt();
        system("cls");
        return;
    }

    Member hehe[size];

    for (int i = 0; i < size; i++) {
        hehe[i] = member[i];
    }

    shellSortMembers(hehe, size, isAscending);
    
    system("cls");
    if (isAscending == 1) {
        UI_Header("SORTED BY VIOLATIONS (ASCENDING: LEAST -> MOST)", ANSI_COLOR_CYAN);
    } else {
        UI_Header("SORTED BY VIOLATIONS (DESCENDING: MOST -> LEAST)", ANSI_COLOR_CYAN);
    }
    
    const char *headers[] = {"NO", "STUDENT ID", "FULL NAME", "TEAM", "ROLE", "VIOLATIONS"};
    const int widths[] = {4, 12, 25, 12, 16, 12};
    UI_Table_Header(6, headers, widths, ANSI_COLOR_CYAN);

    const char *teams[] = {"Academic", "Planning", "HR", "Media"};
    const char *roles[] = {"Member", "Leader/Vice", "Ban Chu Nhiem"};

    for (int i = 0; i < size; i++) {
        char noStr[8];
        snprintf(noStr, sizeof(noStr), "%d", i + 1);
        
        char countStr[12];
        snprintf(countStr, sizeof(countStr), "%d", hehe[i].violationCount);
        
        const char *values[] = {
            noStr,
            hehe[i].studentId,
            hehe[i].fullName,
            teams[hehe[i].team],
            roles[hehe[i].role],
            countStr
        };
        UI_Table_Row(6, values, widths, ANSI_COLOR_CYAN);
    }

    UI_Table_End(6, widths, ANSI_COLOR_CYAN);
    UI_Return_Prompt();
    system("cls");
}

void Menu_sort_violations(Member memberList[], int memberCount) {
    UI_Card_Start("SORT MEMBERS BY VIOLATION", ANSI_COLOR_CYAN);
    UI_Menu_Item(1, "ASCENDING (Least -> Most)", ANSI_COLOR_CYAN);
    UI_Menu_Item(2, "DESCENDING (Most -> Least)", ANSI_COLOR_CYAN);
    UI_Divider(50, ANSI_COLOR_CYAN);
    UI_Menu_Item(0, "Exit", ANSI_COLOR_CYAN);
    UI_Card_End(ANSI_COLOR_CYAN);

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