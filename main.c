#include "auth.h"

int main() {
//--------------------------------------------------------
    account accountList[MAX_ACCOUNT];
    account session[1];
    int quantity = 0;
    int isLogin = 0;
//--------------------------------------------------------
    menu(accountList,&quantity,&isLogin,session);
//--------------------------------------------------------
    return 0;
}