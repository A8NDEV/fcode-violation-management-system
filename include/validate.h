#ifndef VALIDATE_H
#define VALIDATE_H

#include "types.h"
#include <stdbool.h>

bool Validate_fullname(char fullname[]);
bool Validate_email(char email[]);
bool Validate_phone(char phone[]);
bool Validate_studentId(char studentId[]);
bool Validate_team(int input);
bool Validate_role(int input);
bool Validate_violation_reason(int input);
bool Validate_password(char pass[]);
#endif