#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void print_logo(void);
void clear_buffer(void);
void Stdin_string(char str[],int siz);

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