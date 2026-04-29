#ifndef VALIDATE_H
#define VALIDATE_H

#include "types.h"
#include <stdbool.h>

bool Validate_fullname(char fullname[]);
bool Validate_studentId(char studentId[]);
bool Validate_email(char email[]);

#endif