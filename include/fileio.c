#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

static int Count_records(const char path[],int record_size){
    //function to count records in specific binary file
    FILE *file = fopen(path,"rb");
    if(file == NULL)    return 0;

    if(fseek(file,0,SEEK_END) != 0){
        fclose(file);
        return 0;
    }

    int file_size = ftell(file);
    if(file_size < 0){
        fclose(file);
        return 0;
    }
    fclose(file);
    return (int)file_size / record_size;
}
int Count_members_dat(){
    return Count_records("data/members.dat",sizeof(Member));
}
int Count_accounts_dat(){
    return Count_records("data/accounts.dat",sizeof(Account));
}
int Count_violations_dat(){
    return Count_records("data/violations.dat",sizeof(Violation));
}

static int Read_dat(const char path[],const int n,void *list,int record_size){
    if(n < 0)    return 0;
    FILE *file = fopen(path,"rb");
    if(file == NULL)    return 0;
    size_t n_read = fread(list,record_size,n,file);
    fclose(file);
    return (int)n_read;
}
int Read_members_dat(const int size,Member member_list[]){
    return Read_dat("data/members.dat",size,member_list,sizeof(Member));
}
int Read_accounts_dat(const int size,Account account_list[]){
    return Read_dat("data/accounts.dat",size,account_list,sizeof(Account));
}
int Read_violations_dat(const int size,Violation violation_list[]){
    return Read_dat("data/violations.dat",size,violation_list,sizeof(Violation));
}