#include "validate.h"

#include <stdio.h>
#include "types.h"
#include <string.h>
#include <stdbool.h>

static int is_alpha(char x){
    //check variable x is character ?
    if((int)x < 65)  return 0;
    if((int)x > 122) return 0;
    if(91 <= (int)x && (int)x <= 96)    return 0;
    return 1;
}
static int is_number(char x){
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
bool Validate_fullname(char fullname[]){
    int n = strlen(fullname);
    if(n == 0) return false;
    if(fullname[0] == ' ' || fullname[n - 1] == ' ') return false;
    if(!is_uppercase(fullname[0])) return false;
    
    for(int i = 1;i < n;++i){
        if(fullname[i - 1] == ' ' && (fullname[i] == ' ' || (is_uppercase(fullname[i])) == false))  return false;
        if(fullname[i - 1] != ' ' && fullname[i] != ' ' &&  is_lowercase(fullname[i]) == false)    return false;
    }
    return true;
}
bool Validate_studentId(char studentId[]){
    /* DK để studentId thỏa mãn là:
        + n == 8
        + 2 kí tự đầu phải là chữ
        + kí tự chữ đầu phải là chữ cái đầu của các campus
        + dãy sau 2 kí tự đầu phải là số
    */
    int n = strlen(studentId);
    if(n != 8)  return 0;

    char campus[5] = {'H','S','D','C','Q'};
    int is_campus = 0;
    for(int i = 0;i < 5;++i)    is_campus = (is_campus || (studentId[0] == campus[i]));

    if(is_campus == 0 || is_alpha(studentId[1]) == 0)  return 0;
    for(int i = 2;i < n;++i)    if(is_number(studentId[i]) == 0)    return 0;
    return 1;
}
static int Validate_email_local_part(char local_part[]){
    /* DK để local_part thỏa mãn là:
        + size > 0
        + là chữ hoặc số, kèm theo 2 dấu _ và .
        + đầu và cuối phải là chữ hoặc số
        + ko có 2 dấu . liên tiếp
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
    /* DK để doamin thỏa mãn là:
        + n > 0
        + là chữ hoặc số, kèm theo 2 dấu .
        + đầu và cuối phải là chữ
        + có ít nhất 1 dấu .
        + không có 2 dấu . liên tiếp
        + phần last_domain chỉ có chữ
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