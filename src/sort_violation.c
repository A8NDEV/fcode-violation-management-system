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
        printf(ANSI_COLOR_RESET "\n");
    }
    
    
    view_CLB_Profile(hehe, size); 
}