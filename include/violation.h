#ifndef VIOLATION_H
#define VIOLATION_H

#include "types.h"

/* -------------------------------------------------------
 * Helper / Member-facing functions
 * ------------------------------------------------------- */

/* Hien thi lich su vi pham cua chinh member dang nhap */
void view_Own_Violations(Violation violations[], int total_Violations,
                         const char *logged_In_StudentID);

/* Hien thi tong tien phat con no (isPaid == 0) cua member dang nhap */
void check_to_Paid(Violation violation[], int total_Violations,
                   const char *logged_In_StudentID);

/* -------------------------------------------------------
 * BCN-facing Menu functions
 * ------------------------------------------------------- */

/* Ghi nhan vi pham moi cho mot thanh vien */
void Menu_record_violation(int *memberCount, Member memberList[],
                           int *violationCount, Violation violationList[]);

/* Danh dau da thu tien phat (set isPaid = 1, cap nhat totalFine) */
void Menu_mark_paid(int *memberCount, Member memberList[],
                    int *violationCount, Violation violationList[]);

/* Xem danh sach vi pham toan CLB, co bo loc */
void Menu_view_all_violations(int memberCount, Member memberList[],
                              int violationCount, Violation violationList[]);

#endif