#include "auth.h"
//--------------------------------------------------------
// Auth - Login program flow!
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
      printf("CHECK SESSION after logout\n");
      printf("%s | %s | %d | %d\n", (*(session)).studentId,
             (*(session)).password, (*(session)).role, (*(session)).isLocked);
      printf(ANSI_BRIGHT_CYAN "LOGGED OUT SUCCESSFULLY!\n");
      printf("\n");
      *isLogin = 0;
      currentState = loginState;
      break;
    case changePasswordState:
      if (session->role == 0) {
        if (changePassword(accountList, session->role, quantity, session)) {
          printf(ANSI_BOLD ANSI_COLOR_YELLOW
                 "\n[SECURITY] PASSWORD CHANGED. PLEASE LOGIN "
                 "AGAIN!\n" ANSI_COLOR_RESET);
          currentState = logoutState;
        } else {
          currentState = loginState; // Stay logged in if failed
        }
      } else {
        changePassword(accountList, session->role, quantity, session);
        currentState = loginState; // Management Board stays logged in
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
  printf("INITIALIZING AUTH.C...\n");
  FILE *file = fopen("data/accounts.dat", "rb+");
  if (file == NULL) {
    printf("ERROR! ACCOUNTS.DAT NOT FOUND.\n");
    exit(1);
  }

  printf("ACCOUNTS.DAT FOUND. PROCESSING...\n");
  while (fread(&accountList[*quantity], sizeof(account), 1, file) == 1) {
    printf("SUCCESSFULLY READ 1 ACCOUNT FROM FILE\n");
    (*quantity)++;
    if (*quantity >= MAX_ACCOUNT)
      break;
  }
  fclose(file);
  printf("FOUND TOTAL: %d ACCOUNTS\n", *quantity);
  printf("-----------------------------\n");
}
void login(account *accountList, int *quantity, int *isLogin,
           Menu *currentState, account *session) {
  char studentIdInput[100], studentPasswordInput[MAX_PASS_LEN];
  while (1) {
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║             LOGIN MENU                 ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");

    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ STUDENT ID : " ANSI_COLOR_RESET);

    inputString(studentIdInput, 100);

    if (strlen(studentIdInput) != 8) {
      printf("\n" ANSI_COLOR_RED
             "STUDENT ID MUST BE EXACTLY 8 CHARACTERS! (Current length: %lu)\n",
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
      printf("\n" ANSI_COLOR_RED "STUDENT ID NOT FOUND IN LIST!\n");
      continue;
    }

    if (accountList[foundIndex].isLocked == 1) { // check if account is locked
      printf(ANSI_BRIGHT_RED "ACCOUNT IS CURRENTLY LOCKED\n");
      *currentState = loginState;
      return;
    }
    // bug
    printf(ANSI_BOLD ANSI_COLOR_YELLOW
           " ❯ PASSWORD    : " ANSI_COLOR_RESET); // WILL ADD PASSWORD MASKING
                                                  // (*) THÀNH *
    inputString(studentPasswordInput, MAX_PASS_LEN);

    if (strcmp(accountList[foundIndex].password, studentPasswordInput) ==
        0) { // password match check
      printf("\n");
      printf(ANSI_BRIGHT_GREEN "LOGIN SUCCESSFUL!\n");
      *isLogin = 1;
      accountList[foundIndex].failCount = 0;
      *session = accountList[foundIndex]; // get session
      ///// check if session initialized correctly!
      printf("\n");
      printf("CHECK SESSION upon successful login\n");
      printf("%s | %s | %d | %d\n", (*(session)).studentId,
             (*(session)).password, (*(session)).role, (*(session)).isLocked);
      printf("\n");

      // persist reset failCount (file sync)
      saveAccount(foundIndex, &accountList[foundIndex]);

    } else {
      printf("\n");
      printf(ANSI_BRIGHT_RED "INCORRECT PASSWORD!\n");
      (accountList[foundIndex].failCount)++;

      if (accountList[foundIndex].failCount >= 3) {
        accountList[foundIndex].isLocked = 1;
        printf("\n");
        printf(ANSI_BRIGHT_RED "ACCOUNT LOCKED DUE TO 3 FAILED ATTEMPTS!\n");
        printf(ANSI_BRIGHT_BLUE "RETURNING TO LOGIN SCREEN...\n");
        printf("\n");
        *currentState = loginState;
      }

      // Persist changes to file
      saveAccount(foundIndex, &accountList[foundIndex]);

      if (accountList[foundIndex].failCount >= 3) {
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
    printf("║             MEMBER MENU                ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET
                                      "MEMBER FUNCTIONS\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET
                                      "CHANGE PASSWORD\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " [3] " ANSI_COLOR_RESET "LOGOUT\n");
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD
           "------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ SELECT FUNCTION: " ANSI_COLOR_RESET);
    if (scanf("%d", &choice) != 1) {
      choice = 0; // Assign invalid value if input is not a number
    }
    while (getchar() != '\n')
      ; // Clear buffer to prevent hanging
    if (choice == 1) {
      /// not implemented
    } else if (choice == 2) {
      *currentState = changePasswordState;
    } else if (choice == 3) {
      *currentState = logoutState;
    }
  } else {
    int choice;
    printf(ANSI_COLOR_CYAN ANSI_BOLD);
    printf("╔════════════════════════════════════════╗\n");
    printf("║         MANAGEMENT BOARD MENU          ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET "\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [1] " ANSI_COLOR_RESET
                                      "MANAGEMENT FUNCTIONS\n");
    printf(ANSI_BOLD ANSI_COLOR_GREEN " [2] " ANSI_COLOR_RESET
                                      "CHANGE PASSWORD FOR STUDENT BY ID\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " [3] " ANSI_COLOR_RESET "LOGOUT\n");
    printf("\n");
    printf(ANSI_COLOR_CYAN ANSI_BOLD
           "------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ SELECT FUNCTION: " ANSI_COLOR_RESET);
    if (scanf("%d", &choice) != 1) {
      choice = 0;
    }
    while (getchar() != '\n')
      ;
    if (choice == 1) {
      // not implemented
    } else if (choice == 2) {
      *currentState = changePasswordState;
    } else if (choice == 3) {
      *currentState = logoutState;
    }
  }
}
int changePassword(account *accountList, int role, int *quantity,
                    account *session) {
  int found = 0;
  int foundIndex = -1; // <-- initialize to -1
  if (role == 0) {
    char passwordChange[MAX_PASS_LEN], oldPassword[MAX_PASS_LEN];
    printf("\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW " ❯ OLD PASSWORD : " ANSI_COLOR_RESET);
    inputString(oldPassword, MAX_PASS_LEN);
    if (strcmp(oldPassword, session->password) == 0) {
      printf("\n");
      printf(ANSI_BRIGHT_GREEN "OLD PASSWORD VERIFIED SUCCESSFULLY!\n");
      printf("\n");
      printf(ANSI_BOLD ANSI_COLOR_YELLOW
             " ❯ ENTER NEW PASSWORD : " ANSI_COLOR_RESET);
      inputString(passwordChange, MAX_PASS_LEN);
      printf("\n");
      printf(ANSI_BRIGHT_CYAN "UPDATING PASSWORD...\n");

      // Loop to find session account in accountList, then update
      // password and sync with file
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
        saveAccount(foundIndex, &accountList[foundIndex]);
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "PASSWORD CHANGED SUCCESSFULLY!\n");
        printf("\n");
        return 1;
      } else {
        // not found
        printf(ANSI_BRIGHT_RED "ACCOUNT NOT FOUND IN SYSTEM\n");
        return 0;
      }
    }

    else {
      printf("\n");
      printf(ANSI_BRIGHT_RED "INCORRECT PASSWORD ENTERED\n");
      printf("\n");
      return 0;
    }

  }

  else {
    printf("\n");
    printf(ANSI_BOLD ANSI_COLOR_YELLOW
           " ❯ ENTER STUDENT ID TO CHANGE PASSWORD: " ANSI_COLOR_RESET);
    char studentIDneedtochangePassword[MAX_ID_LEN];
    inputString(studentIDneedtochangePassword, MAX_ID_LEN);
    for (int i = 0; i < *quantity; i++) {
      if (strcmp(accountList[i].studentId, studentIDneedtochangePassword) ==
          0) {
        found = 1;
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "STUDENT ID VERIFIED!: %s\n",
               studentIDneedtochangePassword);
        printf("\n");
        printf(ANSI_BOLD ANSI_COLOR_YELLOW
               " ❯ ENTER NEW PASSWORD FOR STUDENT %s: ",
               studentIDneedtochangePassword);
        char passwordChange[MAX_PASS_LEN];
        inputString(passwordChange, MAX_PASS_LEN);
        printf("\n");
        printf(ANSI_BRIGHT_CYAN "UPDATING PASSWORD...\n");
        strcpy(accountList[i].password, passwordChange);
        saveAccount(i, &accountList[i]);
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "PASSWORD CHANGED SUCCESSFULLY FOR %s\n",
               studentIDneedtochangePassword);
        printf("\n");
        return 1;
      }
    }
    if (found == 0) {
      printf("\n");
      printf(ANSI_BRIGHT_RED "STUDENT ID NOT FOUND IN SYSTEM\n");
      printf("\n");
      return 0;
    }
  }
  return 0;
}

void saveAccount(int index, account *acc) {
  FILE *file = fopen("data/accounts.dat", "rb+");
  if (file == NULL) {
    printf(ANSI_BRIGHT_RED
           "ERROR: Unable to open file for update!\n" ANSI_COLOR_RESET);
    return;
  }
  fseek(file, (long)index * sizeof(account), SEEK_SET);
  if (fwrite(acc, sizeof(account), 1, file) != 1) {
    printf(ANSI_BRIGHT_RED "ERROR: Data write failed!\n" ANSI_COLOR_RESET);
  }
  fclose(file);
}

void inputString(char *buffer, int size) {
  if (fgets(buffer, size, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    } else {
      // Buffer was too small, clear the rest of the line
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
  }
}
