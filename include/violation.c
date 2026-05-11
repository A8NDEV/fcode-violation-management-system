#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <violation.h>


void check_to_Paid(Violation violation[], int total_Violations, const char* logged_In_StudentID) {
    double Sum_fine = 0;
    
    for (int i = 0; i < total_Violations; i++) {
        if (strcmp(violation[i].studentId, logged_In_StudentID) == 0 && violation[i].isPaid == 0) {
            if (violation[i].reason == 0) {
                printf("Khong mac ao CLB phat: %.2f\n", violation[i].fine);
                Sum_fine += violation[i].fine;
            } else if (violation[i].reason == 1) {
                printf("Vang hop phat: %.2f\n", violation[i].fine);
                Sum_fine += violation[i].fine;
            } else if (violation[i].reason == 2) {
                printf("Khong tham gia HD phat: %.2f\n", violation[i].fine);
                Sum_fine += violation[i].fine;
            } else {
                printf("Vi pham khac (Bao luc)\n");
            }
        }
    }
    printf("Tong cong tien phat con no: %.2f\n", Sum_fine);
}


void view_Own_Violations(Violation violations[], int total_Violations, const char* logged_In_StudentID) {
    int found = 0;
    printf("\n--- LICH SU VI PHAM CUA BAN (%s) ---\n", logged_In_StudentID);
    printf("%-20s | %-15s | %-12s | %-15s\n", "Ly do", "Ngay vi pham", "Muc phat", "Trang thai");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < total_Violations; i++) {
        if (strcmp(violations[i].studentId, logged_In_StudentID) == 0) {
            found = 1;
            char reasonStr[30];
            switch (violations[i].reason) {
                case 0: strcpy(reasonStr, "Khong mac ao CLB"); break;
                case 1: strcpy(reasonStr, "Vang hop"); break;
                case 2: strcpy(reasonStr, "Khong tham gia HD"); break;
                case 3: strcpy(reasonStr, "Bao luc"); break;
                default: strcpy(reasonStr, "Khong xac dinh"); break;
            }

            char timeStr[20];
            struct tm *timeinfo = localtime(&violations[i].violationTime);
            strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", timeinfo);

            char statusStr[15];
            if (violations[i].isPaid == 1) {
                strcpy(statusStr, "Da thu");
            } else {
                strcpy(statusStr, "Chua thu");
            }

            printf("%-20s | %-15s | %-12.0f | %-15s\n", 
                   reasonStr, timeStr, violations[i].fine, statusStr);
        }
    }

    if (found == 0) {
        printf("Ban khong co vi pham nao.\n");
    }
    printf("-----------------------------------------------------------------------\n");
}

