#include "violation.h"
#include "auth.h"
#include "fileio.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define FINE_MEMBER     20000.0
#define FINE_LEADER     50000.0

static const char *REASON_LABELS[] = {
    "Khong mac ao CLB",
    "Vang hop",
    "Khong tham gia HĐ",
    "Bao luc"
};

static const char *TEAM_LABELS[] = {
    "Academic", "Planning", "HR", "Media"
};

static const char *PAID_LABELS[] = {
    "Chua thu", "Da thu"
};


/* Tra ve muc phat tuong ung voi chuc vu va ly do vi pham */
static double Get_fine(int role, int reason) {
    if (reason == 3) return 0.0; /* Bao luc = 0đ */
    return (role == 0) ? FINE_MEMBER : FINE_LEADER;
}

/* In tieu de bang vi pham */
static const int V_WIDTHS[] = {3, 10, 20, 8, 20, 10, 8, 8};
static const char *V_HEADERS[] = {"NO", "ID", "NAME", "TEAM", "REASON", "DATE", "FINE", "STATUS"};

static void Print_violation_table_header(void) {
    UI_Table_Header(8, V_HEADERS, V_WIDTHS, ANSI_COLOR_CYAN);
}

/* In mot dong vi pham (can biet ten member) */
static void Print_violation_row(int stt, const Violation *v,
                                const char *fullName, const char *team) {
    char timeStr[13], sttStr[8], fineStr[16];
    struct tm *ti = localtime(&v->violationTime);
    strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", ti);
    snprintf(sttStr, sizeof(sttStr), "%d", stt);
    snprintf(fineStr, sizeof(fineStr), "%.0f", v->fine);

    const char *values[] = {
        sttStr, v->studentId, fullName, team, 
        REASON_LABELS[v->reason], timeStr, fineStr, PAID_LABELS[v->isPaid]
    };
    UI_Table_Row(8, values, V_WIDTHS, ANSI_COLOR_CYAN);
}


void check_to_Paid(Violation violation[], int total_Violations,
                   const char *logged_In_StudentID) {
    double sum_fine = 0.0;
    int found = 0;

    UI_Header("OUTSTANDING DEBT", ANSI_COLOR_YELLOW);
    const char *headers[] = {"VIOLATION REASON", "FINE", "STATUS"};
    const int widths[] = {30, 12, 12};
    UI_Table_Header(3, headers, widths, ANSI_COLOR_YELLOW);

    for (int i = 0; i < total_Violations; i++) {
        if (strcmp(violation[i].studentId, logged_In_StudentID) == 0
            && violation[i].isPaid == 0) {
            found = 1;
            char fineStr[16];
            snprintf(fineStr, sizeof(fineStr), "%.0f", violation[i].fine);
            const char *values[] = {
                REASON_LABELS[violation[i].reason],
                fineStr,
                PAID_LABELS[0]
            };
            UI_Table_Row(3, values, widths, ANSI_COLOR_YELLOW);
            sum_fine += violation[i].fine;
        }
    }
    UI_Table_End(3, widths, ANSI_COLOR_YELLOW);

    if (!found) {
        printf(ANSI_BRIGHT_GREEN "YOU HAVE NO OUTSTANDING FINES.\n" ANSI_COLOR_RESET);
        return;
    }
    printf(ANSI_BRIGHT_YELLOW "\nTOTAL AMOUNT DUE: %.0f VND\n" ANSI_COLOR_RESET,
           sum_fine);
    UI_Return_Prompt();
}

void view_Own_Violations(Violation violations[], int total_Violations,
                         const char *logged_In_StudentID) {
    int found = 0;
    UI_Header("VIOLATION HISTORY", ANSI_COLOR_YELLOW);
    printf("  " ANSI_BOLD ANSI_COLOR_WHITE "Student ID: " ANSI_COLOR_RESET "%s\n\n", logged_In_StudentID);
    
    const char *headers[] = {"REASON", "DATE", "FINE", "STATUS"};
    const int widths[] = {30, 12, 12, 10};
    UI_Table_Header(4, headers, widths, ANSI_COLOR_YELLOW);

    for (int i = 0; i < total_Violations; i++) {
        if (strcmp(violations[i].studentId, logged_In_StudentID) != 0)
            continue;
        found = 1;

        char timeStr[13];
        struct tm *ti = localtime(&violations[i].violationTime);
        strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", ti);

        char fineStr[16];
        snprintf(fineStr, sizeof(fineStr), "%.0f", violations[i].fine);

        const char *values[] = {
            REASON_LABELS[violations[i].reason],
            timeStr,
            fineStr,
            PAID_LABELS[violations[i].isPaid]
        };
        UI_Table_Row(4, values, widths, ANSI_COLOR_YELLOW);
    }
    UI_Table_End(4, widths, ANSI_COLOR_YELLOW);

    if (!found)
        printf("YOU HAVE NO VIOLATIONS.\n");
    UI_Return_Prompt();
}


void Menu_record_violation(int *memberCount, Member memberList[],
                           int *violationCount, Violation violationList[]) {
    UI_Header("RECORD VIOLATION", ANSI_COLOR_RED);

    char studentId[SHORT_SIZE];
    Input_studentId(studentId);

    int memberIdx = Find_studentId(*memberCount, memberList, studentId);
    if (memberIdx == -1) {
        printf(ANSI_BRIGHT_RED "MEMBER NOT FOUND WITH STUDENT ID %s!\n"
               ANSI_COLOR_RESET, studentId);
        Sleep(2000);
        system("cls");
        return;
    }

    Member *m = &memberList[memberIdx];
    printf(ANSI_BRIGHT_GREEN ">> Member: %s (%s)\n" ANSI_COLOR_RESET,
           m->fullName, TEAM_LABELS[m->team]);

    /* Buoc 2: Chon ly do vi pham */
    printf("\n" ANSI_BOLD "Select violation reason:\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [0] " ANSI_COLOR_RESET "Khong mac ao CLB              (+%.0f VND)\n", Get_fine(m->role, 0));
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Vang hop                      (+%.0f VND)\n", Get_fine(m->role, 1));
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "Khong tham gia hoat dong      (+%.0f VND)\n", Get_fine(m->role, 2));
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "Bao luc                       (+%.0f VND)\n", Get_fine(m->role, 3));
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);

    int reason;
    Input_violation_reason(&reason);

    /* Buoc 3: Xu ly truong hop bao luc hoac vang hop */
    if (reason == 3 || reason == 1) {
        if (reason == 3) {
            printf(ANSI_BRIGHT_RED ANSI_BOLD
                   "\n!!! WARNING: VIOLENCE VIOLATION !!!\n"
                   "This member may be kicked out of the Club.\n"
                   "BCN must handle this manually via Delete Member.\n"
                   ANSI_COLOR_RESET);
        } else if (reason == 1) {
            int nextAbsences = m->consecutiveAbsences + 1;
            if (nextAbsences == 2 || nextAbsences == 3) {
                printf(ANSI_BRIGHT_YELLOW ANSI_BOLD
                       "\n!!! WARNING: CONSECUTIVE ABSENCES !!!\n"
                       "Member %s will have %d consecutive absences.\n"
                       ANSI_COLOR_RESET, m->fullName, nextAbsences);
            } else if (nextAbsences > 3) {
                printf(ANSI_BRIGHT_RED ANSI_BOLD
                       "\n!!! WARNING: OUT CLB THRESHOLD REACHED !!!\n"
                       "Member %s will have %d consecutive absences (> 3).\n"
                       "BCN must handle this manually via Delete Member.\n"
                       ANSI_COLOR_RESET, m->fullName, nextAbsences);
            }
        }

        int requireConfirm = (reason == 3) || (reason == 1 && (m->consecutiveAbsences + 1) >= 2);
        if (requireConfirm) {
            printf("Do you want to continue recording this violation?\n");
            printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "Cancel\n");
            printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Confirm Record\n");
            printf("\n");
            printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);
            int confirm = -1;
            Input_user_choose(&confirm);
            if (confirm != 1) {
                printf("Recording cancelled.\n");
                Sleep(1500);
                system("cls");
                return;
            }
        }
    }

    /* Buoc 4: Tao ban ghi vi pham moi */
    Violation newV = Init_Violation();
    strcpy(newV.studentId, studentId);
    newV.reason        = reason;
    newV.fine          = Get_fine(m->role, reason);
    newV.isPaid        = 0;
    newV.violationTime = time(NULL);

    /* Buoc 5: Cap nhat member */
    m->violationCount++;
    m->totalFine += newV.fine;
    if (reason == 1) {
        m->consecutiveAbsences++;
    }

    /* Buoc 6: Luu vao file */
    if (!Append_violations_dat(newV)) {
        Announcement_error_acction();
        /* Rollback in-memory */
        m->violationCount--;
        m->totalFine -= newV.fine;
        if (reason == 1) {
            m->consecutiveAbsences--;
        }
        return;
    }
    if (!Update_members_dat(memberIdx, *m)) {
        Announcement_error_acction();
        return;
    }

    /* Cap nhat mang trong bo nho */
    violationList[*violationCount] = newV;
    (*violationCount)++;

    printf(ANSI_BRIGHT_GREEN
           "\nRecorded successfully! Fine amount: %.0f VND\n"
           ANSI_COLOR_RESET, newV.fine);
    Sleep(2000);
    system("cls");
}


void Menu_mark_paid(int *memberCount, Member memberList[],
                    int *violationCount, Violation violationList[]) {
    UI_Header("MARK AS PAID", ANSI_COLOR_GREEN);

    /* Buoc 1: Chon thanh vien */
    char studentId[SHORT_SIZE];
    Input_studentId(studentId);

    int memberIdx = Find_studentId(*memberCount, memberList, studentId);
    if (memberIdx == -1) {
        printf(ANSI_BRIGHT_RED "MEMBER NOT FOUND WITH STUDENT ID %s!\n"
               ANSI_COLOR_RESET, studentId);
        Sleep(2000);
        system("cls");
        return;
    }

    Member *m = &memberList[memberIdx];

    while (1) {
        /* Buoc 2: Thu thap danh sach vi pham chua thu */
        int unpaidIdx[MAX_ACCOUNT * 10]; /* global index trong violationList */
        int unpaidCount = 0;

        for (int i = 0; i < *violationCount; i++) {
            if (strcmp(violationList[i].studentId, studentId) == 0
                && violationList[i].isPaid == 0) {
                unpaidIdx[unpaidCount++] = i;
            }
        }

        /* Buoc 3: Kiem tra co vi pham chua thu khong */
        if (unpaidCount == 0) {
            printf(ANSI_BRIGHT_GREEN
                   "\nNO UNPAID VIOLATIONS FOR MEMBER %s.\n"
                   ANSI_COLOR_RESET, m->fullName);
            Sleep(2000);
            system("cls");
            return;
        }

        /* Buoc 4: Hien thi danh sach vi pham chua thu */
        printf(ANSI_BOLD "\n  Unpaid violations for " ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET ":\n", m->fullName);
        const char *headers[] = {"NO", "REASON", "DATE", "FINE"};
        const int widths[] = {4, 30, 12, 12};
        UI_Table_Header(4, headers, widths, ANSI_COLOR_CYAN);

        for (int i = 0; i < unpaidCount; i++) {
            Violation *v = &violationList[unpaidIdx[i]];
            char timeStr[13], sttStr[8], fineStr[16];
            struct tm *ti = localtime(&v->violationTime);
            strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", ti);
            snprintf(sttStr, sizeof(sttStr), "%d", i + 1);
            snprintf(fineStr, sizeof(fineStr), "%.0f", v->fine);

            const char *values[] = {sttStr, REASON_LABELS[v->reason], timeStr, fineStr};
            UI_Table_Row(4, values, widths, ANSI_COLOR_CYAN);
        }
        UI_Table_End(4, widths, ANSI_COLOR_CYAN);
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "Exit\n");
        printf("\n");
        printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);

        /* Buoc 5: Chon vi pham can danh dau */
        int choice;
        Input_user_choose(&choice);
        if (choice == 0) {
            system("cls");
            return;
        }
        if (choice < 1 || choice > unpaidCount) {
            Announcement_unaivailable_option();
            continue;
        }

        /* Buoc 6: Cap nhat isPaid va totalFine */
        int gIdx = unpaidIdx[choice - 1];
        Violation *target = &violationList[gIdx];

        target->isPaid = 1;
        m->totalFine  -= target->fine;
        if (m->totalFine < 0.0) m->totalFine = 0.0;

        /* Buoc 7: Luu vao file */
        if (!Update_violations_dat(gIdx, *target)) {
            /* Rollback */
            target->isPaid = 0;
            m->totalFine  += target->fine;
            Announcement_error_acction();
            continue;
        }
        if (!Update_members_dat(memberIdx, *m)) {
            Announcement_error_acction();
            continue;
        }

        printf(ANSI_BRIGHT_GREEN
               "Marked violation \"%s\" as PAID!\n"
               ANSI_COLOR_RESET, REASON_LABELS[target->reason]);
        Sleep(1500);
        system("cls");
    }
}

void Menu_view_all_violations(int memberCount, Member memberList[],
                              int violationCount, Violation violationList[]) {
    UI_Header("ALL CLUB VIOLATIONS", ANSI_COLOR_CYAN);

    /* Buoc 1: Chon bo loc */
    printf(ANSI_BOLD "Select filter type:\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [0] " ANSI_COLOR_RESET "All (No filter)\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Filter by Team\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "Filter by Violation Reason\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "Filter by Payment Status\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [4] " ANSI_COLOR_RESET "Filter by Date Range\n");
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);

    int filterBy;
    Input_user_choose(&filterBy);

    int filterTeam   = -1;
    int filterReason = -1;
    int filterPaid   = -1;
    time_t filterDateFrom = (time_t)(-1);
    time_t filterDateTo   = (time_t)(-1);

    if (filterBy == 1) {
        Input_team(&filterTeam);
    } else if (filterBy == 2) {
        printf("Select reason: [0] Khong mac ao  [1] Vang hop  [2] Khong HD  [3] Bao luc\n");
        Input_violation_reason(&filterReason);
    } else if (filterBy == 3) {
        printf("Select status: [0] Unpaid  [1] Paid\n");
        int paid = -1;
        Input_user_choose(&paid);
        if (paid == 0 || paid == 1)
            filterPaid = paid;
    } else if (filterBy == 4) {
        filterDateFrom = Input_date("Start date");
        /* Normalize end date to 23:59:59 of that day */
        filterDateTo = Input_date("End date  ");
        if (filterDateTo != (time_t)(-1)) filterDateTo += 86399;
    } else if (filterBy != 0) {
        Announcement_unaivailable_option();
        return;
    }

    /* Buoc 2: In bang vi pham */
    Print_violation_table_header();

    int displayCount = 0;
    for (int i = 0; i < violationCount; i++) {
        Violation *v = &violationList[i];

        /* Tim thong tin member */
        int mIdx = Find_studentId(memberCount, memberList, v->studentId);
        const char *name = (mIdx != -1) ? memberList[mIdx].fullName : "???";
        int team = (mIdx != -1) ? memberList[mIdx].team : -1;
        const char *teamStr = (team >= 0 && team <= 3)
            ? TEAM_LABELS[team] : "???";

        /* Ap dung bo loc */
        if (filterTeam     != -1 && team != filterTeam)              continue;
        if (filterReason   != -1 && v->reason != filterReason)       continue;
        if (filterPaid     != -1 && v->isPaid != filterPaid)         continue;
        if (filterDateFrom != (time_t)(-1) && v->violationTime < filterDateFrom) continue;
        if (filterDateTo   != (time_t)(-1) && v->violationTime > filterDateTo)   continue;

        displayCount++;
        Print_violation_row(displayCount, v, name, teamStr);
    }
    UI_Table_End(8, V_WIDTHS, ANSI_COLOR_CYAN);

    /* Buoc 3: In tong ket */
    printf(ANSI_BRIGHT_YELLOW
           "\nTotal displayed violations: %d\n"
           ANSI_COLOR_RESET, displayCount);
    if (displayCount == 0)
        printf("No violations match the conditions.\n");
    UI_Return_Prompt();
}

void view_Statistics_By_Team(Member memberList[], int memberCount,
                             Violation violationList[], int violationCount) {
    UI_Header("VIOLATION STATISTICS", ANSI_COLOR_MAGENTA);

    double totalFineTeam[4] = {0, 0, 0, 0};
    int totalViolationsTeam[4] = {0, 0, 0, 0};
    int membersInTeam[4] = {0, 0, 0, 0};

    /* Dem so thanh vien moi ban */
    for (int i = 0; i < memberCount; i++) {
        if (memberList[i].team >= 0 && memberList[i].team < 4) {
            membersInTeam[memberList[i].team]++;
        }
    }

    /* Tinh tong tien va so vi pham moi ban */
    for (int i = 0; i < violationCount; i++) {
        int mIdx = Find_studentId(memberCount, memberList, violationList[i].studentId);
        if (mIdx != -1) {
            int team = memberList[mIdx].team;
            if (team >= 0 && team < 4) {
                totalViolationsTeam[team]++;
                totalFineTeam[team] += violationList[i].fine;
            }
        }
    }

    /* In ket qua */
    const char *statHeaders[] = {"TEAM NAME", "MEMBERS", "VIOLATIONS", "TOTAL FINE"};
    const int statWidths[] = {12, 10, 12, 15};
    UI_Table_Header(4, statHeaders, statWidths, ANSI_COLOR_MAGENTA);

    double grandTotalFine = 0;
    int grandTotalViolations = 0;

    for (int i = 0; i < 4; i++) {
        char memStr[8], violStr[8], fineStr[20];
        snprintf(memStr, sizeof(memStr), "%d", membersInTeam[i]);
        snprintf(violStr, sizeof(violStr), "%d", totalViolationsTeam[i]);
        snprintf(fineStr, sizeof(fineStr), "%.0f VND", totalFineTeam[i]);

        const char *values[] = {TEAM_LABELS[i], memStr, violStr, fineStr};
        UI_Table_Row(4, values, statWidths, ANSI_COLOR_MAGENTA);
        
        // Integrated Trend Bar within table borders
        printf("  " ANSI_COLOR_MAGENTA "│" ANSI_COLOR_RESET "      " ANSI_BRIGHT_BLACK "↳ Trend: " ANSI_COLOR_MAGENTA);
        int barWidth = (totalViolationsTeam[i] > 20) ? 20 : totalViolationsTeam[i];
        for(int b=0; b<barWidth; b++) printf("█");
        
        // Calculate remaining spaces to close the table border correctly
        // Total width of internal table is sum(statWidths) + 2*num_cols - 1 (approximately)
        // Let's just pad it manually to match the 4-column layout
        int current_pos = 6 + 9 + barWidth; // spaces + "↳ Trend: " + bars
        int total_table_inner_width = 0;
        for(int w=0; w<4; w++) total_table_inner_width += (statWidths[w] + 2);
        total_table_inner_width += 3; // for separators

        for(int s=0; s < (total_table_inner_width - current_pos - 1); s++) printf(" ");
        printf(ANSI_COLOR_MAGENTA "│" ANSI_COLOR_RESET "\n");

        grandTotalFine += totalFineTeam[i];
        grandTotalViolations += totalViolationsTeam[i];
    }
    
    char gViolStr[16], gFineStr[32];
    snprintf(gViolStr, sizeof(gViolStr), "%d", grandTotalViolations);
    snprintf(gFineStr, sizeof(gFineStr), "%.0f VND", grandTotalFine);
    const char *totals[] = {"GRAND TOTAL", "", gViolStr, gFineStr};
    UI_Table_Row(4, totals, statWidths, ANSI_COLOR_MAGENTA);
    UI_Table_End(4, statWidths, ANSI_COLOR_MAGENTA);
    UI_Return_Prompt();
}

/* -------------------------------------------------------
 * BCN: Diem danh tu dong qua file (Batch process attendance)
 * ------------------------------------------------------- */
void Menu_batch_attendance(int memberCount, Member memberList[],
                           int *violationCount, Violation violationList[]) {
    UI_Header("ATTENDANCE PROCESSOR", ANSI_COLOR_CYAN);

    FILE *file = fopen("data/attendance.txt", "r");
    if (!file) {
        printf(ANSI_BRIGHT_RED "Error: Could not open data/attendance.txt\n"
               "Please ensure the attendance file exists.\n" ANSI_COLOR_RESET);
        Sleep(2500);
        system("cls");
        return;
    }

    char attended[MAX_ACCOUNT][SHORT_SIZE];
    int attCount = 0;
    char line[64];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) > 0 && attCount < MAX_ACCOUNT) {
            strcpy(attended[attCount++], line);
        }
    }
    fclose(file);

    int presentCount = 0;
    int absentCount  = 0;
    int dangerCount  = 0;
    char dangerList[MAX_ACCOUNT][100];

    time_t currentTime = time(NULL);

    for (int i = 0; i < memberCount; i++) {
        Member *m = &memberList[i];
        int isPresent = 0;

        for (int j = 0; j < attCount; j++) {
            if (strcmp(m->studentId, attended[j]) == 0) {
                isPresent = 1;
                break;
            }
        }

        if (isPresent) {
            m->consecutiveAbsences = 0;
            presentCount++;
        } else {
            m->consecutiveAbsences++;
            absentCount++;

            /* Record violation */
            Violation newV = Init_Violation();
            strcpy(newV.studentId, m->studentId);
            newV.reason        = 1; /* Vang hop */
            newV.fine          = Get_fine(m->role, 1);
            newV.isPaid        = 0;
            newV.violationTime = currentTime;

            m->violationCount++;
            m->totalFine += newV.fine;

            violationList[*violationCount] = newV;
            (*violationCount)++;

            if (m->consecutiveAbsences >= 2) {
                snprintf(dangerList[dangerCount++], sizeof(dangerList[0]), 
                         "%-10s | %-20s | %d absences", m->studentId, m->fullName, m->consecutiveAbsences);
            }
        }
    }

    /* Save to files */
    if (!Rewrite_members_dat(memberCount, memberList) || 
        !Rewrite_violations_dat(*violationCount, violationList)) {
        Announcement_error_acction();
        return;
    }

    UI_Card_Start("ATTENDANCE SUMMARY", ANSI_COLOR_CYAN);
    printf(ANSI_BOLD "    👥 Total Members   : " ANSI_COLOR_RESET "%d\n", memberCount);
    printf(ANSI_BOLD "    ✅ Present Count   : " ANSI_COLOR_RESET "%d\n", presentCount);
    printf(ANSI_BOLD "    ❌ Absent Count    : " ANSI_COLOR_RESET "%d\n", absentCount);
    UI_Card_End(ANSI_COLOR_CYAN);

    if (dangerCount > 0) {
        printf(ANSI_BRIGHT_RED "\n  [!] KICK-OUT RISK DETECTED (%d members):\n" ANSI_COLOR_RESET, dangerCount);
        for (int i = 0; i < dangerCount; i++) {
            printf("      " ANSI_BRIGHT_RED "❯" ANSI_COLOR_RESET " %s\n", dangerList[i]);
        }
    } else {
        printf("\n" ANSI_BRIGHT_GREEN "  ✔ All members are in good standing." ANSI_COLOR_RESET "\n");
    }

    UI_Return_Prompt();
}
