#include <stdio.h>
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
    Member hehe[size];

    for (int i = 0; i < size; i++) {
        hehe[i] = member[i];
    }

    shellSortMembers(hehe, size, isAscending);
    
    
    if (isAscending == 1) {
        UI_Header("VIOLATIONS: LEAST TO MOST", ANSI_COLOR_CYAN);
    } else {
        UI_Header("VIOLATIONS: MOST TO LEAST", ANSI_COLOR_CYAN);
    }
    
    view_CLB_Profile(hehe, size); 
}