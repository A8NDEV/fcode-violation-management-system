#ifndef AUTH_H
#define AUTH_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h> 


#define MAX_ID_LEN 9
#define MAX_PASS_LEN 50
#define MAX_ACCOUNT 1000


struct account {
    char studentId[MAX_ID_LEN];
    char password[MAX_PASS_LEN];
    int role;  
    int isLocked; 
    int failCount; 
}; 
typedef struct account account;

enum menuState { 
    startState,
    loginState,
    logoutState,
    changePasswordState,
    exitState
};
typedef enum menuState Menu;


void start(account *accountList, int *quantity);
void login(account *accountList, int *quantity, int *isLogin, Menu *currentState, account *session);
void setting(int role, Menu *currentState);
void menu(account *accountList, int *quantity, int *isLogin, account *session);
void changePassword(account *accountList, int role, int *quantity);

#endif 