#include "auth.h"
//--------------------------------------------------------
//FLOW9 CỦA CHƯƠNG TRÌNH
void menu(account *accountList,int *quantity,int *isLogin,account *session) {
    Menu currentState = startState;
    while (currentState != exitState) {
        switch(currentState) {
            case startState:
                start(accountList,quantity);
                currentState = loginState;
                break;
            case loginState:
                if (*isLogin == 0) {
                    login(accountList,quantity,isLogin,&currentState,session);
                }
                else {
                    // thêm mở setting khi đã login r
                    setting(((*(session)).role),&currentState);
                    currentState = exitState; // thíu 1 state mở menu khi phát hiện đã logout rồi
                }   
                break;
            case logoutState:
                memset(session, 0, sizeof(account));
                printf("CHECK SESSION khi đã logout\n");
                printf("%s | %s | %d | %d\n",(*(session)).studentId, (*(session)).password , (*(session)).role , (*(session)).isLocked);
                printf("ĐÃ ĐĂNG XUẤT TÀI KHOẢN!\n");
                *isLogin = 0;
                currentState = loginState;
                break;
            case changePasswordState:
                changePassword(accountList, ((*(session)).role), quantity);
                currentState = loginState; // thêm cái này vô 
                break;
            case exitState:
                break;
        }
    }
}

//--------------------------------------------------------
//--------------------------------------------------------
void start(account *accountList,int *quantity) {
    printf("-----------------------------\n");
    printf("ADMIN LOG!\n");
    printf("ĐANG KHỞI ĐỘNG CHƯƠNG TRÌNH\n");
    FILE *file = fopen("data/accounts.dat", "rb+"); 
    if (file == NULL) {
        printf("Lỗi ! Không tìm thấy file accounts.dat!\n");
        exit(1);
    }

    printf("Đã tìm thấy accounts.dat và đang xử lý......\n");
    while (fread(&accountList[*quantity], sizeof(account),1 , file) == 1) {
        printf("Đã đọc thành công 1 tài khoản\n");
        (*quantity)++;
        if (*quantity >= MAX_ACCOUNT) break;
    }
    printf("Đã đọc được tổng cộng %d tài khoản\n",*quantity);
    printf("-----------------------------\n");

}
void login(account *accountList,int *quantity,int *isLogin, Menu *currentState, account *session) {
    char studentIdInput[MAX_ID_LEN], studentPasswordInput[MAX_PASS_LEN];
    int failCount = 0;
    while (1) {
        printf("MENU ĐĂNG NHẬP\n");
        printf("Mã sinh viên: "); scanf(" %8[^\n]",studentIdInput);
        printf("Mật khẩu: "); scanf(" %49[^\n]",studentPasswordInput);

        int foundIndex = -1;


        for (int i = 0 ; i < *quantity; i++) {
            if (strcmp((*(accountList + i)).studentId,studentIdInput) == 0) { // tìm thấy mssv cần login
                foundIndex = i;
                break;
            }
        }
        if (foundIndex != -1) {
            if ((*(accountList + foundIndex)).isLocked == 1) { // check xem mssv này có bị khóa khong
                printf("Tài khoản đang bị khóa!!\n");
                *currentState = exitState;
                return;
            }
            if (strcmp((*(accountList + foundIndex)).password,studentPasswordInput) == 0) { // k khóa thì check password ng dùng nhập vào
                printf("Đã đăng nhập thành công!\n");
                *isLogin = 1;
                (*(accountList + foundIndex)).failCount = 0;
                *session = *(accountList + foundIndex); // lấy session
                ///// kỉm tra lấy session ok chưa!
                printf("CHECK SESSION lúc đăng nhập thành công\n");
                printf("%s | %s | %d | %d\n",(*(session)).studentId, (*(session)).password , (*(session)).role , (*(session)).isLocked);

                //theo session ta lấy role của session đó và thông báo tương ứng
                if ((*(session)).role == 0) printf("MENU MEMBER\n");
                else printf("MENU BAN CHỦ NHIỆM\n");

                setting((*(session)).role,currentState);
            }
            else {
            printf("Mật khẩu không đúng!\n");
            ((*(accountList + foundIndex)).failCount)++; 
            if ((*(accountList + foundIndex)).failCount >= 3) {
                (*(accountList + foundIndex)).isLocked = 1;
                printf("TÀI KHOẢN ĐÃ BỊ KHÓA DO NHẬP SAI MẬT KHẨU 3 LẦN!\n");
                printf("ĐANG THOÁT KHỎI CHƯƠNG TRÌNH DO NHẬP SAI 3 LẦN!\n");
                *currentState = exitState;
                return;
                }
            }
        break;
        }
    } 
}
void setting(int role, Menu *currentState) {
    if (role == 0) {
        int choice;
        printf("---------------------------------------\n");
        printf("Chức năng có sẵn dành cho Member F-Code\n");
        printf(" 1.Menu Member F-Code\n"); //gọi hàm menu member mà mấy ô kia làm / vd memberMenu();
        printf(" 2.Đổi mật khẩu\n");
        printf(" 3.Đăng xuất khỏi tài khoản hiện tại\n");
        printf("---------------------------------------\n");
        printf("Hãy chọn chức năng: ");
        scanf("%d",&choice);
        if (choice == 1) {
            ///chưa có import
        }
        else if (choice == 2) {
            *currentState = changePasswordState;
        }
        else if (choice == 3) {
            *currentState = logoutState;
        }
    }
    else {
        int choice;
        printf("---------------------------------------\n");
        printf("Chức năng có sẵn dành cho Ban chủ nhiệm F-Code\n");
        printf(" 1.Ban chủ nhiệm F-Code\n"); //gọi hàm menu bcn mà mấy ô kia làm / vd bcnMenu();
        printf(" 2.Đổi mật khẩu (BCN)\n");
        printf(" 3.Đăng xuất khỏi tài khoản hiện tại\n");
        printf("---------------------------------------\n");
        printf("Hãy chọn chức năng: ");
        scanf("%d",&choice);
        if (choice == 1) {
            // chưa có import
        }
        else if (choice == 2) {
            *currentState = changePasswordState;
        }
        else if (choice == 3) {
            *currentState = logoutState;
        }
    }
}
void changePassword(account *accountList,int role, int *quantity) {
    if (role == 0) {
        printf("Bạn là member F-Code\n");
        printf("Hãy nhập mã sinh viên của bạn: ");
        char studentIdFind[MAX_ID_LEN];
        scanf(" %8[^\n]",studentIdFind);
        printf("Hãy nhập mật khẩu cũ: ");
        char password[100];
        scanf(" %49[^\n]",password);
        if (strcmp((*(accountList)).password,password) == 0) {
            printf("Mật khẩu bạn nhập hợp lệ\n");
            printf("Nhập mật khẩu bạn muốn đổi: ");
            char passwordChange[100];
            scanf(" %99[^\n]",passwordChange);
            strcpy(passwordChange,(*(accountList)).password);
            printf("Đã đổi mật khẩu thành công!\n");
            // ghi mat khau vo file
            FILE *file = fopen("accounts.dat", "rb+"); // rb+ cho phép vừa đọc vừa ghi đè mà k xóa nd cũ của file dat
            if (file == NULL) {
            printf("Có lỗi xảy ra khi đổi mật khẩu.\n");
            exit(0);
            int found = 0;
            while (fread(&accountList, sizeof(accountList),1,file) == 1) {
                if (strcmp((*(accountList)).studentId, studentIdFind) == 0) {
                    found = 1;
                    // Cập nhật mật khẩu mới vào Struct trên RAM (dùng strncpy cho an toàn)
                    strncpy((*(accountList)).password, passwordChange, sizeof((*(accountList)).password) - 1);
                    (*(accountList)).password[sizeof((*(accountList)).password) - 1] = '\0';

                    fseek(file, -sizeof(*accountList), SEEK_CUR);

                    fwrite(&accountList,sizeof(*accountList),1,file);
                    if (strcmp((*(accountList)).password,passwordChange) == 0) printf("Đã đổi mật khẩu thành công!\n");
                    break;
                }
            }
            if (found != 1) {
                printf("Đổi mk k thành công!\n");
                }
            } 
        }
    } 

    else {
        printf("Bạn là Ban chủ nhiệm F-Code\n");
        printf("Nhập mã sinh viên của thành viên cần đổi: ");
        char studentIDneedtochangePassword[MAX_ID_LEN];
        scanf(" %[^\n]",studentIDneedtochangePassword);
        char passwordChange[100];
        int found = 0;
        for (int i = 0 ; i < *quantity ; i++) {
            if (strcmp(accountList[i].studentId,studentIDneedtochangePassword) == 0) {
                found = 1;
                printf("Đã tìm thấy mã sinh viên cần đổi: %s\n",studentIDneedtochangePassword);
                printf("Nhập mật khẩu cần đổi cho sinh viên %s: ",studentIDneedtochangePassword); // bị trôi lệnh
                char passwordChange[MAX_PASS_LEN];
                scanf(" %49[^\n]",passwordChange);
                printf("Đang thực hiện đổi\n");
                strcpy(accountList[i].password, passwordChange);
                printf("Đã thực hiện đổi thành công!\n");
                FILE *file = fopen("accounts.dat", "rb+");
                if (file != NULL) {
                    account tempAcc;
                    while (fread(&tempAcc, sizeof(account), 1, file) == 1) {
                        if (strcmp(tempAcc.studentId, studentIDneedtochangePassword) == 0) {
                            fseek(file, -sizeof(account), SEEK_CUR);
                            fwrite(&accountList[i], sizeof(account), 1, file);
                            break;
                        }
                    }
                    fclose(file);
                }
                
                printf("Đã reset mật khẩu của %s thành công!\n", studentIDneedtochangePassword); 
                break; 
                
        
            }
        }
        if (found == 0) {
        printf("Không tìm thấy sinh viên %s trong hệ thống!\n", studentIDneedtochangePassword);
}
    }
}

