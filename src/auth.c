#include "auth.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void login(Account *accountList, int *quantity, int *isLogin, void *notUsed, Account *session) {
    char studentIdInput[100], studentPasswordInput[MAX_PASS_LEN];
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║             MENU ĐĂNG NHẬP             ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MÃ SINH VIÊN: " ANSI_COLOR_RESET);
    scanf(" %99[^\n]", studentIdInput);
    while (getchar() != '\n');
    int foundIndex = -1;
    for (int i = 0; i < *quantity; i++) {
        if (strcmp(accountList[i].studentId, studentIdInput) == 0) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == -1) {
        printf("\n" ANSI_COLOR_RED "KHÔNG TÌM THẤY MÃ SINH VIÊN NÀY!\n");
        return;
    }
    if (accountList[foundIndex].isLocked == 1) {
        printf(ANSI_BRIGHT_RED "TÀI KHOẢN ĐANG BỊ KHÓA\n");
        return;
    }
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MẬT KHẨU    : " ANSI_COLOR_RESET);
    scanf(" %49[^\n]", studentPasswordInput);
    while (getchar() != '\n');
    if (strcmp(accountList[foundIndex].password, studentPasswordInput) == 0) {
        printf("\n" ANSI_BRIGHT_GREEN "ĐÃ ĐĂNG NHẬP THÀNH CÔNG!\n");
        *isLogin = 1;
        accountList[foundIndex].failCount = 0;
        *session = accountList[foundIndex];
    } else {
        printf("\n" ANSI_BRIGHT_RED "MẬT KHẨU KHÔNG ĐÚNG!\n");
        accountList[foundIndex].failCount++;
        if (accountList[foundIndex].failCount >= 3) {
            accountList[foundIndex].isLocked = 1;
            printf(ANSI_BRIGHT_RED "TÀI KHOẢN ĐÃ BỊ KHÓA DO NHẬP SAI 3 LẦN!\n");
        }
    }
}

void changePassword(Account *accountList, int role, int *quantity, Account *session) {
    if (role == 0) {
        char passwordChange[MAX_PASS_LEN], oldPassword[MAX_PASS_LEN];
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MẬT KHẨU CŨ : " ANSI_COLOR_RESET);
        scanf(" %49[^\n]", oldPassword);
        while (getchar() != '\n');
        if (strcmp(oldPassword, session->password) == 0) {
            printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ NHẬP MẬT KHẨU MỚI : " ANSI_COLOR_RESET);
            scanf(" %49[^\n]", passwordChange);
            while (getchar() != '\n');
            for (int i = 0; i < *quantity; i++) {
                if (strcmp(accountList[i].studentId, session->studentId) == 0) {
                    strcpy(accountList[i].password, passwordChange);
                    strcpy(session->password, passwordChange);
                    printf(ANSI_BRIGHT_GREEN "ĐỔI MẬT KHẨU THÀNH CÔNG!\n");
                    break;
                }
            }
        } else {
            printf(ANSI_BRIGHT_RED "SAI MẬT KHẨU CŨ!\n");
        }
    } else {
        char studentId[MAX_ID_LEN], newPass[MAX_PASS_LEN];
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ NHẬP MSSV CẦN ĐỔI: " ANSI_COLOR_RESET);
        scanf(" %8[^\n]", studentId);
        while (getchar() != '\n');
        for (int i = 0; i < *quantity; i++) {
            if (strcmp(accountList[i].studentId, studentId) == 0) {
                printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ NHẬP MẬT KHẨU MỚI: " ANSI_COLOR_RESET);
                scanf(" %49[^\n]", newPass);
                while (getchar() != '\n');
                strcpy(accountList[i].password, newPass);
                printf(ANSI_BRIGHT_GREEN "ĐÃ ĐỔI MẬT KHẨU CHO %s\n", studentId);
                return;
            }
        }
        printf(ANSI_BRIGHT_RED "KHÔNG TÌM THẤY MSSV NÀY!\n");
    }
}
