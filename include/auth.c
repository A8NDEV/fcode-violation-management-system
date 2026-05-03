#include "auth.h"
//--------------------------------------------------------
// flow chạy của chương trình login - auth!
void menu(account *accountList, int *quantity, int *isLogin, account *session) {
  Menu currentState = startState;
  while (currentState != exitState) {
    switch (currentState) {
    case startState:
      start(accountList, quantity);
      currentState = loginState;
      break;
    case loginState:
      if (*isLogin == 0) {
        login(accountList, quantity, isLogin, &currentState, session);
      } else {
        setting(((*(session)).role), &currentState);
      }
      break;
    case logoutState:
      memset(session, 0, sizeof(account));
      printf("CHECK SESSION khi đã logout\n");
      printf("%s | %s | %d | %d\n", (*(session)).studentId,
             (*(session)).password, (*(session)).role, (*(session)).isLocked);
      printf(ANSI_BRIGHT_CYAN "ĐÃ ĐĂNG XUẤT TÀI KHOẢN!\n");
      printf("\n");
      *isLogin = 0;
      currentState = loginState;
      break;
    case changePasswordState:
      if (session->role == 0) {
        changePassword(accountList, session->role, quantity, session);
        printf(ANSI_BOLD ANSI_COLOR_YELLOW
               "\n[BẢO MẬT] MẬT KHẨU ĐÃ THAY ĐỔI. VUI LÒNG ĐĂNG NHẬP "
               "LẠI!\n" ANSI_COLOR_RESET);
        currentState = logoutState;
      } else {
        changePassword(accountList, session->role, quantity, session);
        currentState = loginState; // BCN giữ trạng thái đăng nhập
      }
      break;
    case exitState:
      break;
    }
  }
}

//--------------------------------------------------------
//--------------------------------------------------------
void start(account *accountList, int *quantity) {
  printf("-----------------------------\n");
  printf("ADMIN LOG!\n");
  printf("ĐANG KHỞI ĐỘNG CHƯƠNG TRÌNH AUTH.C\n");
  FILE *file = fopen("data/accounts.dat", "rb+");
  if (file == NULL) {
    printf("LỖI! KHÔNG TÌM THẤY ACCOUNTS.DAT.\n");
    exit(1);
  }

  printf("ĐÃ TÌM THẤY ACCOUNTS.DAT VÀ ĐANG XỬ LÝ...\n");
  while (fread(&accountList[*quantity], sizeof(account), 1, file) == 1) {
    printf("ĐỌC TỪ FILE THÀNH CÔNG 1 TÀI KHOẢN\n");
    (*quantity)++;
    if (*quantity >= MAX_ACCOUNT)
      break;
  }
  fclose(file);
  printf("TÌM THẤY TỔNG CỘNG: %d TÀI KHOẢN\n", *quantity);
  printf("-----------------------------\n");
}
void login(account *accountList, int *quantity, int *isLogin,
           Menu *currentState, account *session) {
  char studentIdInput[100], studentPasswordInput[MAX_PASS_LEN];
  while (1) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║             MENU ĐĂNG NHẬP             ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MÃ SINH VIÊN: " ANSI_COLOR_RESET);

    scanf(" %99[^\n]", studentIdInput);
    while (getchar() != '\n')
      ;

    if (strlen(studentIdInput) != 8) {
      printf("\n" ANSI_COLOR_RED
             "MÃ SINH VIÊN PHẢI ĐÚNG 8 KÝ TỰ! (Độ dài bạn nhập: %lu)\n",
             strlen(studentIdInput));
      continue;
    }

    int foundIndex = -1;
    for (int i = 0; i < *quantity; i++) {
      if (strcmp(accountList[i].studentId, studentIdInput) == 0) {
        foundIndex = i;
        break;
      }
    }

    if (foundIndex == -1) {
      printf("\n" ANSI_COLOR_RED
             "KHÔNG TÌM THẤY MÃ SINH VIÊN NÀY TRONG DANH SÁCH!\n");
      continue;
    }

    if (accountList[foundIndex].isLocked ==
        1) { // check xem mssv này có bị khóa khong
      printf(ANSI_BRIGHT_RED "TÀI KHOẢN ĐANG BỊ KHÓA\n");
      *currentState = loginState;
      return;
    }
    // bug
    printf(ANSI_BOLD ANSI_COLOR_YELLOW
           " ❯ MẬT KHẨU    : " ANSI_COLOR_RESET); // SẼ ADD CƠ CHẾ THAY MẬT KHẨU
                                                  // THÀNH *
    scanf(" %49[^\n]", studentPasswordInput); 
    while (getchar() != '\n');

    if (strcmp(accountList[foundIndex].password, studentPasswordInput) ==
        0) { // k khóa thì check password ng dùng nhập vào
      printf("\n");
      printf(ANSI_BRIGHT_GREEN "ĐÃ ĐĂNG NHẬP THÀNH CÔNG!\n");
      *isLogin = 1;
      accountList[foundIndex].failCount = 0;
      *session = accountList[foundIndex]; // lấy session
      ///// kỉm tra lấy session ok chưa!
      printf("\n");
      printf("CHECK SESSION lúc đăng nhập thành công\n");
      printf("%s | %s | %d | %d\n", (*(session)).studentId,
             (*(session)).password, (*(session)).role, (*(session)).isLocked);
      printf("\n");

      // persist reset failCount (đồng bộ file)
      FILE *file_upd = fopen("data/accounts.dat", "rb+");
      if (file_upd != NULL) {
        fseek(file_upd, (long)foundIndex * sizeof(account), SEEK_SET);
        fwrite(&accountList[foundIndex], sizeof(account), 1, file_upd);
        fclose(file_upd);
      }

    } else {
      printf("\n");
      printf(ANSI_BRIGHT_RED "MẬT KHẨU KHÔNG ĐÚNG!\n"); //
      (accountList[foundIndex].failCount)++;

      // Persist change to file immediately
      FILE *file_upd = fopen("data/accounts.dat", "rb+");
      if (file_upd != NULL) {
        fseek(file_upd, (long)foundIndex * sizeof(account), SEEK_SET);
        fwrite(&accountList[foundIndex], sizeof(account), 1, file_upd);
        fclose(file_upd);
      }

      if (accountList[foundIndex].failCount >= 3) {
        accountList[foundIndex].isLocked = 1;

        // Persist lock to file
        FILE *file_lock = fopen("data/accounts.dat", "rb+");
        if (file_lock != NULL) {
          fseek(file_lock, (long)foundIndex * sizeof(account), SEEK_SET);
          fwrite(&accountList[foundIndex], sizeof(account), 1, file_lock);
          fclose(file_lock);
        }

        printf("\n");
        printf(ANSI_BRIGHT_RED
               "TÀI KHOẢN ĐÃ BỊ KHÓA DO NHẬP SAI MẬT KHẨU 3 LẦN!\n");
        printf(ANSI_BRIGHT_BLUE
               "ĐANG THOÁT KHỎI CHƯƠNG TRÌNH DO NHẬP SAI 3 LẦN!\n");
        printf("\n");
        *currentState = exitState;
        return;
      }
    }
    break;
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
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET
                                      "CHỨC NĂNG MEMBER\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET
                                      "ĐỔI MẬT KHẨU\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " [3] " ANSI_COLOR_RESET "ĐĂNG XUẤT\n");
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD
           "------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW
           " ❯ HÃY CHỌN CHỨC NĂNG: " ANSI_COLOR_RESET);
    if (scanf("%d", &choice) != 1) {
      choice = 0; // Gán giá trị không hợp lệ nếu nhập chữ
    }
    while (getchar() != '\n')
      ; // Xóa bộ đệm để tránh treo chương trình
    if (choice == 1) {
      /// chưa có import
    } else if (choice == 2) {
      *currentState = changePasswordState;
    } else if (choice == 3) {
      *currentState = logoutState;
    }
  } else {
    int choice;
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║           MENU BAN CHỦ NHIỆM           ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET
                                      "CHỨC NĂNG BAN CHỦ NHIỆM\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET
                                      "ĐỔI MẬT KHẨU CHO SINH VIÊN THEO MSSV\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " [3] " ANSI_COLOR_RESET "ĐĂNG XUẤT\n");
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD
           "------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW
           " ❯ HÃY CHỌN CHỨC NĂNG: " ANSI_COLOR_RESET);
    if (scanf("%d", &choice) != 1) {
      choice = 0;
    }
    while (getchar() != '\n')
      ;
    if (choice == 1) {
      // chưa có import
    } else if (choice == 2) {
      *currentState = changePasswordState;
    } else if (choice == 3) {
      *currentState = logoutState;
    }
  }
}
void changePassword(account *accountList, int role, int *quantity,
                    account *session) {
  int found = 0;
  int foundIndex = -1; // <-- initialize to -1
  if (role == 0) {
    char passwordChange[MAX_PASS_LEN], oldPassword[MAX_PASS_LEN];
    printf("\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ MẬT KHẨU CŨ : " ANSI_COLOR_RESET);
    scanf(" %49[^\n]", oldPassword);
    while (getchar() != '\n');
    if (strcmp(oldPassword, session->password) == 0) {
      printf("\n");
      printf(ANSI_BRIGHT_GREEN "XÁC MINH MẬT KHẨU CŨ THÀNH CÔNG!\n");
      printf("\n");
      printf(ANSI_BOLD ANSI_COLOR_YELLOW
             " ❯ NHẬP MẬT KHẨU CẦN ĐỔI : " ANSI_COLOR_RESET); // bị trôi lệnh
      scanf(" %49[^\n]", passwordChange);
      while (getchar() != '\n');
      printf("\n");
      printf(ANSI_BRIGHT_CYAN "ĐANG THỰC HIỆN ĐỔI..\n");

      // dùng loop tìm account session trong accountList vị trí thứ mấy sau đó
      // sẽ đổi mk ở accountList[vi tri tìm thấy] và ghi vào file => đồng bộ
      // trạng thái
      for (int i = 0; i < *quantity; i++) {
        if (strcmp(accountList[i].studentId, session->studentId) == 0) {
          foundIndex = i;
          break;
        }
      }
      if (foundIndex != -1) {
        strcpy(session->password, passwordChange);
        strcpy(accountList[foundIndex].password, passwordChange);
        // Persist by seeking directly to record offset
        FILE *file = fopen("data/accounts.dat", "rb+");
        if (file != NULL) {
          fseek(file, (long)foundIndex * sizeof(account), SEEK_SET);
          fwrite(&accountList[foundIndex], sizeof(account), 1, file);
          fclose(file);
        }
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "ĐỔI MẬT KHẨU THÀNH CÔNG!\n");
        printf("\n");
      } else {
        // not found
        printf(ANSI_BRIGHT_RED "KHÔNG TÌM THẤY TÀI KHOẢN TRONG HỆ THỐNG\n");
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
    printf(ANSI_BOLD ANSI_COLOR_YELLOW
           " ❯ NHẬP MÃ SINH VIÊN CẦN ĐỔI: " ANSI_COLOR_RESET);
    char studentIDneedtochangePassword[MAX_ID_LEN];
    scanf(" %8[^\n]", studentIDneedtochangePassword);
    while (getchar() != '\n');
    for (int i = 0; i < *quantity; i++) {
      if (strcmp(accountList[i].studentId, studentIDneedtochangePassword) ==
          0) {
        found = 1;
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "ĐÃ XÁC THỰC MÃ SINH VIÊN CẦN ĐỔI!: %s\n",
               studentIDneedtochangePassword);
        printf("\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW
               " ❯ NHẬP MẬT KHẨU CẦN ĐỔI CHO SINH VIÊN %s: ",
               studentIDneedtochangePassword); // bị trôi lệnh
        char passwordChange[MAX_PASS_LEN];
        scanf(" %49[^\n]", passwordChange);
        while (getchar() != '\n');
        printf("\n");
        printf(ANSI_BRIGHT_CYAN "ĐANG THỰC HIỆN ĐỔI..\n");
        strcpy(accountList[i].password, passwordChange);
        FILE *file = fopen("data/accounts.dat", "rb+");
        if (file != NULL) {
          fseek(file, (long)i * sizeof(account), SEEK_SET);
          fwrite(&accountList[i], sizeof(account), 1, file);
          fclose(file);
        }
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "ĐỔI MẬT KHẨU THÀNH CÔNG CHO %s\n",
               studentIDneedtochangePassword);
        printf("\n");
        break;
      }
    }
    if (found == 0) {
      printf("\n");
      printf(ANSI_BRIGHT_RED
             "KHÔNG TÌM THẤY MÃ SINH VIÊN CẦN ĐỔI TRONG HỆ THỐNG\n");
      printf("\n");
    }
  }
}
