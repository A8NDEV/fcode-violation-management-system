#include "violation.h"
#include "auth.h"
#include "fileio.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

/* -------------------------------------------------------
 * Constants
 * ------------------------------------------------------- */
#define FINE_NO_SHIRT   20000.0
#define FINE_ABSENT     50000.0
#define FINE_NO_ACTIVITY 30000.0
#define FINE_VIOLENCE   200000.0

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

/* -------------------------------------------------------
 * Static helpers
 * ------------------------------------------------------- */

/* Tra ve muc phat tuong ung voi ly do vi pham */
static double Get_fine_by_reason(int reason) {
    switch (reason) {
        case 0: return FINE_NO_SHIRT;
        case 1: return FINE_ABSENT;
        case 2: return FINE_NO_ACTIVITY;
        case 3: return FINE_VIOLENCE;
        default: return 0.0;
    }
}

/* In tieu de bang vi pham */
static void Print_violation_table_header(void) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("\n%-4s %-12s %-25s %-10s %-20s %-12s %-12s %-10s\n",
           "STT", "MSSV", "Ho va ten", "Ban", "Ly do",
           "Ngay", "Tien phat", "Trang thai");
    printf("%-4s %-12s %-25s %-10s %-20s %-12s %-12s %-10s\n",
           "---", "----", "---------", "---", "-----",
           "----", "---------", "----------");
    printf(ANSI_COLOR_RESET);
}

/* In mot dong vi pham (can biet ten member) */
static void Print_violation_row(int stt, const Violation *v,
                                const char *fullName, const char *team) {
    char timeStr[13];
    struct tm *ti = localtime(&v->violationTime);
    strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", ti);

    const char *paidColor = (v->isPaid == 1)
        ? ANSI_BRIGHT_GREEN : ANSI_BRIGHT_RED;

    printf("%-4d %-12s %-25s %-10s %-20s %-12s %-12.0f %s%-10s%s\n",
           stt,
           v->studentId,
           fullName,
           team,
           REASON_LABELS[v->reason],
           timeStr,
           v->fine,
           paidColor, PAID_LABELS[v->isPaid], ANSI_COLOR_RESET);
}

/* -------------------------------------------------------
 * Member-facing functions (giu lai tu include/violation.c)
 * ------------------------------------------------------- */

void check_to_Paid(Violation violation[], int total_Violations,
                   const char *logged_In_StudentID) {
    double sum_fine = 0.0;
    int found = 0;

    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("\n%-20s | %-12s | %-12s\n",
           "Ly do vi pham", "Tien phat", "Trang thai");
    printf("%-20s | %-12s | %-12s\n",
           "--------------------", "----------", "----------");
    printf(ANSI_COLOR_RESET);

    for (int i = 0; i < total_Violations; i++) {
        if (strcmp(violation[i].studentId, logged_In_StudentID) == 0
            && violation[i].isPaid == 0) {
            found = 1;
            printf("%-20s | %-12.0f | %-12s\n",
                   REASON_LABELS[violation[i].reason],
                   violation[i].fine,
                   PAID_LABELS[0]);
            sum_fine += violation[i].fine;
        }
    }

    if (!found) {
        printf(ANSI_BRIGHT_GREEN "Ban khong co khoan no nao.\n" ANSI_COLOR_RESET);
        return;
    }
    printf(ANSI_BRIGHT_YELLOW "\nTONG TIEN CON NO: %.0f VND\n" ANSI_COLOR_RESET,
           sum_fine);
}

void view_Own_Violations(Violation violations[], int total_Violations,
                         const char *logged_In_StudentID) {
    int found = 0;

    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("\n--- LICH SU VI PHAM (%s) ---\n", logged_In_StudentID);
    printf("%-20s | %-12s | %-12s | %-10s\n",
           "Ly do", "Ngay vi pham", "Tien phat", "Trang thai");
    printf("%-20s | %-12s | %-12s | %-10s\n",
           "--------------------", "------------", "----------", "----------");
    printf(ANSI_COLOR_RESET);

    for (int i = 0; i < total_Violations; i++) {
        if (strcmp(violations[i].studentId, logged_In_StudentID) != 0)
            continue;
        found = 1;

        char timeStr[13];
        struct tm *ti = localtime(&violations[i].violationTime);
        strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", ti);

        const char *paidColor = (violations[i].isPaid == 1)
            ? ANSI_BRIGHT_GREEN : ANSI_BRIGHT_RED;

        printf("%-20s | %-12s | %-12.0f | %s%-10s%s\n",
               REASON_LABELS[violations[i].reason],
               timeStr,
               violations[i].fine,
               paidColor, PAID_LABELS[violations[i].isPaid], ANSI_COLOR_RESET);
    }

    if (!found)
        printf("Ban khong co vi pham nao.\n");
}

/* -------------------------------------------------------
 * BCN: Ghi nhan vi pham moi
 * ------------------------------------------------------- */

void Menu_record_violation(int *memberCount, Member memberList[],
                           int *violationCount, Violation violationList[]) {
    /* Buoc 1: Chon thanh vien */
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║            GHI NHAN VI PHAM            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

    char studentId[SHORT_SIZE];
    Input_studentId(studentId);

    int memberIdx = Find_studentId(*memberCount, memberList, studentId);
    if (memberIdx == -1) {
        printf(ANSI_BRIGHT_RED "Khong tim thay thanh vien voi MSSV %s!\n"
               ANSI_COLOR_RESET, studentId);
        Sleep(2000);
        system("cls");
        return;
    }

    Member *m = &memberList[memberIdx];
    printf(ANSI_BRIGHT_GREEN ">> Thanh vien: %s (%s)\n" ANSI_COLOR_RESET,
           m->fullName, TEAM_LABELS[m->team]);

    /* Buoc 2: Chon ly do vi pham */
    printf("\n" ANSI_BOLD "Chon ly do vi pham:\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [0] " ANSI_COLOR_RESET "Khong mac ao CLB    (+%.0f VND)\n", FINE_NO_SHIRT);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Vang hop            (+%.0f VND)\n", FINE_ABSENT);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "Khong tham gia HĐ   (+%.0f VND)\n", FINE_NO_ACTIVITY);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "Bao luc             (+%.0f VND)\n", FINE_VIOLENCE);

    int reason;
    Input_violation_reason(&reason);

    /* Buoc 3: Xu ly truong hop bao luc */
    if (reason == 3) {
        printf(ANSI_BRIGHT_RED ANSI_BOLD
               "\n!!! CANH BAO: VI PHAM BAO LUC !!!\n"
               "Thanh vien nay co the bi Out CLB.\n"
               "BCN can xu ly thu cong qua chuc nang Xoa thanh vien.\n"
               ANSI_COLOR_RESET);
        printf("Ban co muon tiep tuc ghi nhan vi pham nay?\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "Huy bo\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Xac nhan ghi nhan\n");
        int confirm = -1;
        Input_user_choose(&confirm);
        if (confirm != 1) {
            printf("Da huy ghi nhan.\n");
            Sleep(1500);
            system("cls");
            return;
        }
    }

    /* Buoc 4: Tao ban ghi vi pham moi */
    Violation newV = Init_Violation();
    strcpy(newV.studentId, studentId);
    newV.reason        = reason;
    newV.fine          = Get_fine_by_reason(reason);
    newV.isPaid        = 0;
    newV.violationTime = time(NULL);

    /* Buoc 5: Cap nhat member */
    m->violationCount++;
    m->totalFine += newV.fine;

    /* Buoc 6: Luu vao file */
    if (!Append_violations_dat(newV)) {
        Announcement_error_acction();
        /* Rollback in-memory */
        m->violationCount--;
        m->totalFine -= newV.fine;
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
           "\nGhi nhan thanh cong! Muc phat: %.0f VND\n"
           ANSI_COLOR_RESET, newV.fine);
    Sleep(2000);
    system("cls");
}

/* -------------------------------------------------------
 * BCN: Danh dau da thu tien phat
 * ------------------------------------------------------- */

void Menu_mark_paid(int *memberCount, Member memberList[],
                    int *violationCount, Violation violationList[]) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║          DANH DAU DA THU TIEN          ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

    /* Buoc 1: Chon thanh vien */
    char studentId[SHORT_SIZE];
    Input_studentId(studentId);

    int memberIdx = Find_studentId(*memberCount, memberList, studentId);
    if (memberIdx == -1) {
        printf(ANSI_BRIGHT_RED "Khong tim thay thanh vien voi MSSV %s!\n"
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
                   "\nKhong con vi pham chua thu cho thanh vien %s.\n"
                   ANSI_COLOR_RESET, m->fullName);
            Sleep(2000);
            system("cls");
            return;
        }

        /* Buoc 4: Hien thi danh sach vi pham chua thu */
        printf(ANSI_BOLD "\nDanh sach vi pham chua thu cua %s:\n"
               ANSI_COLOR_RESET, m->fullName);
        printf(ANSI_COLOR_CYAN "%-4s %-20s %-12s %-12s\n",
               "STT", "Ly do", "Ngay", "Tien phat");
        printf("%-4s %-20s %-12s %-12s\n" ANSI_COLOR_RESET,
               "---", "--------------------", "----------", "----------");

        for (int i = 0; i < unpaidCount; i++) {
            Violation *v = &violationList[unpaidIdx[i]];
            char timeStr[13];
            struct tm *ti = localtime(&v->violationTime);
            strftime(timeStr, sizeof(timeStr), "%d/%m/%Y", ti);
            printf("%-4d %-20s %-12s %-12.0f\n",
                   i + 1,
                   REASON_LABELS[v->reason],
                   timeStr,
                   v->fine);
        }
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "Thoat\n");

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
               "Da danh dau vi pham \"%s\" la DA THU!\n"
               ANSI_COLOR_RESET, REASON_LABELS[target->reason]);
        Sleep(1500);
        system("cls");
    }
}

/* -------------------------------------------------------
 * BCN: Xem danh sach vi pham toan CLB (co bo loc)
 * ------------------------------------------------------- */

void Menu_view_all_violations(int memberCount, Member memberList[],
                              int violationCount, Violation violationList[]) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║       DANH SACH VI PHAM TOAN CLB       ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

    /* Buoc 1: Chon bo loc */
    printf(ANSI_BOLD "Chon kieu loc:\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [0] " ANSI_COLOR_RESET "Tat ca (khong loc)\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Loc theo ban (team)\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "Loc theo ly do vi pham\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "Loc theo trang thai thu tien\n");

    int filterBy;
    Input_user_choose(&filterBy);

    int filterTeam   = -1;
    int filterReason = -1;
    int filterPaid   = -1;

    if (filterBy == 1) {
        Input_team(&filterTeam);
    } else if (filterBy == 2) {
        printf("Chon ly do: [0] Khong mac ao  [1] Vang hop  [2] Khong HD  [3] Bao luc\n");
        Input_violation_reason(&filterReason);
    } else if (filterBy == 3) {
        printf("Chon trang thai: [0] Chua thu  [1] Da thu\n");
        int paid = -1;
        Input_user_choose(&paid);
        if (paid == 0 || paid == 1)
            filterPaid = paid;
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
        if (filterTeam   != -1 && team != filterTeam)      continue;
        if (filterReason != -1 && v->reason != filterReason) continue;
        if (filterPaid   != -1 && v->isPaid != filterPaid)  continue;

        displayCount++;
        Print_violation_row(displayCount, v, name, teamStr);
    }

    /* Buoc 3: In tong ket */
    printf(ANSI_BRIGHT_YELLOW
           "\nTong so vi pham hien thi: %d\n"
           ANSI_COLOR_RESET, displayCount);
    if (displayCount == 0)
        printf("Khong co vi pham nao thoa dieu kien.\n");

    printf("\nNhan Enter de quay lai...");
    getchar();
    system("cls");
}

/* -------------------------------------------------------
 * BCN: Thong ke vi pham theo ban (Team)
 * ------------------------------------------------------- */
void view_Statistics_By_Team(Member memberList[], int memberCount,
                             Violation violationList[], int violationCount) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║       THONG KE VI PHAM THEO BAN        ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

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
    printf(ANSI_BOLD "%-12s | %-12s | %-12s | %-15s\n",
           "Ten Ban", "TV Co Loi", "Tong Vi Pham", "Tong Tien Phat");
    printf("%-12s | %-12s | %-12s | %-15s\n" ANSI_COLOR_RESET,
           "------------", "------------", "------------", "---------------");

    double grandTotalFine = 0;
    int grandTotalViolations = 0;

    for (int i = 0; i < 4; i++) {
        printf("%-12s | %-12d | %-12d | %-15.0f VND\n",
               TEAM_LABELS[i], membersInTeam[i], totalViolationsTeam[i], totalFineTeam[i]);
        grandTotalFine += totalFineTeam[i];
        grandTotalViolations += totalViolationsTeam[i];
    }

    printf(ANSI_COLOR_YELLOW "------------------------------------------------------------\n");
    printf("%-12s | %-12s | %-12d | %-15.0f VND\n",
           "TONG CONG", "", grandTotalViolations, grandTotalFine);
    printf(ANSI_COLOR_RESET);

    printf("\nNhan Enter de quay lai...");
    getchar();
    system("cls");
}
