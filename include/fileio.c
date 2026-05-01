#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

static int Count_records(const char path[],size_t record_size){
    //function to count records in specific binary file
    FILE *file = fopen(path,"rb");
    if(file == NULL)    return 0;

    if(fseek(file,0,SEEK_END) != 0){
        fclose(file);
        return 0;
    }

    long file_size = ftell(file);
    fclose(file);

    if(file_size < 0)   return 0;
    if((size_t)file_size % record_size != 0)    return -1;
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

static int Read_dat(const char path[],const int n,void *list,size_t record_size){
    if(list == NULL)    return 0;

    FILE *file = fopen(path,"rb");
    if(file == NULL)    return 0;
    size_t n_read = fread(list,record_size,(size_t)n,file);
    fclose(file);
    return (int)n_read;
}
bool Read_members_dat(const int size,Member member_list[]){
    if(size < 0)    return false;
    return (Read_dat("data/members.dat",size,member_list,sizeof(Member)) == size);
}
bool Read_accounts_dat(const int size,Account account_list[]){
    if(size < 0)    return false;
    return (Read_dat("data/accounts.dat",size,account_list,sizeof(Account)) == size);
}
bool Read_violations_dat(const int size,Violation violation_list[]){
    if(size < 0)    return false;
    return (Read_dat("data/violations.dat",size,violation_list,sizeof(Violation)) == size);
}
