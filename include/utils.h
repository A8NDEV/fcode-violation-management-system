#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void print_logo(void);
void print_member_logo(void);
void print_bcn_logo(void);
void clear_buffer(void);
void Stdin_string(char str[],int siz);
void UI_Header(const char *title, const char *color);
void UI_Card_Start(const char *title, const char *color);
void UI_Card_End(const char *color);
void UI_Menu_Item(int index, const char *label, const char *color);
void UI_Divider(int width, const char *color);
void UI_Prompt(const char *label, const char *glyph);
void UI_Table_Header(int num_cols, const char *headers[], const int widths[], const char *color);
void UI_Table_Row(int num_cols, const char *values[], const int widths[], const char *color);
void UI_Table_End(int num_cols, const int widths[], const char *color);
void UI_Return_Prompt(void);
int UTF8_Display_Width(const char *s);

//Input function group
void Input_user_choose(int *input);
void Input_fullname(char fullname[]);
void Input_email(char email[]);
void Input_studentId(char varriable[]);
void Input_phone(char phone_num[]);
void Input_team(int *input);
void Input_role(int *input);
void Input_violation_reason(int *input);
void Input_password(char pass[]);
/* Parse a date string DD/MM/YYYY into time_t. Returns -1 on invalid input. */
time_t Input_date(const char *prompt);

//Announment function  group
void Announcement_unaivailable_option();
void Announcement_wrong_format();
void Announcement_uaivailable_member();
void Announcement_complete_action();
void Announcement_error_acction();

//Find function group
int Find_studentId(int member_size,Member member_list[],char studentId[]);

void Rainbow_Loading(const char *taskName);

#endif