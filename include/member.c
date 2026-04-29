#include "member.h"

#include "types.h"
#include "utils.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static void create_member(int member_size,Member member_list[],Member new_member){
    //add to member_list array
    member_list[member_size + 1] = new_member;

    //add new binary record at end file
    FILE *fout;
    fout = fopen("data/members.dat","ab");
    if(fout == NULL){
        printf("khong mo duoc file member.dat\n");
        return;
    }
    //add new record to member.dat
    fwrite(&new_member,sizeof(Member),1,fout);
    fclose(fout);
}
static void create_account(int member_size,Account account_list[],Account new_account){
    //add to member_list array
    account_list[member_size + 1] = new_account;

    //add new binary record at end file
    FILE *fout;
    fout = fopen("data/accounts.dat","ab");
    if(fout == NULL){
        printf("khong mo duoc file account.dat\n");
        return;
    }
    //add new record to account.dat
    fwrite(&new_account,sizeof(Account),1,fout);
    fclose(fout);
}
static void Menu_create(int *member_size,Member member_list[],Account account_list[]){
    Member new_member = Init_Member();
    Input_fullname(new_member.fullName);
    Input_email(new_member.email);
    Input_phone(new_member.phone);
    Input_studentId(new_member.studentId);
    if(Find_studentId(*member_size,member_list,new_member.studentId) != -1){
        printf("Da co member voi MSSV la %s.\nTao tai khoan that bai\n",new_member.studentId);
        Sleep(3000);
        system("cls");
        return;
    }
    Input_team(&new_member.team);
    Input_role(&new_member.role);

    Account new_account = Init_Account();
    strcpy(new_account.studentId,new_member.studentId);
    Input_password(new_account.password);
    new_account.role = new_member.role;
    
    create_account(*member_size,account_list,new_account);
    create_member(*member_size,member_list,new_member);
    ++(*member_size);
    printf("Create new member complete!");
    Sleep(3000);
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
    int tmp = 0,idx = -1; 
    while(tmp == 0 || idx == -1){
        printf("======================================\n");
        Input_studentId(studentId);
        if(tmp != 0)    idx = Find_studentId(*member_size,member_list,studentId);
        if(tmp == 0) Announcement_wrong_format();
        else if(idx == -1)   Announcement_uaivailable_member();
        else    break;
    }
    remove_member_account(member_size,member_list,account_list,idx);
    for(int i = 0;i < *violation_size;++i)   if(strcmp(studentId,violation_list[i].studentId)){
        remove_violation_index(violation_size,violation_list,i);
    }
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
        printf("Lua chon cua ban : ");
        int user_choose = 0;
        scanf("%d",&user_choose);
        clear_buffer();

        if(user_choose == 0)   break;
        else if(user_choose == 1)   Menu_create(member_size,member_list,account_list);
        else if(user_choose == 2)   Menu_update();
        else if(user_choose == 3)   Menu_remove(member_size,member_list,account_list,violation_size,violation_list);
        else    Announcement_unaivailable_option();
    }
}