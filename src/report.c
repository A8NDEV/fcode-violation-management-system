#include "report.h"
#include "auth.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#define MAKE_DIR(p) mkdir(p, 0777)
#endif

void Export_Violation_Report(int memberCount, Member memberList[],
                             int violationCount, Violation violationList[]) {
    UI_Header("EXPORT VIOLATION REPORT", ANSI_COLOR_CYAN);

    MAKE_DIR("data");

    const char *filepath = "data/violation_report.txt";
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf(ANSI_BRIGHT_RED "Error: Could not create output file %s\n" ANSI_COLOR_RESET, filepath);
        printf("\nPress Enter to return...");
        getchar();
        system("cls");
        return;
    }

    time_t rawtime = time(NULL);
    struct tm *timeinfo = localtime(&rawtime);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M:%S", timeinfo);

    fprintf(file, "================================================================================\n");
    fprintf(file, "                       F-CODE CLUB VIOLATION SUMMARY REPORT\n");
    fprintf(file, "================================================================================\n");
    fprintf(file, "Report Generated         : %s\n", timeStr);
    fprintf(file, "Total Active Club Members: %d\n", memberCount);
    fprintf(file, "Total Violations Recorded: %d\n", violationCount);
    fprintf(file, "================================================================================\n\n");

    const char *teams[] = {"Academic", "Planning", "HR", "Media"};
    int teamViolations[4] = {0, 0, 0, 0};
    double teamPaid[4]    = {0.0, 0.0, 0.0, 0.0};
    double teamUnpaid[4]  = {0.0, 0.0, 0.0, 0.0};

    int totalViolations = 0;
    double grandTotalPaid = 0.0;
    double grandTotalUnpaid = 0.0;

    for (int i = 0; i < violationCount; i++) {
        Violation *v = &violationList[i];
        int teamIdx = 0;
        for (int j = 0; j < memberCount; j++) {
            if (strcmp(v->studentId, memberList[j].studentId) == 0) {
                teamIdx = memberList[j].team;
                if (teamIdx < 0 || teamIdx > 3) teamIdx = 0;
                break;
            }
        }
        teamViolations[teamIdx]++;
        totalViolations++;

        if (v->isPaid) {
            teamPaid[teamIdx] += v->fine;
            grandTotalPaid    += v->fine;
        } else {
            teamUnpaid[teamIdx] += v->fine;
            grandTotalUnpaid    += v->fine;
        }
    }

    fprintf(file, "1. STATISTICS BY TEAM\n");
    fprintf(file, "--------------------------------------------------------------------------------\n");
    fprintf(file, "%-15s | %-16s | %-18s | %-18s\n", "Team Name", "Total Violations", "Paid Fines (VND)", "Unpaid Debt (VND)");
    fprintf(file, "--------------------------------------------------------------------------------\n");
    for (int t = 0; t < 4; t++) {
        fprintf(file, "%-15s | %-16d | %-18.0f | %-18.0f\n",
                teams[t], teamViolations[t], teamPaid[t], teamUnpaid[t]);
    }
    fprintf(file, "--------------------------------------------------------------------------------\n");
    fprintf(file, "%-15s | %-16d | %-18.0f | %-18.0f\n",
            "GRAND TOTAL", totalViolations, grandTotalPaid, grandTotalUnpaid);
    fprintf(file, "================================================================================\n\n");

    const char *roles[] = {"Member", "Leader/Vice", "Management Board"};
    fprintf(file, "2. LIST OF MEMBERS WITH UNPAID FINES / DEBT\n");
    fprintf(file, "--------------------------------------------------------------------------------\n");
    fprintf(file, "%-10s | %-22s | %-10s | %-12s | %-15s\n", "Student ID", "Member Name", "Team", "Role", "Unpaid Debt");
    fprintf(file, "--------------------------------------------------------------------------------\n");

    int debtorCount = 0;
    for (int i = 0; i < memberCount; i++) {
        Member *m = &memberList[i];
        if (m->totalFine > 0.0) {
            int tIdx = (m->team >= 0 && m->team <= 3) ? m->team : 0;
            int rIdx = (m->role >= 0 && m->role <= 2) ? m->role : 0;
            fprintf(file, "%-10s | %-22s | %-10s | %-12s | %-15.0f VND\n",
                    m->studentId, m->fullName, teams[tIdx], roles[rIdx], m->totalFine);
            debtorCount++;
        }
    }
    if (debtorCount == 0) {
        fprintf(file, "All members have paid their fines. No outstanding debt!\n");
    }
    fprintf(file, "================================================================================\n\n");

    fprintf(file, "3. MEMBERS AT KICK-OUT RISK (>= 2 Consecutive Absences or Violence Violation)\n");
    fprintf(file, "--------------------------------------------------------------------------------\n");
    fprintf(file, "%-10s | %-22s | %-35s\n", "Student ID", "Member Name", "Risk Reason");
    fprintf(file, "--------------------------------------------------------------------------------\n");

    int riskCount = 0;
    for (int i = 0; i < memberCount; i++) {
        Member *m = &memberList[i];
        int hasViolence = 0;
        for (int j = 0; j < violationCount; j++) {
            if (strcmp(violationList[j].studentId, m->studentId) == 0 && violationList[j].reason == 3) {
                hasViolence = 1;
                break;
            }
        }

        if (m->consecutiveAbsences >= 2 || hasViolence) {
            char reasonBuf[64] = "";
            if (hasViolence && m->consecutiveAbsences >= 2) {
                snprintf(reasonBuf, sizeof(reasonBuf), "Violence & %d Absences", m->consecutiveAbsences);
            } else if (hasViolence) {
                snprintf(reasonBuf, sizeof(reasonBuf), "Violence Violation");
            } else {
                snprintf(reasonBuf, sizeof(reasonBuf), "%d Consecutive Absences", m->consecutiveAbsences);
            }
            fprintf(file, "%-10s | %-22s | %-35s\n", m->studentId, m->fullName, reasonBuf);
            riskCount++;
        }
    }
    if (riskCount == 0) {
        fprintf(file, "No members are currently at kick-out risk.\n");
    }
    fprintf(file, "================================================================================\n");
    fprintf(file, "                             *** END OF REPORT ***\n");
    fprintf(file, "================================================================================\n");

    fclose(file);

    Rainbow_Loading("Exporting Report");
    system("cls");
    UI_Header("EXPORT VIOLATION REPORT", ANSI_COLOR_CYAN);

    UI_Card_Start("EXPORT SUMMARY", ANSI_COLOR_GREEN);
    printf(ANSI_BOLD "    📄 File Path   : " ANSI_COLOR_RESET "data/violation_report.txt\n");
    printf(ANSI_BOLD "    ⏰ Time        : " ANSI_COLOR_RESET "%s\n", timeStr);
    printf(ANSI_BOLD "    👥 Total Mem   : " ANSI_COLOR_RESET "%d\n", memberCount);
    printf(ANSI_BOLD "    💰 Debtors     : " ANSI_COLOR_RESET "%d members\n", debtorCount);
    printf(ANSI_BOLD "    ⚠  At Risk     : " ANSI_COLOR_RESET "%d members\n", riskCount);
    UI_Card_End(ANSI_COLOR_GREEN);

    printf("\n" ANSI_BRIGHT_CYAN "Report successfully exported!" ANSI_COLOR_RESET "\n");
    UI_Return_Prompt();
}
