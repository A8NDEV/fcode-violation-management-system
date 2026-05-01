#include "utils.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"
#include "validate.h"

void Stdin_string(char str[],int siz){
    if(fgets(str,siz,stdin) == NULL){
        str[0] = '\0';
        return;
    }
    size_t len = strcspn(str, "\n");
    if(str[len] == '\n')    str[len] = '\0';//inputuser is not greater than SHORT_SIZE 
    else{
        // don't have '\n' => inputuser is greater than SHORT_SIZE
        clear_buffer();//clear
    }
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
    printf("Your choose: ");
    scanf("%d",input);
    clear_buffer();
}
void Input_fullname(char fullname[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)  printf("Enter Full-name: ");
        else    printf("Invalid input, try again: ");
        Stdin_string(fullname,LONG_SIZE);
        ok = (ok || Validate_fullname(fullname));
        wrong |= 1;
    }
}
void Input_email(char email[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)   printf("Enter email: ");
        else    printf("Invalid input, try again: ");
        Stdin_string(email,LONG_SIZE);
        ok = (ok || Validate_email(email));
        wrong |= 1;
    }
}
void Input_studentId(char studentId[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)   printf("Enter Student ID: ");
        else    printf("Invalid input, try again: ");
        Stdin_string(studentId,SHORT_SIZE);
        ok = (ok || Validate_studentId(studentId));
        wrong |= 1;
    }
}
void Input_phone(char phone_num[]){
    bool ok = false;
    bool wrong = false;
    while(!ok){
        if(wrong == false)  printf("Enter phone number: ");
        else    printf("Invalid input, try again: ");
        Stdin_string(phone_num,SHORT_SIZE);
        ok |= Validate_phone(phone_num);
        wrong |= 1;
    }
}
void Input_team(int *input){
    bool ok = false;
    bool wrong = false;
    printf("[0] | Academic\n[1] | Planning\n[2] | HR\n[3] | Media\n");
    while(!ok){
        if(wrong == false)  printf("Enter member's team: ");
        else    printf("Invalid input, try again: ");
        scanf("%d",input);
        clear_buffer();
        ok |= Validate_team(*input);
        wrong |= 1;
    }
}
void Input_role(int *input){
    bool ok = false,wrong = false;
    printf("[0] | Member\n[1] | Leader/Vice\n[2] | Ban Chu Nhiem\n");
    while(!ok){
        if(wrong == false)  printf("Enter member's role: ");
        else    printf("Invalid input, try again: ");
        scanf("%d",input);
        clear_buffer();
        ok |= Validate_role(*input);
        wrong |= 1;
    }
}
void Input_violation_reason(int *input){
    bool ok = false,wrong = false;
    while(!ok){
        if(wrong == false)  printf("Enter reason of violation: ");
        else    printf("Invalid input, try again: ");
        scanf("%d",input);
        clear_buffer();
        ok |= Validate_violation_reason(*input);
        wrong |= 1;
    }
}
void Input_password(char pass[]){
    bool ok = false,wrong = false;
    while(!ok){
        if(wrong == false)  printf("Enter password: ");
        else    printf("Invalid input, try again: ");
        Stdin_string(pass,LONG_SIZE);
        ok |= Validate_password(pass);
        wrong |= 1;
    }
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