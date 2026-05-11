#ifndef AUTH_H
#define AUTH_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_ID_LEN 9
#define MAX_PASS_LEN 50
#define MAX_ACCOUNT 1000



/// COLOR PALETTE
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD          "\x1b[1m"
// 3. Regular Foreground Colors
// ==========================================
#define ANSI_COLOR_BLACK    "\x1b[30m"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"  // Pink Magenta
#define ANSI_COLOR_CYAN     "\x1b[36m"  // Light Cyan
#define ANSI_COLOR_WHITE    "\x1b[37m"

// 4. Bright Foreground Colors
// ==========================================
#define ANSI_BRIGHT_BLACK   "\x1b[90m"  // Dark Gray
#define ANSI_BRIGHT_RED     "\x1b[91m"
#define ANSI_BRIGHT_GREEN   "\x1b[92m"
#define ANSI_BRIGHT_YELLOW  "\x1b[93m"
#define ANSI_BRIGHT_BLUE    "\x1b[94m"
#define ANSI_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_BRIGHT_CYAN    "\x1b[96m"
#define ANSI_BRIGHT_WHITE   "\x1b[97m"


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
void changePassword(account *accountList,int role, int *quantity,account *session);
void saveAccount(int index, account *acc);
void inputString(char *buffer, int size);

#endif 