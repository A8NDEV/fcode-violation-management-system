#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#define MAKE_DIR(p) mkdir(p, 0777)
#endif

static int Count_records(const char path[],size_t record_size){
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
int Count_deleted_members_dat(){
    return Count_records("data/deleted_members.dat",sizeof(Member));
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
bool Read_deleted_members_dat(const int size,Member member_list[]){
    if(size < 0)    return false;
    return (Read_dat("data/deleted_members.dat",size,member_list,sizeof(Member)) == size);
}

static bool Append_dat(const char path[],const void *record,size_t record_size){
    if(record == NULL)  return false;

    MAKE_DIR("data");

    FILE *file = fopen(path,"ab");
    if(file == NULL)    return false;
    size_t written = fwrite(record,record_size,1,file);
    fclose(file);
    return (written == 1);
}
bool Append_members_dat(const Member record){
    return Append_dat("data/members.dat",&record,sizeof(Member));
}
bool Append_accounts_dat(const Account record){
    return Append_dat("data/accounts.dat",&record,sizeof(Account));
}
bool Append_violations_dat(const Violation record){
    return Append_dat("data/violations.dat",&record,sizeof(Violation));
}
bool Append_deleted_members_dat(const Member record){
    return Append_dat("data/deleted_members.dat",&record,sizeof(Member));
}
bool Append_deleted_accounts_dat(const Account record){
    return Append_dat("data/deleted_accounts.dat",&record,sizeof(Account));
}

static bool Update_dat(const char path[],const int idx,const void *record,size_t record_size){
    if(idx < 0 || record == NULL) return false;
    int count = Count_records(path,record_size);
    if(count < 0 || idx >= count) return false;

    FILE *file = fopen(path,"rb+");
    if(file == NULL)    return false;
    if(fseek(file, (long)idx * record_size, SEEK_SET) != 0){
        fclose(file);
        return false;
    }
    size_t written = fwrite(record, record_size, 1, file);
    fclose(file);
    return (written == 1);
}
bool Update_members_dat(const int index,const Member record){
    return Update_dat("data/members.dat",index,&record,sizeof(Member));
}
bool Update_accounts_dat(const int index,const Account record){
    return Update_dat("data/accounts.dat",index,&record,sizeof(Account));
}
bool Update_violations_dat(const int index,const Violation record){
    return Update_dat("data/violations.dat",index,&record,sizeof(Violation));
}

static bool Rewrite_dat(const char path[],const int size,const void *record,size_t record_size){
    if(size < 0 || (size > 0 && record == NULL))  return false;

    MAKE_DIR("data");

    FILE *file = fopen(path,"wb");
    if(file == NULL)    return false;
    size_t written = fwrite(record,record_size,(size_t)size,file);
    fclose(file);
    return (written == (size_t)size);
}
bool Rewrite_members_dat(const int member_size,Member member_list[]){
    return Rewrite_dat("data/members.dat",member_size,member_list,sizeof(Member));
}
bool Rewrite_accounts_dat(const int member_size,Account account_list[]){
    return Rewrite_dat("data/accounts.dat",member_size,account_list,sizeof(Account));
}
bool Rewrite_violations_dat(const int violation_size,Violation violation_list[]){ 
    return Rewrite_dat("data/violations.dat",violation_size,violation_list,sizeof(Violation));
}
