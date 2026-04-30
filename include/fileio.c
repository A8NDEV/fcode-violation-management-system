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
    if(file_size < 0)   return 0;
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

