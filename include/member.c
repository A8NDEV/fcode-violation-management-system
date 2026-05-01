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
    member_list[*member_size] = new_member;
    account_list[*member_size] = new_account;
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
    strcpy(new_account.password,new_member.studentId);
    new_account.role = new_member.role;

    if(create_member(member_size,member_list,new_member,account_list,new_account) == 0){
        Announcement_error_acction();
    }
    else    Announcement_complete_action();
}

static int Delete_member_account(const int index,int *member_size,Member member_list[],Account account_list[]){
    for(int i = index + 1;i < (*member_size);++i){
        member_list[i - 1] = member_list[i];
        account_list[i - 1] = account_list[i];
    }
    --(*member_size);
    if(Rewrite_members_dat(*member_size,member_list) == 0 || Rewrite_accounts_dat(*member_size,account_list) == 0){
        return 0;
    }
    return 1;
}
static int Delete_violation(const char studentId[],int *violation_size,Violation violation_list[]){
    for(int i = 0;i < (*violation_size);++i){
        if(strcmp(studentId,violation_list[i].studentId) == 0){
            for(int j = i + 1;j < (*violation_size);++j){
                violation_list[j - 1] = violation_list[j];
            }
            --(*violation_size);
            --i; // continue scanning from current index after shift
        }
    }
    if(Rewrite_violations_dat(*violation_size,violation_list) == 0) return 0;
    return 1;
}
int Delete_member(const int index,char studentId[],int *member_size,Member member_list[],Account account_list[],int *violation_size,Violation violation_list[]){
    if(Delete_member_account(index,member_size,member_list,account_list) == 0)  return 0;
    if(Delete_violation(studentId,violation_size,violation_list) == 0) return 0;
    return 1;
}
static void Menu_remove(int *member_size,Member member_list[],Account account_list[],int *violation_size,Violation violation_list[]){
    char studentId[SHORT_SIZE];
    Input_studentId(studentId);
    int idx = Find_studentId(*member_size,member_list,studentId);
    while(idx == -1){
        printf("ko ton tai member voi MSSV %s\n",studentId);
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
    printf("WARNING: This action will delete the member, account, and related violations.\n");
    printf("Do you want to continue?\n");
    printf("[0] | No, cancel\n");
    printf("[1] | Yes, delete\n");
    int user_choose = 0;
    Input_user_choose(&user_choose);
    switch (user_choose){
        case 0:
            system("cls");
            return;
        case 1:
            if(Delete_member(idx,studentId,member_size,member_list,account_list,violation_size,violation_list) == false){
                Announcement_error_acction();
            }
            else    Announcement_complete_action();
            break;
        default:
            Announcement_unaivailable_option();
            break;
    }
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
                Menu_update(*member_size,member_list,account_list);
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