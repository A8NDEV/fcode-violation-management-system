#ifndef UTILS_H
#define UTILS_H

#include <structure.h>

void clear_buffer(void);
void Stdin_string(char str[],int siz);

//Announment function  group
void Announcement_unaivailable_option();
void Announcement_wrong_format();
void Announcement_uaivailable_member();

//Find function group
int Find_studentId(int member_size,Member member_list[],char studentId[]);


#endif