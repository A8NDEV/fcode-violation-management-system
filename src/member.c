#include "member.h"
#include "types.h"
#include "utils.h"
#include "validate.h"
#include "fileio.h"
#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static int create_member(int *member_size, Member member_list[], Member new_member, int *account_size, Account account_list[], Account new_account) {
    if (*member_size >= MAX_ACCOUNT || *account_size >= MAX_ACCOUNT) return 0;
    member_list[*member_size] = new_member;
    account_list[*account_size] = new_account;
    if (Append_members_dat(new_member) == 0 || Append_accounts_dat(new_account) == 0) return 0;
    ++(*member_size);
    ++(*account_size);
    return 1;
}

void Menu_create(int *member_size, Member member_list[], int *account_size, Account account_list[]) {
    UI_Header("ADD NEW MEMBER", ANSI_COLOR_CYAN);
    if (*member_size >= MAX_ACCOUNT || *account_size >= MAX_ACCOUNT) {
        printf("\n" ANSI_BRIGHT_RED "  [!] SYSTEM CAPACITY FULL! CANNOT ADD MORE MEMBERS (MAX %d).\n" ANSI_COLOR_RESET, MAX_ACCOUNT);
        Sleep(3000);
        system("cls");
        return;
    }
    Member new_member = Init_Member();
    Input_studentId(new_member.studentId);
    if (Find_studentId(*member_size, member_list, new_member.studentId) != -1) {
        printf(ANSI_COLOR_RED "Member with Student ID %s already exists.\nAccount creation failed\n" ANSI_COLOR_RESET, new_member.studentId);
        Sleep(3000);
        system("cls");
        return;
    }
    Input_fullname(new_member.fullName);
    Input_email(new_member.email);
    Input_phone(new_member.phone);
    Input_team(&new_member.team);
    Input_role(&new_member.role);

    Account new_account = Init_Account();
    strcpy(new_account.studentId, new_member.studentId);
    strcpy(new_account.password, new_member.studentId);
    new_account.role = new_member.role;

    if (create_member(member_size, member_list, new_member, account_size, account_list, new_account) == 0) {
        Announcement_error_acction();
    } else Announcement_complete_action();
}

static int Find_and_validate_studentId(const int member_size, Member member_list[], char studentId[]) {
    Input_studentId(studentId);
    int idx = Find_studentId(member_size, member_list, studentId);
    while (idx == -1) {
        printf(ANSI_COLOR_RED "Member with Student ID %s does not exist\n" ANSI_COLOR_RESET, studentId);
        int user_choose = -1;
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [0] " ANSI_COLOR_RESET "Back to menu\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Try again\n");
        Input_user_choose(&user_choose);
        if (user_choose == 0) {
            system("cls");
            return -1;
        }
        Input_studentId(studentId);
        idx = Find_studentId(member_size, member_list, studentId);
    }
    return idx;
}

static int Delete_member_account(const int index, int *member_size, Member member_list[], int *account_size, Account account_list[]) {
    for (int i = index + 1; i < (*member_size); ++i) {
        member_list[i - 1] = member_list[i];
        account_list[i - 1] = account_list[i];
    }
    --(*member_size);
    --(*account_size);
    if (Rewrite_members_dat(*member_size, member_list) == 0 || Rewrite_accounts_dat(*account_size, account_list) == 0) {
        return 0;
    }
    return 1;
}



static int Delete_violation(const char studentId[], int *violation_size, Violation violation_list[]) {
    for (int i = 0; i < (*violation_size); ++i) {
        if (strcmp(studentId, violation_list[i].studentId) == 0) {
            for (int j = i + 1; j < (*violation_size); ++j) {
                violation_list[j - 1] = violation_list[j];
            }
            --(*violation_size);
            --i;
        }
    }
    if (Rewrite_violations_dat(*violation_size, violation_list) == 0) return 0;
    return 1;
}

int Delete_member(const int index, char studentId[], int *member_size, Member member_list[], int *account_size, Account account_list[], int *violation_size, Violation violation_list[]) {
    if (Append_deleted_members_dat(member_list[index]) == 0) return 0;
    if (Append_deleted_accounts_dat(account_list[index]) == 0) return 0;
    if (Delete_member_account(index, member_size, member_list, account_size, account_list) == 0) return 0;
    if (Delete_violation(studentId, violation_size, violation_list) == 0) return 0;
    return 1;
}

void Menu_remove(int *member_size, Member member_list[], int *account_size, Account account_list[], int *violation_size, Violation violation_list[]) {
    UI_Header("REMOVE MEMBER", ANSI_COLOR_RED);
    char studentId[SHORT_SIZE];
    int idx = Find_and_validate_studentId(*member_size, member_list, studentId);
    if (idx == -1) return;
    printf(ANSI_COLOR_RED ANSI_BOLD "  [!] WARNING: This will delete member, account, and violations.\n" ANSI_COLOR_RESET);
    printf("  Do you want to continue?\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "  [0] " ANSI_COLOR_RESET "No, cancel\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [1] " ANSI_COLOR_RESET "Yes, delete\n");
    int user_choose = 0;
    Input_user_choose(&user_choose);
    switch (user_choose) {
    case 0:
        system("cls");
        return;
    case 1:
        if (Delete_member(idx, studentId, member_size, member_list, account_size, account_list, violation_size, violation_list) == 0) {
            Announcement_error_acction();
        } else Announcement_complete_action();
        break;
    default:
        Announcement_unaivailable_option();
        break;
    }
}

void print_memnu_update(const int index, Member member_list[], Account account_list[]) {
    Member res = member_list[index];
    UI_Card_Start("CURRENT MEMBER INFORMATION", ANSI_COLOR_CYAN);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    👤 Full Name    : " ANSI_COLOR_RESET "%s\n", res.fullName);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    🆔 Student ID   : " ANSI_COLOR_RESET "%s\n", res.studentId);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    📧 Email        : " ANSI_COLOR_RESET "%s\n", res.email);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    📞 Phone        : " ANSI_COLOR_RESET "%s\n", res.phone);
    const char *teams[] = {"Academic", "Planning", "HR", "Media"};
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    👥 Team         : " ANSI_COLOR_RESET "%s\n", teams[res.team]);
    const char *roles[] = {"Member", "Leader/Vice", "Ban Chu Nhiem"};
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    🎖  Role         : " ANSI_COLOR_RESET "%s\n", roles[res.role]);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    🗝  Password     : " ANSI_COLOR_RESET "%s\n", account_list[index].password);
    UI_Card_End(ANSI_COLOR_CYAN);

    UI_Card_Start("UPDATE OPTIONS", ANSI_COLOR_YELLOW);
    UI_Menu_Item(1, "Full name", ANSI_COLOR_YELLOW);
    UI_Menu_Item(2, "Email", ANSI_COLOR_YELLOW);
    UI_Menu_Item(3, "Phone number", ANSI_COLOR_YELLOW);
    UI_Menu_Item(4, "Team", ANSI_COLOR_YELLOW);
    UI_Menu_Item(5, "Role", ANSI_COLOR_YELLOW);
    UI_Menu_Item(6, "Password", ANSI_COLOR_YELLOW);
    UI_Menu_Item(7, "Reset Locked Status", ANSI_COLOR_YELLOW);
    UI_Divider(50, ANSI_COLOR_YELLOW);
    UI_Menu_Item(0, "Finish / Exit", ANSI_COLOR_YELLOW);
    UI_Card_End(ANSI_COLOR_YELLOW);
}

void Menu_update(const int member_size, Member member_list[], Account account_list[]) {
    UI_Header("UPDATE MEMBER", ANSI_COLOR_CYAN);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Enter Student ID of member to update:\n" ANSI_COLOR_RESET);
    char studentId[SHORT_SIZE];
    int idx = Find_and_validate_studentId(member_size, member_list, studentId);
    if (idx == -1) return;
    system("cls");
    while (1) {
        print_memnu_update(idx, member_list, account_list);
        Member upd_member = member_list[idx];
        Account upd_account = account_list[idx];
        int user_choose = 0;
        Input_user_choose(&user_choose);
        switch (user_choose) {
        case 0:
            system("cls");
            return;
        case 1:
            Input_fullname(upd_member.fullName);
            break;
        case 2:
            Input_email(upd_member.email);
            break;
        case 3:
            Input_phone(upd_member.phone);
            break;
        case 4:
            Input_team(&upd_member.team);
            break;
        case 5:
            Input_role(&upd_member.role);
            upd_account.role = upd_member.role;
            break;
        case 6:
            Input_password(upd_account.password);
            break;
        case 7:
            upd_account.isLocked = upd_account.failCount = 0;
            break;
        default:
            Announcement_unaivailable_option();
            return;
        }
        if (user_choose <= 5 && Update_members_dat(idx, upd_member) == 0) {
            Announcement_error_acction();
            return;
        }
        if (user_choose > 5 && Update_accounts_dat(idx, upd_account) == 0) {
            Announcement_error_acction();
            return;
        }
        member_list[idx] = upd_member;
        account_list[idx] = upd_account;
        Announcement_complete_action();
    }
}
void Print_Deleted_Members(const int member_size, Member member_list[]) {
    UI_Header("DELETED MEMBERS LIST", ANSI_COLOR_RED);
    const char *headers[] = {"ID", "FULL NAME", "EMAIL", "PHONE"};
    const int widths[] = {10, 25, 25, 12};
    UI_Table_Header(4, headers, widths, ANSI_COLOR_RED);

    for (int i = 0; i < member_size; ++i) {
        const char *values[] = {
            member_list[i].studentId,
            member_list[i].fullName,
            member_list[i].email,
            member_list[i].phone
        };
        UI_Table_Row(4, values, widths, ANSI_COLOR_RED);
    }
    UI_Table_End(4, widths, ANSI_COLOR_RED);
    UI_Return_Prompt();
}

void Menu_view_deleted_members(void) {
    int deleted_member_count = Count_deleted_members_dat();
    if (deleted_member_count < 0) {
        Announcement_error_acction();
        return;
    }
    if (deleted_member_count == 0) {
        printf("No deleted members found.\n");
        return;
    }

    Member deleted_member_list[MAX_ACCOUNT];
    if (Read_deleted_members_dat(deleted_member_count, deleted_member_list) == false) {
        Announcement_error_acction();
        return;
    }
    system("cls");
    Print_Deleted_Members(deleted_member_count, deleted_member_list);
}
void view_Profile (Member currentuser) {
    UI_Card_Start("YOUR PROFILE", ANSI_COLOR_CYAN);
    
    if (currentuser.consecutiveAbsences >= 2) {
        if (currentuser.consecutiveAbsences > 3) {
            printf(ANSI_BRIGHT_RED ANSI_BOLD 
                   "  [!] DANGER: %d CONSECUTIVE ABSENCES (>3)\n"
                   "      You are subject to expulsion!\n\n" 
                   ANSI_COLOR_RESET, currentuser.consecutiveAbsences);
        } else {
            printf(ANSI_BRIGHT_YELLOW ANSI_BOLD 
                   "  [!] WARNING: %d CONSECUTIVE ABSENCES\n"
                   "      Limit is 3 before expulsion.\n\n" 
                   ANSI_COLOR_RESET, currentuser.consecutiveAbsences);
        }
    }

    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    👤 Name    : " ANSI_COLOR_RESET "%s\n", currentuser.fullName);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    🆔 ID      : " ANSI_COLOR_RESET "%s\n", currentuser.studentId);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    📧 Email   : " ANSI_COLOR_RESET "%s\n", currentuser.email);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    📞 Phone   : " ANSI_COLOR_RESET "%s\n", currentuser.phone);
    char *teams[] = {"Academic", "Planning", "HR", "Media"};
    char *roles[] = {"Member", "Leader/Vice", "Management Board"};
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    👥 Team    : " ANSI_COLOR_RESET "%s\n", teams[currentuser.team]);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    🎖  Role    : " ANSI_COLOR_RESET "%s\n", roles[currentuser.role]);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "    ⚠  Absences: " ANSI_COLOR_RESET "%d\n", currentuser.consecutiveAbsences);
    UI_Card_End(ANSI_COLOR_CYAN);
    UI_Return_Prompt();
}
void view_CLB_Profile (Member member[], int size_memberlist) {
    UI_Header("CLUB MEMBERS LIST", ANSI_COLOR_CYAN);
    
    const char *headers[] = {"FULL NAME", "TEAM", "ROLE", "VIOLATIONS"};
    const int widths[] = {25, 12, 12, 10};
    UI_Table_Header(4, headers, widths, ANSI_COLOR_CYAN);

    const char *teams[] = {"Academic", "Planning", "HR", "Media"};
    const char *roles[] = {"Member", "Leader/Vice", "BCN"};
    
    for (int i = 0; i < size_memberlist; i++) {
        char countStr[12];
        snprintf(countStr, sizeof(countStr), "%d", member[i].violationCount);
        const char *values[] = {
            member[i].fullName,
            teams[member[i].team],
            roles[member[i].role],
            countStr
        };
        UI_Table_Row(4, values, widths, ANSI_COLOR_CYAN);
    }
    UI_Table_End(4, widths, ANSI_COLOR_CYAN);
    printf("\n" ANSI_BRIGHT_BLACK "  Total members: %d\n" ANSI_COLOR_RESET, size_memberlist);
    UI_Return_Prompt();
}
