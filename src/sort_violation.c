#include <stdio.h>
#include "sort_violation.h"
#include "auth.h"



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
    Member hehe[size];

    for (int i = 0; i < size; i++) {
        hehe[i] = member[i];
    }

    shellSortMembers(hehe, size, isAscending);
    
    
    if (isAscending == 1) {
<<<<<<< Updated upstream
        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════════════════╗\n");
        printf("║    DANH SACH TANG DAN (Tu it den nhieu vi pham)    ║\n");
        printf("╚════════════════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");
    } else {
        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════════════════╗\n");
        printf("║    DANH SACH GIAM DAN (Tu nhieu ve it vi pham)     ║\n");
        printf("╚════════════════════════════════════════════════════╝\n");
=======
        printf(ANSI_BRIGHT_YELLOW ANSI_BOLD);
        printf("╔═══════════════════════════════════════════════════════════╗\n");
        printf("║    LIST OF VIOLATIONS (FROM LEAST TO MOST VIOLATIONS)     ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");
    } else {
        printf(ANSI_BRIGHT_YELLOW ANSI_BOLD);
        printf("╔═══════════════════════════════════════════════════════════╗\n");
        printf("║    LIST OF VIOLATIONS (FROM MOST TO LEAST VIOLATIONS)     ║\n");
        printf("╚═══════════════════════════════════════════════════════════╝\n");
>>>>>>> Stashed changes
        printf(ANSI_COLOR_RESET "\n");
    }
    
    printf(ANSI_BRIGHT_BLUE "┌────┬──────────────┬───────────────────────────┬────────────┬──────────────────┬─────────────────┐\n" ANSI_COLOR_RESET);
    printf(ANSI_BRIGHT_BLUE "│" ANSI_BRIGHT_MAGENTA ANSI_BOLD " %-2s " ANSI_BRIGHT_BLUE "│" ANSI_BRIGHT_MAGENTA ANSI_BOLD " %-12s " ANSI_BRIGHT_BLUE "│" ANSI_BRIGHT_MAGENTA ANSI_BOLD " %-25s " ANSI_BRIGHT_BLUE "│" ANSI_BRIGHT_MAGENTA ANSI_BOLD " %-10s " ANSI_BRIGHT_BLUE "│" ANSI_BRIGHT_MAGENTA ANSI_BOLD " %-16s " ANSI_BRIGHT_BLUE "│" ANSI_BRIGHT_MAGENTA ANSI_BOLD " %-15s " ANSI_BRIGHT_BLUE "│\n" ANSI_COLOR_RESET,
           "NO", "STUDENT ID", "FULL NAME", "TEAM", "ROLE", "TOTAL FINE");
    printf(ANSI_BRIGHT_BLUE "├────┼──────────────┼───────────────────────────┼────────────┼──────────────────┼─────────────────┤\n" ANSI_COLOR_RESET);

    const char *teams[] = {"Academic", "Planning", "HR", "Media"};
    const char *roles[] = {"Member", "Leader/Vice", "Ban Chu Nhiem"};

    for (int i = 0; i < size; i++) {
        int teamIdx = (hehe[i].team >= 0 && hehe[i].team <= 3) ? hehe[i].team : 0;
        int roleIdx = (hehe[i].role >= 0 && hehe[i].role <= 2) ? hehe[i].role : 0;
        
        printf(ANSI_BRIGHT_BLUE "│" ANSI_COLOR_RESET " %-2d " ANSI_BRIGHT_BLUE "│" ANSI_COLOR_RESET " %-12s " ANSI_BRIGHT_BLUE "│" ANSI_COLOR_RESET " %-25s " ANSI_BRIGHT_BLUE "│" ANSI_COLOR_RESET " %-10s " ANSI_BRIGHT_BLUE "│" ANSI_COLOR_RESET " %-16s " ANSI_BRIGHT_BLUE "│" ANSI_COLOR_RESET " %-15.0f " ANSI_BRIGHT_BLUE "│\n" ANSI_COLOR_RESET,
               i + 1,
               hehe[i].studentId,
               hehe[i].fullName,
               teams[teamIdx],
               roles[roleIdx],
               hehe[i].totalFine);
               
        if (i < size - 1) {
            printf(ANSI_BRIGHT_BLUE "├────┼──────────────┼───────────────────────────┼────────────┼──────────────────┼─────────────────┤\n" ANSI_COLOR_RESET);
        } else {
            printf(ANSI_BRIGHT_BLUE "└────┴──────────────┴───────────────────────────┴────────────┴──────────────────┴─────────────────┘\n" ANSI_COLOR_RESET);
        }
    } 
    
<<<<<<< Updated upstream
    view_CLB_Profile(hehe, size); 
=======
    if (size == 0) {
        printf(ANSI_BRIGHT_BLUE "└────┴──────────────┴───────────────────────────┴────────────┴──────────────────┴─────────────────┘\n" ANSI_COLOR_RESET);
    }
>>>>>>> Stashed changes
}