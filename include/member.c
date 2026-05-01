#include "member.h"

#include "types.h"
#include "utils.h"
#include "validate.h"
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static int create_member(int *member_size,Member member_list[],Member new_member,Account account_list[],Account new_account){
    //add to member_list array
    member_list[*member_size + 1] = new_member;
    account_list[*member_size + 1] = new_account;
    //add new binary record at end file
    if(Append_members_dat(new_member) == 0 || Append_accounts_dat(new_account) == 0)    return 0;
    ++(*member_size);
    return 1;
}
static void Menu_create(int *member_size,Member member_list[],Account account_list[]){
    Member new_member = Init_Member();
    Input_fullname(new_member.fullName);
    Input_studentId(new_member.studentId);
    if(Find_studentId(*member_size,member_list,new_member.studentId) != -1){
        printf("Da co member voi MSSV la %s.\nTao tai khoan that bai\n",new_member.studentId);
        Sleep(3000);
        system("cls");
        return;
    }
    Input_email(new_member.email);
    Input_phone(new_member.phone);
    Input_team(&new_member.team);
    Input_role(&new_member.role);

    Account new_account = Init_Account();
    strcpy(new_account.studentId,new_member.studentId);
    Input_password(new_account.password);
    new_account.role = new_member.role;

    if(create_member(member_size,member_list,new_member,account_list,new_account) == 0){
        Announcement_error_acction();
    }
    else    Announcement_complete_action();
}

static void remove_member_account(int *member_size,Member member_list[],Account account_list[],int idx){
    //Certain that member_list and account_list have a same order.
    for(int i = idx + 1;i < *member_size;++i){
        member_list[i - 1] = member_list[i];
        account_list[i - 1] = account_list[i];
    }
    --(*member_size);
}
static void remove_violation_index(int *violation_size,Violation violation_list[],int idx){
    //delete a specific index in violation list
    for(int i = idx + 1;i < *violation_size;++i){
        violation_list[i - 1] = violation_list[i];
    }
    --(*violation_size);
}
static void Menu_remove(int *member_size,Member member_list[],Account account_list[],int *violation_size,Violation violation_list[]){
    char studentId[SHORT_SIZE];
    Input_studentId(studentId);
    int idx = Find_studentId(*member_size,member_list,studentId);
    while(idx == -1){
        printf("ko ton tai member voi MSSV %s\n",studentId);
        printf("Action Fail!\n");
        int user_choose = -1;
        printf("[0] | back to memnu\n");
        printf("[1] | try again\n");
        Input_user_choose(&user_choose);
        if(user_choose == 0){
            system("cls");
            return;
        }
        Input_studentId(studentId);
        idx = Find_studentId(*member_size,member_list,studentId);
    }
    remove_member_account(member_size,member_list,account_list,idx);
    for(int i = 0;i < *violation_size;++i)   if(strcmp(studentId,violation_list[i].studentId)){
        remove_violation_index(violation_size,violation_list,i);
    }
    Announcement_complete_action();
}

static void Menu_update(){

}
void main_menu_CRUD(int *member_size,Member member_list[],Account account_list[],int *violation_size,Violation violation_list[]){
    // main menu of CRUD
    while(1){
        printf("======================================\n");
        printf("[1] | Them member\n");
        printf("[2] | Cap nhat thong tin member\n");
        printf("[3] | Xoa member\n");
        printf("[0] | Thoat\n");
        int user_choose = 0;
        Input_user_choose(&user_choose);
        system("cls");

        switch(user_choose){
            case 0:
                return;
            case 1:
                Menu_create(member_size,member_list,account_list);
                break;
            case 2:
                Menu_update();
                break;
            case 3:
                Menu_remove(member_size,member_list,account_list,violation_size,violation_list);
                break;
            default:
                Announcement_unaivailable_option();
                break;
        }
    }
}