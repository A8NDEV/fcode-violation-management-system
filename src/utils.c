#include "utils.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"
#include "validate.h"
#include "auth.h"

void print_logo(void) {
    printf("\n");
    printf(ANSI_COLOR_GREEN ANSI_BOLD);
    printf("  ███████╗      ██████╗ ██████╗ ██████╗ ███████╗    \n");
    printf("  ██╔════╝     ██╔════╝██╔═══██╗██╔══██╗██╔════╝    \n");
    printf("  █████╗  ████╗██║     ██║   ██║██║  ██║█████╗      \n");
    printf("  ██╔══╝  ╚═══╝██║     ██║   ██║██║  ██║██╔══╝      \n");
    printf("  ██║          ╚██████╗╚██████╔╝██████╔╝███████╗    \n");
    printf("  ╚═╝           ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝    \n");
    printf(ANSI_COLOR_YELLOW "                C O D E  T H E  D R E A M          \n" ANSI_COLOR_RESET);
    printf("\n");
}

void Stdin_string(char str[],int siz){
    if(fgets(str,siz,stdin) == NULL){
        str[0] = '\0';
        return;
    }
    size_t len = strcspn(str, "\n");
    if(str[len] == '\n')    str[len] = '\0';//inputuser is not greater than SHORT_SIZE 
    else{
        // don't have '\n' => inputuser is greater than SHORT_SIZE
        int c;
        // clear buffer safely
        while((c = getchar()) != '\n' && c != EOF);
    }
}
void clear_buffer(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void Announcement_unaivailable_option(){
    printf("Option does not exist!");
    Sleep(3000);
    system("cls");
}
void Announcement_wrong_format(){
    printf("Invalid format!");
    Sleep(3000);
    system("cls");
}
void Announcement_uaivailable_member(){
    printf("Member does not exist.");
    Sleep(3000);
    system("cls");
}
void Announcement_complete_action(){
    system("cls");
    printf("Action completed!\n");
    Sleep(2000);
    system("cls");
}
void Announcement_error_acction(){
    system("cls");
    printf("Action Error!\n");
    Sleep(2000);
    system("cls");
}

void Input_user_choose(int *input){
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Your choice   : " ANSI_COLOR_RESET);
    char buf[32];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (sscanf(buf, "%d", input) != 1) {
            *input = -1;
        }
    } else {
        *input = -1;
    }
}
void Input_fullname(char fullname[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)  printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Full name    : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid full name, try again: " ANSI_COLOR_RESET);
        Stdin_string(fullname,LONG_SIZE);
        ok = (ok || Validate_fullname(fullname));
        wrong |= 1;
    }
}
void Input_email(char email[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)   printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Email        : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid email, try again: " ANSI_COLOR_RESET);
        Stdin_string(email,LONG_SIZE);
        ok = (ok || Validate_email(email));
        wrong |= 1;
    }
}
void Input_studentId(char studentId[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)   printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Student ID   : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid Student ID, try again: " ANSI_COLOR_RESET);
        Stdin_string(studentId,SHORT_SIZE);
        ok = (ok || Validate_studentId(studentId));
        wrong |= 1;
    }
}
void Input_phone(char phone_num[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)  printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Phone number : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid phone number, try again: " ANSI_COLOR_RESET);
        Stdin_string(phone_num,SHORT_SIZE);
        ok |= Validate_phone(phone_num);
        wrong |= 1;
    }
}
void Input_team(int *input){
    bool ok = false;
    bool wrong = false;
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [0] " ANSI_COLOR_RESET "Academic\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Planning\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "HR\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [3] " ANSI_COLOR_RESET "Media\n");
    while(!ok){
        if(wrong == false)  printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Team         : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid team, try again: " ANSI_COLOR_RESET);
        char buf[32];
        if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
        if (buf[0] == '\n' || buf[1] != '\n') {
            wrong |= 1;
            continue;
        }
        char x = buf[0];
        if(is_number(x) == false){
            ok = false;
            wrong |= 1;
            continue;
        }
        (*input) = (int)(x - '0');
        ok |= Validate_team(*input);
        wrong |= 1;
    }
}
void Input_role(int *input){
    bool ok = false,wrong = false;
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [0] " ANSI_COLOR_RESET "Member\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "Leader/Vice\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "BCN\n");
    while(!ok){
        if(wrong == false)  printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Role         : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid role, try again: " ANSI_COLOR_RESET);
        char buf[32];
        if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
        if (buf[0] == '\n' || buf[1] != '\n') {
            wrong |= 1;
            continue;
        }
        char x = buf[0];
        if(is_number(x) == false){
            ok = false;
            wrong |= 1;
            continue;
        }
        (*input) = (int)(x - '0');
        ok |= Validate_role(*input);
        wrong |= 1;
    }
}
void Input_violation_reason(int *input){
    bool ok = false,wrong = false;
    while(!ok){
        if(wrong == false)  printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Violation    : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid reason, try again: " ANSI_COLOR_RESET);
        char buf[32];
        if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
        if (sscanf(buf, "%d", input) != 1) {
            wrong |= 1;
            continue;
        }
        ok |= Validate_violation_reason(*input);
        wrong |= 1;
    }
}
void Input_password(char pass[]){
    bool ok = false,wrong = false;
    while(!ok){
        if(wrong == false)  printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Password     : " ANSI_COLOR_RESET);
        else    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Invalid password, try again: " ANSI_COLOR_RESET);
        Stdin_string(pass,LONG_SIZE);
        ok |= Validate_password(pass);
        wrong |= 1;
    }
}

int Find_studentId(int member_size,Member member_list[],char studentId[]){
    //WARNNING
    //have : return index
    //don't have : return -1
    for(int i = 0;i < member_size;++i){
        if(strcmp(member_list[i].studentId,studentId) == 0){
            return i;
        }
    }
    return -1;
}

void Rainbow_Loading(const char *taskName) {
    const char *colors[] = {
        ANSI_BRIGHT_RED,
        ANSI_BRIGHT_YELLOW,
        ANSI_BRIGHT_GREEN,
        ANSI_BRIGHT_CYAN,
        ANSI_BRIGHT_BLUE,
        ANSI_BRIGHT_MAGENTA,
        ANSI_BRIGHT_WHITE
    };
    const char *spinners[] = {"\xe2\xa0\x8b", "\xe2\xa0\x99", "\xe2\xa0\xb9", "\xe2\xa0\xb8", "\xe2\xa0\xbc", "\xe2\xa0\xb4", "\xe2\xa0\xa6", "\xe2\xa0\xa7", "\xe2\xa0\x87", "\xe2\xa0\x8f"};
    int total_blocks = 35;
    printf("\n");
    for (int i = 1; i <= 100; i++) {
        int completed_blocks = (i * total_blocks) / 100;
        const char *spinner = spinners[i % 10];

        printf("\r" ANSI_BRIGHT_YELLOW " %s " ANSI_COLOR_CYAN "%-20s " ANSI_BRIGHT_BLACK "[" ANSI_COLOR_RESET, spinner, taskName);

        for (int b = 0; b < total_blocks; b++) {
            if (b < completed_blocks) {
                const char *blockColor = colors[(b + i / 2) % 7];
                printf("%s\xe2\xa0\xbf", blockColor);
            } else {
                printf(ANSI_BRIGHT_BLACK "\xe2\xa0\x80");
            }
        }
        printf(ANSI_BRIGHT_BLACK "] " ANSI_BRIGHT_WHITE "%3d%%" ANSI_COLOR_RESET, i);
        fflush(stdout);
        Sleep(15);
    }
    printf("\r" ANSI_BRIGHT_GREEN " \xe2\x9c\x94 " ANSI_COLOR_CYAN "%-20s " ANSI_BRIGHT_BLACK "[", taskName);
    for (int b = 0; b < total_blocks; b++) {
        printf("%s\xe2\xa0\xbf", colors[b % 7]);
    }
    printf(ANSI_BRIGHT_BLACK "] " ANSI_BRIGHT_WHITE "100%%" ANSI_BRIGHT_GREEN " (Complete!)\n\n" ANSI_COLOR_RESET);
    Sleep(300);
}
