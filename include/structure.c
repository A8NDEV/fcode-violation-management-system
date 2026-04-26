#include "structure.h"
#include <string.h>
#include <time.h>

Member Init_Member(){
    Member res;
    strcpy(res.fullName,"");
    strcpy(res.email,"");
    strcpy(res.phone,"");
    strcpy(res.studentId,"");
    res.team = res.role = res.violationCount = res.consecutiveAbsences = 0;
    res.totalFine = 0.0;
    return res;
}
Account Init_Account(){
    Account res;
    strcpy(res.studentId,"");
    strcpy(res.password,"");
    res.role = res.isLooked = res.failCount = 0;
    return res;
}
Violation Init_Violation(){
    Violation res;
    strcpy(res.studentId,"");
    strcpy(res.note,"");
    res.reason = res.isPaid = res.penalty = 0;
    res.violationTime = 0;
    res.fine = 20.000;
    return res;
}