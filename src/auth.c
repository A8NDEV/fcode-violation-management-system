#include "auth.h"
#include "validate.h"
#include "utils.h"
#include "windows.h"
#ifdef _WIN32
#include <conio.h>
#endif

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
          currentState = loginState;
        }
      } else {
        changePassword(accountList, session->role, quantity, session);
        currentState = loginState;
      }
      break;
    case exitState:
      break;
    }
  }
}



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
             "STUDENT ID MUST BE EXACTLY 8 CHARACTERS! (Current length: %d)\n",
             (int)strlen(studentIdInput));
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

    if (accountList[foundIndex].isLocked == 1) {
      printf(ANSI_BRIGHT_RED "ACCOUNT IS CURRENTLY LOCKED\n");
      if (currentState != NULL) *currentState = loginState;
      return;
    }

    while (1) {
      printf(ANSI_BOLD ANSI_COLOR_YELLOW
             " ❯ PASSWORD    : " ANSI_COLOR_RESET);
      inputPassword(studentPasswordInput, MAX_PASS_LEN);

      if (strcmp(accountList[foundIndex].password, studentPasswordInput) == 0) {
        printf("\n");
        Rainbow_Loading("Verifying Account");
        system("cls");
        printf(ANSI_BRIGHT_GREEN "LOGIN SUCCESSFUL!\n");
        *isLogin = 1;
        accountList[foundIndex].failCount = 0;
        *session = accountList[foundIndex];

        saveAccount(foundIndex, &accountList[foundIndex]);
        break;
      } else {
        printf("\n");
        printf(ANSI_BRIGHT_RED "INCORRECT PASSWORD!\n");
        (accountList[foundIndex].failCount)++;

        if (accountList[foundIndex].failCount >= 3) {
          accountList[foundIndex].failCount = 0;
          saveAccount(foundIndex, &accountList[foundIndex]);
          printf("\n" ANSI_BRIGHT_RED "TOO MANY FAILED ATTEMPTS! SESSION TERMINATED.\n" ANSI_COLOR_RESET);
          Sleep(3000);
          exit(0);
        }

        saveAccount(foundIndex, &accountList[foundIndex]);

        char retryChoice[32];
        int wantRetry = 0;
        while (1) {
          printf("\n" ANSI_BOLD ANSI_COLOR_YELLOW " ❯ Do you want to try entering password again? (y/n): " ANSI_COLOR_RESET);
          inputString(retryChoice, sizeof(retryChoice));
          if (strlen(retryChoice) == 0) continue;
          if (retryChoice[0] == 'y' || retryChoice[0] == 'Y') {
            wantRetry = 1;
            break;
          } else if (retryChoice[0] == 'n' || retryChoice[0] == 'N') {
            wantRetry = 0;
            break;
          } else {
            printf(ANSI_COLOR_RED "Invalid choice! Please enter 'y' or 'n'." ANSI_COLOR_RESET "\n");
          }
        }

        if (wantRetry) {
          printf("\n");
          continue;
        } else {
          return;
        }
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
    char buf[32];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
      if (sscanf(buf, "%d", &choice) != 1) choice = 0;
    } else {
      choice = 0;
    }
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
    char buf[32];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
      if (sscanf(buf, "%d", &choice) != 1) choice = 0;
    } else {
      choice = 0;
    }
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
  int foundIndex = -1;
  if (role == 0) {
    char passwordChange[MAX_PASS_LEN], oldPassword[MAX_PASS_LEN];
    UI_Header("CHANGE PASSWORD", ANSI_COLOR_YELLOW);
    UI_Prompt("Old Password", "🗝");
    inputPassword(oldPassword, MAX_PASS_LEN);
    if (strcmp(oldPassword, session->password) == 0) {
      printf("\n" ANSI_BRIGHT_GREEN "  ✔ OLD PASSWORD VERIFIED SUCCESSFULLY!\n" ANSI_COLOR_RESET);
      while(1){
        char passwordComfirm[MAX_PASS_LEN];
        UI_Prompt("New Password", "🗝");
        inputPassword(passwordChange, MAX_PASS_LEN);
        
        if(Validate_password(passwordChange) == 0){
          printf(ANSI_BOLD ANSI_COLOR_RED
                "  ✖ YOUR PASSWORD NEEDS AT LEAST ONE UPPERCASE, ONE LOWERCASE, AND ONE DIGIT!\n" ANSI_COLOR_RESET);
          continue;
        }

        UI_Prompt("Confirm Password", "🗝");
        inputPassword(passwordComfirm,MAX_PASS_LEN);
        if(strcmp(passwordChange,passwordComfirm) == 0){
          break;
        }
        else{
          printf(ANSI_BOLD ANSI_COLOR_RED
                " ❯ NOT MATCH, TRY AGAIN!\n" ANSI_COLOR_RESET);
        }
      }
      printf("\n");
      for (int i = 0; i < *quantity; i++) {
        if (strcmp(accountList[i].studentId, session->studentId) == 0) {
          foundIndex = i;
          break;
        }
      }
      printf(ANSI_BRIGHT_CYAN "UPDATING PASSWORD...\n");
      if (foundIndex != -1) {
        strcpy(session->password, passwordChange);
        strcpy(accountList[foundIndex].password, passwordChange);
        saveAccount(foundIndex, &accountList[foundIndex]);
        printf("\n");
        printf(ANSI_BRIGHT_GREEN "PASSWORD CHANGED SUCCESSFULLY!\n");
        printf("\n");
        return 1;
      } else {
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
    UI_Header("ADMIN PASSWORD OVERRIDE", ANSI_COLOR_RED);
    UI_Prompt("Target Student ID", "🔍");
    char studentIDneedtochangePassword[MAX_ID_LEN];
    inputString(studentIDneedtochangePassword, MAX_ID_LEN);
    for (int i = 0; i < *quantity; i++) {
      if (strcmp(accountList[i].studentId, studentIDneedtochangePassword) ==
          0) {
        found = 1;
        printf("\n" ANSI_BRIGHT_GREEN "  ✔ STUDENT ID VERIFIED: %s\n" ANSI_COLOR_RESET,
               studentIDneedtochangePassword);
        UI_Prompt("New Password", "🗝");
        char passwordChange[MAX_PASS_LEN];
        inputPassword(passwordChange, MAX_PASS_LEN);
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
      buffer[strcspn(buffer, "\r\n")] = '\0';
    } else {
      buffer[strcspn(buffer, "\r\n")] = '\0';
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
  }
}

void inputPassword(char *password, int maxSize) {
    int i = 0;
    char c;

    while (i < maxSize - 1) {
#ifdef _WIN32
        c = _getch();
#else
        c = getchar(); 
#endif

        if (c == '\n' || c == '\r') {
            password[i] = '\0';
            break;
        }
        else if (c == 127 || c == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        }
        else if (c >= 32 && c <= 126) {
            password[i++] = c;
            printf("*");
            fflush(stdout);
        }
    }
    password[i] = '\0';
    printf("\n");
}
