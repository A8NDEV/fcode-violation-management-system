#include "include/auth.h"

int main() {
//--------------------------------------------------------
    Account accountList[MAX_ACCOUNT];
    Account session[1];
    int quantity = 0;
    int isLogin = 0;
//--------------------------------------------------------
    menu(accountList,&quantity,&isLogin,session);
//--------------------------------------------------------
    return 0;
}
/*gcc main.c include/auth.c -o fcode_program
./fcode_program
*/
