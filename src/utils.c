#include "utils.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "types.h"
#include "validate.h"
#include "auth.h"

void print_logo(void) {
    printf("\n");
    printf(ANSI_BRIGHT_GREEN ANSI_BOLD);
    printf("  ███████╗      ██████╗ ██████╗ ██████╗ ███████╗    \n");
    printf("  ██╔════╝     ██╔════╝██╔═══██╗██╔══██╗██╔════╝    \n");
    printf("  █████╗  ████╗██║     ██║   ██║██║  ██║█████╗      \n");
    printf("  ██╔══╝  ╚═══╝██║     ██║   ██║██║  ██║██╔══╝      \n");
    printf("  ██║          ╚██████╗╚██████╔╝██████╔╝███████╗    \n");
    printf("  ╚═╝           ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝    \n");
    printf(ANSI_COLOR_YELLOW "                C O D E  T H E  D R E A M          \n" ANSI_COLOR_RESET);
    printf("\n");
}

void print_member_logo(void) {
    printf(ANSI_COLOR_GREEN ANSI_BOLD);
    printf("  ███╗   ███╗███████╗███╗   ███╗██████╗ ███████╗██████╗ \n");
    printf("  ████╗ ████║██╔════╝████╗ ████║██╔══██╗██╔════╝██╔══██╗\n");
    printf("  ██╔████╔██║█████╗  ██╔████╔██║██████╔╝█████╗  ██████╔╝\n");
    printf("  ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██╔══██╗██╔══╝  ██╔══██╗\n");
    printf("  ██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║██████╔╝███████╗██║  ██║\n");
    printf("  ╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝\n");
    printf(ANSI_COLOR_RESET "\n");
}

void print_bcn_logo(void) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("  ██████╗  ██████╗███╗   ██╗\n");
    printf("  ██╔══██╗██╔════╝████╗  ██║\n");
    printf("  ██████╔╝██║     ██╔██╗ ██║\n");
    printf("  ██╔══██╗██║     ██║╚██╗██║\n");
    printf("  ██████╔╝╚██████╗██║ ╚████║\n");
    printf("  ╚═════╝  ╚═════╝╚═╝  ╚═══╝\n");
    printf(ANSI_COLOR_RESET "\n");
}

void UI_Header(const char *title, const char *color) {
    int width = 48; // Internal width (dashes)
    printf("\n%s╭", color);
    for (int i = 0; i < width; i++) printf("─");
    printf("╮\n");

    printf("│" ANSI_BOLD ANSI_COLOR_WHITE " %-46s " ANSI_COLOR_RESET "%s│\n", title, color);

    printf("╰");
    for (int i = 0; i < width; i++) printf("─");
    printf("╯" ANSI_COLOR_RESET "\n");
}

void UI_Card_Start(const char *title, const char *color) {
    int width = 48;
    int title_len = strlen(title);
    int side_dashes = (width - title_len - 2) / 2;

    printf("\n%s╭", color);
    for (int i = 0; i < side_dashes; i++) printf("─");
    printf(" " ANSI_BOLD ANSI_COLOR_WHITE "%s" ANSI_COLOR_RESET "%s ", title, color);
    for (int i = 0; i < (width - title_len - 2 - side_dashes); i++) printf("─");
    printf("╮" ANSI_COLOR_RESET "\n");
}

void UI_Card_End(const char *color) {
    int width = 48;
    printf("%s╰", color);
    for (int i = 0; i < width; i++) printf("─");
    printf("╯" ANSI_COLOR_RESET "\n");
}

void UI_Menu_Item(int index, const char *label, const char *color) {
    char buf[64];
    if (index >= 0) {
        snprintf(buf, sizeof(buf), "[%d] %s", index, label);
    } else {
        snprintf(buf, sizeof(buf), "%s", label);
    }
    printf("%s│ " ANSI_BOLD ANSI_COLOR_CYAN "%-46s " ANSI_COLOR_RESET "%s│\n", color, buf, color);
}

void UI_Divider(int width, const char *color) {
    int w = 48; // Fixed for consistency
    printf("%s├", color);
    for (int i = 0; i < w; i++) printf("─");
    printf("┤" ANSI_COLOR_RESET "\n");
}

void UI_Prompt(const char *label, const char *glyph) {
    printf(ANSI_BOLD ANSI_COLOR_YELLOW "  %s %-15s " ANSI_COLOR_RESET ": ", glyph, label);
}

int UTF8_Display_Width(const char *s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xc0) != 0x80) len++;
        s++;
    }
    return len;
}

void UI_Table_Header(int num_cols, const char *headers[], const int widths[], const char *color) {
    printf("  %s╭", color);
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) printf("─");
        if (i < num_cols - 1) printf("┬");
    }
    printf("╮" ANSI_COLOR_RESET "\n");

    printf("  %s│" ANSI_COLOR_RESET, color);
    for (int i = 0; i < num_cols; i++) {
        int d_width = UTF8_Display_Width(headers[i]);
        printf(" %s%-*s" ANSI_COLOR_RESET " %s│" ANSI_COLOR_RESET, 
               ANSI_BOLD, (int)(widths[i] + (strlen(headers[i]) - d_width)), headers[i], color);
    }
    printf("\n");

    printf("  %s├", color);
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) printf("─");
        if (i < num_cols - 1) printf("┼");
    }
    printf("┤" ANSI_COLOR_RESET "\n");
}

void UI_Table_Row(int num_cols, const char *values[], const int widths[], const char *color) {
    printf("  %s│" ANSI_COLOR_RESET, color);
    for (int i = 0; i < num_cols; i++) {
        int d_width = UTF8_Display_Width(values[i]);
        printf(" %-*s %s│" ANSI_COLOR_RESET, (int)(widths[i] + (strlen(values[i]) - d_width)), values[i], color);
    }
    printf("\n");
}

void UI_Table_End(int num_cols, const int widths[], const char *color) {
    printf("  %s╰", color);
    for (int i = 0; i < num_cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) printf("─");
        if (i < num_cols - 1) printf("┴");
    }
    printf("╯" ANSI_COLOR_RESET "\n");
}

void UI_Return_Prompt(void) {
    printf("\n" ANSI_BOLD ANSI_COLOR_YELLOW "  ❯ Press Enter to return... " ANSI_COLOR_RESET);
    getchar();
}

void Stdin_string(char str[],int siz){
    if(fgets(str,siz,stdin) == NULL){
        str[0] = '\0';
        return;
    }
    size_t len = strcspn(str, "\n");
    if(str[len] == '\n')    str[len] = '\0';
    else{
        int c;
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
    UI_Prompt("Your Choice", "❯");
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
        if(wrong == false)  UI_Prompt("Full Name", "❯");
        else    UI_Prompt("Invalid Name, try again", "❯");
        Stdin_string(fullname,LONG_SIZE);
        ok = (ok || Validate_fullname(fullname));
        wrong |= 1;
    }
}
void Input_email(char email[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)   UI_Prompt("Email", "❯");
        else    UI_Prompt("Invalid Email, try again", "❯");
        Stdin_string(email,LONG_SIZE);
        ok = (ok || Validate_email(email));
        wrong |= 1;
    }
}
void Input_studentId(char studentId[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)   UI_Prompt("Student ID", "❯");
        else    UI_Prompt("Invalid ID, try again", "❯");
        Stdin_string(studentId,SHORT_SIZE);
        ok = (ok || Validate_studentId(studentId));
        wrong |= 1;
    }
}
void Input_phone(char phone_num[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)  UI_Prompt("Phone Number", "❯");
        else    UI_Prompt("Invalid Phone, try again", "❯");
        Stdin_string(phone_num,SHORT_SIZE);
        ok |= Validate_phone(phone_num);
        wrong |= 1;
    }
}
void Input_team(int *input){
    bool ok = false;
    bool wrong = false;
    printf("\n" ANSI_BOLD "  Select Team:\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [0] " ANSI_COLOR_RESET "Academic\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [1] " ANSI_COLOR_RESET "Planning\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [2] " ANSI_COLOR_RESET "HR\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [3] " ANSI_COLOR_RESET "Media\n");
    while(!ok){
        if(wrong == false)  UI_Prompt("Team", "❯");
        else    UI_Prompt("Invalid Team, try again", "❯");
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
    printf("\n" ANSI_BOLD "  Select Role:\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [0] " ANSI_COLOR_RESET "Member\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [1] " ANSI_COLOR_RESET "Leader/Vice\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN "  [2] " ANSI_COLOR_RESET "BCN\n");
    while(!ok){
        if(wrong == false)  UI_Prompt("Role", "❯");
        else    UI_Prompt("Invalid Role, try again", "❯");
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
        if(wrong == false)  UI_Prompt("Violation", "❯");
        else    UI_Prompt("Invalid Reason, try again", "❯");
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
    bool ok = false, wrong = false;
    while(!ok){
        if(wrong == false)  UI_Prompt("New Password", "🗝");
        else    UI_Prompt("Invalid Password, try again", "🗝");
        inputPassword(pass, LONG_SIZE);
        if (Validate_password(pass) == false) {
            printf(ANSI_BOLD ANSI_COLOR_RED
                   "  ✖ YOUR PASSWORD NEEDS AT LEAST ONE UPPERCASE, ONE LOWERCASE, AND ONE DIGIT (MIN 9 CHARS)!\n" ANSI_COLOR_RESET);
            wrong = true;
            continue;
        }
        char confirm[LONG_SIZE];
        UI_Prompt("Confirm Password", "🗝");
        inputPassword(confirm, LONG_SIZE);
        if (strcmp(pass, confirm) != 0) {
            printf(ANSI_BOLD ANSI_COLOR_RED " ❯ PASSWORDS DO NOT MATCH, TRY AGAIN!\n" ANSI_COLOR_RESET);
            wrong = true;
            continue;
        }
        ok = true;
    }
}

/* Parse date input DD/MM/YYYY and convert to time_t.
 * Returns (time_t)(-1) if the format is invalid. */
time_t Input_date(const char *prompt) {
    char buf[12];
    int day, mon, year;
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ %s (DD/MM/YYYY): " ANSI_COLOR_RESET, prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return (time_t)(-1);
    /* Strip trailing newline */
    buf[strcspn(buf, "\n")] = '\0';
    if (sscanf(buf, "%d/%d/%d", &day, &mon, &year) != 3) return (time_t)(-1);
    struct tm t = {0};
    t.tm_mday  = day;
    t.tm_mon   = mon - 1;
    t.tm_year  = year - 1900;
    t.tm_hour  = 0; t.tm_min = 0; t.tm_sec = 0;
    time_t result = mktime(&t);
    return result;
}

int Find_studentId(int member_size,Member member_list[],char studentId[]){
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
