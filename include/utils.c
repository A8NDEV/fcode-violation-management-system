#include "utils.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

void Stdin_string(char str[],int siz){
    fgets(str,siz,stdin);
    str[strcspn(str,"\n")] = '\0';
}
void clear_buffer(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void Announcement_unaivailable_option(){
    printf("Lua chon ko ton tai!");
    Sleep(3000);
    system("cls");
}
void Announcement_wrong_format(){
    printf("Dinh dang khong hop le!");
    Sleep(3000);
    system("cls");
}
void Announcement_uaivailable_member(){
    printf("Member khong ton tai.");
    Sleep(3000);
    system("cls");
}


int Find_studentId(int member_size,Member member_list[],char studentId[]){
    //WARNNING
    //have : return index
    //don't have : return -1
    for(int i = 0;i <= member_size;++i){
        if(strcmp(member_list[i].studentId,studentId) == 0){
            return i;
        }
    }
    return -1;
}