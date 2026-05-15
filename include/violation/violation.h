#ifndef VIOLATION_H
#define VIOLATION_H


void viewOwnViolations(Violation violations[], int totalViolations, const char* loggedInStudentID, int userViolationCount);
void check_to_Paid(Violation violation[], int total_Violations, const char* logged_In_StudentID);
void sort_CLB_Violations(Member member[], int size, int isAscending);
#endif