#include "validate.h"

#include <stdio.h>
#include "types.h"
#include <string.h>
#include <stdbool.h>
#include "auth.h"
static int is_alpha(char x){
    //check variable x is character ?
    if((int)x < 65)  return 0;
    if((int)x > 122) return 0;
    if(91 <= (int)x && (int)x <= 96)    return 0;
    return 1;
}
int is_number(char x){
    //check variable x is number ?
    if((int)x < 48) return 0;
    if(57 < (int)x) return 0;
    return 1;
}
static bool is_uppercase(char x){
    if((int)x < 65) return false;
    if((int)x > 90) return false;
    return true;
}
static bool is_lowercase(char x){
    if((int)x < 97) return false;
    if((int)x > 122)    return false;
    return true;
}
static char character_lowercase(char x){
    if(is_lowercase(x)) return x;
    return (char)((int)(x - 'A') + 'a');
}
static char character_uppercase(char x){
    if(is_uppercase(x)) return x;
    return (char)((int)(x - 'a') + 'A');
}
static bool is_special_character(char x){
    char special_list[] = "!@#$%^&*()_-+=|:;'<>,.?~`";
    for(int i = 0;special_list[i] != '\0';++i)  if(special_list[i] == x){
        return true;
    }
    return false;
}
static void normalize_fullname(int *n,char fullname[]){
    char normalized_name[LONG_SIZE];
    normalized_name[0] = character_uppercase(fullname[0]);
    int id = 1;
    for(int i = 1;i < (*n);){
        if(is_alpha(fullname[i])){
            if(fullname[i - 1] == ' ')  normalized_name[id] = character_uppercase(fullname[i]);
            else    normalized_name[id] = character_lowercase(fullname[i]);
        }
        else    normalized_name[id] = fullname[i];
        ++id;
        if(fullname[i] == ' '){
            int j = i;
            while(j < (*n) && fullname[j] == ' ')  ++j;
            i = j;
        }
        else    ++i;
    }
    normalized_name[id] = '\0';
    strcpy(fullname,normalized_name);
    (*n) = strlen(fullname);
}
bool Validate_fullname(char fullname[]){
    int n = strlen(fullname);
    if(n < 5) return false;
    if(fullname[0] == ' ' || fullname[n - 1] == ' ') return false;
    
    for(int i = 1;i < n;++i)    if(fullname[i] != ' ' && is_alpha(fullname[i]) == false){
        return false;
    }
    normalize_fullname(&n,fullname);
    return true;
}
static int Validate_email_local_part(char local_part[]){
    /* Condition for local_part to satisfy:
        + size > 0
        + letters or numbers, with 2 characters _ and .
        + beginning and end must be letters or numbers
        + no 2 consecutive dots
    */
    int n = strlen(local_part);
    if(n == 0)   return 0;

    if(is_alpha(local_part[0]) == 0 && is_number(local_part[0]) == 0)   return 0; //ở cuối nên là chữ hoặc số
    if(is_alpha(local_part[n - 1]) == 0 && is_number(local_part[n - 1]) == 0)    return 0;

    for(int i = 0;i < n;++i){
        if(is_alpha(local_part[i]) == 0 && is_number(local_part[i]) == 0 && local_part[i] != '_' && local_part[i] != '.')   return 0;
        if(i > 0 && ((local_part[i - 1] == '.' || local_part[i - 1] == '_') && (local_part[i] == '.' || local_part[i] == '_'))){
            return 0;
        }
    }
    return 1;
}
static int Validate_email_domain(char domain[]){
    /* Condition for domain to satisfy:
        + n > 0
        + letters or numbers, with 2 characters .
        + beginning and end must be letters
        + at least 1 dot
        + no 2 consecutive dots
        + last_domain part contains only letters
    */
    int n = strlen(domain);
    if(n == 0 || is_alpha(domain[0]) == 0 || is_alpha(domain[n - 1]) == 0) return 0;

    int dot_cnt = 0;
    for(int i = 0;i < n;++i){
        if((is_alpha(domain[i]) || is_number(domain[i]) || domain[i] == '.' || domain[i] == '-') == 0)  return 0;
        if(i > 0 && ((domain[i - 1] == '.' || domain[i - 1] == '-') && (domain[i] == '.' || domain[i] == '-'))){
            return 0;
        }
        dot_cnt += (domain[i] == '.');
    }
    if(dot_cnt == 0)    return 0;

    char *last_dot = strrchr(domain,'.');
    int last_dot_len = strlen(last_dot + 1);
    if(last_dot_len < 2)    return 0;
    for(int i = 1; i <= last_dot_len; ++i)  if(is_alpha(last_dot[i]) == 0) return 0;
    return 1;
}
bool Validate_email(char email[]){
    int n = strlen(email);
    if(n == 0) return 0;

    char local_part[LONG_SIZE], domain[LONG_SIZE];
    int cnt_At = 0;
    int u = 0, v = 0;

    for(int i = 0;i <= n;++i){
        if(i == n){
            if(cnt_At != 1)  return 0;
            local_part[u] = '\0';
            domain[v] = '\0';
            break;
        }

        if(email[i] == '@'){
            if(cnt_At == 1) return 0;
            cnt_At = 1;
            continue;
        }

        if(cnt_At == 0) local_part[u++] = email[i];
        else   domain[v++] = email[i];
    }
    if(Validate_email_local_part(local_part) == 0)  return 0;
    if(Validate_email_domain(domain) == 0)  return 0;
    return 1;
}
bool Validate_phone(char phone[]){
    int n = strlen(phone);
    if(n != 10) return false;
    if(phone[0] != '0') return false;
    for(int i = 1;i < n;++i)    if(is_number(phone[i]) == false)    return false;

    bool is_same = true;
    for(int i = 1;i < n;++i)    if(phone[1] != phone[i]){
        is_same = false;
        break;
    }
    return (is_same == false);
}
bool Validate_studentId(char studentId[]){
    /* Condition for studentId to satisfy:
        + n == 8
        + first 2 characters must be letters
        + first letter must be the first letter of a campus
        + the sequence after the first 2 characters must be digits
    */
    int n = strlen(studentId);
    if(n != 8 || !is_alpha(studentId[0]) || !is_alpha(studentId[1]))  return 0;
    studentId[0] = character_uppercase(studentId[0]);
    studentId[1] = character_uppercase(studentId[1]);
    
    char campus[5] = {'H','S','D','C','Q'};
    int is_campus = 0;
    for(int i = 0;i < 5;++i)    is_campus = (is_campus || (studentId[0] == campus[i]));

    if(is_campus == 0 || is_alpha(studentId[1]) == 0)  return 0;

    for(int i = 2;i < n;++i)    if(is_number(studentId[i]) == 0)    return 0;
    return 1;
}
bool Validate_team(int input){
    return !(input < 0 || input > 3);
}
bool Validate_role(int input){
    return !(input < 0 || input > 2);
}
bool Validate_violation_reason(int input){
    return !(input < 0 || input > 3);
}
bool Validate_password(char pass[]){
    /*Some condition to make a valid password:
    - Its length is from 8 to 40 characters
    - It contains at least 1 uppercase letter
    - It contains at least 1 lowercase letter
    - It contains at least 1 digit
    - It contains at least 1 special character
    - It does not contain spaces
    - It is not made up of only one repeated character
    */
    int n = strlen(pass);
    if(n < MIN_PASS_LEN || MAX_PASS_LEN < n) return false;

    bool ok[5] = {0,0,0,0,0};
    for(int i = 0;i < n;++i){
        if(pass[i] == ' ')  return false;
        ok[0] |= is_uppercase(pass[i]);
        ok[1] |= is_lowercase(pass[i]);
        ok[2] |= is_number(pass[i]);
        ok[3] |= is_special_character(pass[i]);
        ok[4] |= (pass[0] != pass[i]);
    }
    for(int i = 0;i < 5;++i)    if(ok[i] == false)  return false;
    return true;
}