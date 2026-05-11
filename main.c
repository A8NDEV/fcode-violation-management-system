#include "include/auth.h"
#include <windows.h>
int main() {
//--------------------------------------------------------
    account accountList[MAX_ACCOUNT];
    account session[1];
    int quantity = 0;
    int isLogin = 0;
//--------------------------------------------------------
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    menu(accountList,&quantity,&isLogin,session);
//--------------------------------------------------------
    return 0;
}
/*gcc main.c include/fileio.c include/validate.c  include/types.c include/member.c include/utils.c -Iinclude -o main.exe
./fcode_program
*/
