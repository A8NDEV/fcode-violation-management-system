#include "auth.h"
//--------------------------------------------------------
// flow chạy của chương trình login - auth!
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
                    setting(((*(session)).role),&currentState);
                }   
                break;
            case logoutState:
                memset(session, 0, sizeof(account));
                printf("CHECK SESSION khi đã logout\n");
                printf("%s | %s | %d | %d\n",(*(session)).studentId, (*(session)).password , (*(session)).role , (*(session)).isLocked);
                printf(ANSI_BRIGHT_CYAN "ĐÃ ĐĂNG XUẤT TÀI KHOẢN!\n");
                printf("\n");
                *isLogin = 0;
                currentState = loginState;
                break;
            case changePasswordState:
                changePassword(accountList, ((*(session)).role), quantity,session);
                currentState = loginState;
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
    printf("ĐANG KHỞI ĐỘNG CHƯƠNG TRÌNH AUTH.C\n");
    FILE *file = fopen("data/accounts.dat", "rb+"); 
    if (file == NULL) {
        printf("LỖI! KHÔNG TÌM THẤY ACCOUNTS.DAT.\n");
        exit(1);
    }

    printf("ĐÃ TÌM THẤY ACCOUNTS.DAT VÀ ĐANG XỬ LÝ...\n");
    while (fread(&accountList[*quantity], sizeof(account),1 , file) == 1) {
        printf("ĐỌC TỪ FILE THÀNH CÔNG 1 TÀI KHOẢN\n");
        (*quantity)++;
        if (*quantity >= MAX_ACCOUNT) 
            break;
    }
    fclose(file);
    printf("TÌM THẤY TỔNG CỘNG: %d TÀI KHOẢN\n",*quantity);
    printf("-----------------------------\n");

}
void login(account *accountList,int *quantity,int *isLogin, Menu *currentState, account *session) {
    char studentIdInput[MAX_ID_LEN], studentPasswordInput[MAX_PASS_LEN];
    int failCount = 0;
    while (1) {
        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════╗\n");
        printf("║             MENU ĐĂNG NHẬP             ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");

        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MÃ SINH VIÊN: " ANSI_COLOR_RESET); 
        scanf(" %8[^\n]", studentIdInput); // THIẾU CẮT KHOẢNG TRẮNG ĐẦU ĐUÔI KHI INPUT    
        
        int foundIndex = -1;

        for (int i = 0 ; i < *quantity; i++) {
            if (strcmp(accountList[i].studentId,studentIdInput) == 0) { // tìm thấy mssv cần login
                foundIndex = i;
                break;
            }
        }
        if (foundIndex == -1) {
            printf("\n");
            printf(ANSI_COLOR_RED "MÃ SINH VIÊN BẠN NHẬP KHÔNG HỢP LỆ\n");
        }


        if (foundIndex != -1) {
            if (accountList[foundIndex].isLocked == 1) { // check xem mssv này có bị khóa khong
                printf(ANSI_BRIGHT_RED "TÀI KHOẢN ĐANG BỊ KHÓA\n");
                *currentState = loginState;
                return;
            }
            
            printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MẬT KHẨU    : " ANSI_COLOR_RESET);  // SẼ ADD CƠ CHẾ THAY MẬT KHẨU THÀNH *
            scanf(" %49[^\n]", studentPasswordInput); // THIẾU CẮT KHOẢNG TRẮNG ĐẦU ĐUÔI KHI INPUT
            

            if (strcmp(accountList[foundIndex].password,studentPasswordInput) == 0) { // k khóa thì check password ng dùng nhập vào
                printf("\n");
                printf(ANSI_BRIGHT_GREEN  "ĐÃ ĐĂNG NHẬP THÀNH CÔNG!\n");
                *isLogin = 1;
                accountList[foundIndex].failCount = 0;
                *session = accountList[foundIndex]; // lấy session
                ///// kỉm tra lấy session ok chưa!
                printf("\n");
                printf("CHECK SESSION lúc đăng nhập thành công\n");
                printf("%s | %s | %d | %d\n",(*(session)).studentId, (*(session)).password , (*(session)).role , (*(session)).isLocked);
                printf("\n");
                
            }
            else {
            printf("\n");
            printf(ANSI_BRIGHT_RED  "MẬT KHẨU KHÔNG ĐÚNG!\n");
            (accountList[foundIndex].failCount)++; 
            if (accountList[foundIndex].failCount >= 3) {
                accountList[foundIndex].isLocked = 1;
                printf("\n");
                printf(ANSI_BRIGHT_RED "TÀI KHOẢN ĐÃ BỊ KHÓA DO NHẬP SAI MẬT KHẨU 3 LẦN!\n");
                printf(ANSI_BRIGHT_BLUE "ĐANG THOÁT KHỎI CHƯƠNG TRÌNH DO NHẬP SAI 3 LẦN!\n");
                printf("\n");
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
        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════╗\n");
        printf("║             MENU MEMBER                ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "CHỨC NĂNG MEMBER\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "ĐỔI MẬT KHẨU\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [3] " ANSI_COLOR_RESET "ĐĂNG XUẤT\n");
        printf("\n");
        printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ HÃY CHỌN CHỨC NĂNG: " ANSI_COLOR_RESET);
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
        printf(ANSI_COLOR_CYAN ANSI_BOLD);
        printf("╔════════════════════════════════════════╗\n");
        printf("║           MENU BAN CHỦ NHIỆM           ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf(ANSI_COLOR_RESET "\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET "CHỨC NĂNG MEMBER\n");
        printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET "ĐỔI MẬT KHẨU CHO SINH VIÊN THEO MSSV\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " [3] " ANSI_COLOR_RESET "ĐĂNG XUẤT\n");
        printf("\n");
        printf(ANSI_COLOR_CYAN ANSI_BOLD "------------------------------------------\n" ANSI_COLOR_RESET);
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ HÃY CHỌN CHỨC NĂNG: " ANSI_COLOR_RESET);
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
void changePassword(account *accountList,int role, int *quantity,account *session) {
    int found = 0;
    int foundIndex = 0;
    if (role == 0) {
        char passwordChange[MAX_PASS_LEN], oldPassword[MAX_PASS_LEN];
            printf("\n");
            printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MẬT KHẨU CŨ : " ANSI_COLOR_RESET); 
            scanf(" %49[^\n]",oldPassword);
            if (strcmp(oldPassword,session->password) == 0) {
                printf("\n");
                printf(ANSI_BRIGHT_GREEN "XÁC MINH MẬT KHẨU CŨ THÀNH CÔNG!\n");
                printf("\n");
                printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ NHẬP MẬT KHẨU CẦN ĐỔI : " ANSI_COLOR_RESET); // bị trôi lệnh
                scanf(" %49[^\n]",passwordChange);
                printf("\n");
                printf(ANSI_BRIGHT_CYAN "ĐANG THỰC HIỆN ĐỔI..\n");

                // dùng loop tìm account session trong accountList vị trí thứ mấy sau đó 
                // sẽ đổi mk ở accountList[vi tri tìm thấy] và ghi vào file => đồng bộ trạng thái
                for (int i = 0; i < *quantity; i++) {
                    if (strcmp(accountList[i].studentId, session->studentId) == 0) {
                        foundIndex = i;
                        break;
                    }
                }
                if (foundIndex != -1) {
                    strcpy(session->password, passwordChange);
                    strcpy(accountList[foundIndex].password, passwordChange);
                }
                FILE *file = fopen("data/accounts.dat", "rb+");
                if (file != NULL) {
                    account tempAcc;
                    while (fread(&tempAcc, sizeof(account), 1, file) == 1) {
                        if (strcmp(tempAcc.studentId, session->studentId) == 0) {
                            fseek(file, -sizeof(account), SEEK_CUR);
                            fwrite(session, sizeof(account), 1, file);
                            break;
                        }
                    }
                    fclose(file);
                    printf("\n");
                    printf(ANSI_BRIGHT_GREEN "ĐỔI MẬT KHẨU THÀNH CÔNG!\n");
                    printf("\n");
                
                }
            }
                
            else {
                printf("\n");
                printf(ANSI_BRIGHT_RED "BẠN ĐÃ NHẬP SAI MẬT KHẨU\n");
                printf("\n");
        
        
    }
    
}   
    
    else {
        printf("\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ NHẬP MÃ SINH VIÊN CẦN ĐỔI: " ANSI_COLOR_RESET); 
        char studentIDneedtochangePassword[MAX_ID_LEN];
        scanf(" %[^\n]",studentIDneedtochangePassword);
        char passwordChange[100];
        for (int i = 0 ; i < *quantity ; i++) {
            if (strcmp(accountList[i].studentId,studentIDneedtochangePassword) == 0) {
                found = 1;
                printf("\n");
                printf(ANSI_BRIGHT_GREEN "ĐÃ XÁC THỰC MÃ SINH VIÊN CẦN ĐỔI!: %s\n",studentIDneedtochangePassword);
                printf("\n");
                printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ NHẬP MẬT KHẨU CẦN ĐỔI CHO SINH VIÊN %s: ",studentIDneedtochangePassword); // bị trôi lệnh
                char passwordChange[MAX_PASS_LEN];
                scanf(" %49[^\n]",passwordChange);
                printf("\n");
                printf(ANSI_BRIGHT_CYAN "ĐANG THỰC HIỆN ĐỔI..\n");
                strcpy(accountList[i].password, passwordChange);
                FILE *file = fopen("data/accounts.dat", "rb+");
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
                printf("\n");
                printf(ANSI_BRIGHT_GREEN "ĐỔI MẬT KHẨU THÀNH CÔNG CHO %s\n", studentIDneedtochangePassword);
                printf("\n");
                break; 
                
        
            }
        }
        if (found == 0) {
        printf("\n");
        printf(ANSI_BRIGHT_RED "KHÔNG TÌM THẤY MÃ SINH VIÊN CẦN ĐỔI TRONG HỆ THỐNG\n");
        printf("\n");
        }
    }
}

