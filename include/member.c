#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <member.h>





void view_Profile (Member currentuser) {
    printf("\n===============HO SO CA NHAN===============\n");
    printf("\nHO VA TEN     :  %s\n",currentuser.fullName);
    printf("\nMSSV          :  %s\n",currentuser.studentId);
    printf("\nEMAIL         :  %s\n",currentuser.email);
    printf("\nSDT           :  %s\n",currentuser.phone);
}



void view_CLB_Profile (Member member[], int size_memberlist) {
    printf("\n===============HO SO DANH SACH THANH VIEN CLB===============\n");
    for (int i = 0; i < size_memberlist; i++) {
        printf("\nHO VA TEN         :   %s\n",member[i].fullName);
        if (member[i].team == 0) {
            printf("BAN           :   ACADEMIC\n");
        } else if ( member[i].team == 1) {
            printf("BAN           :   PLANNING\n");
        } else if (member[i].team == 2) {
            printf("BAN           :   HR\n");
        } else if (member[i].team == 3 ) {
            printf("BAN           :   MEDIA\n");
        } else {
            printf("\nKHONG XAC DINH BAN THUOC VE\n");
        }
        if (member[i].role == 0) {
            printf("CHUC VU       :   MEMBER\n");
        } else if ( member[i].role == 1) {
            printf("CHUC VU       :   LEADER/VICE\n");
        } else if ( member[i].role == 2) {
            printf("CHUC VU       :   BAN CHU NHIEM\n");
        } else {
            printf("KHONG XAC DINH CHUC VU\n");
        }
    }
}












    











