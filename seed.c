/*
 * seed.c - Seed du lieu mau cho FCode Violation Management System
 * Compile & run: gcc -Iinclude seed.c src/types.c src/fileio.c -o seed.exe && ./seed.exe
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "include/types.h"
#include "include/fileio.h"

/* -------------------------------------------------------
 * Helper macros
 * ------------------------------------------------------- */
#define DAYS_AGO(n) (time(NULL) - (time_t)(n) * 86400)

/* -------------------------------------------------------
 * Helper functions
 * ------------------------------------------------------- */
static Member make_member(const char *sid, const char *name,
                          const char *email, const char *phone,
                          int team, int role) {
    Member m;
    strcpy(m.studentId, sid);
    strcpy(m.fullName,  name);
    strcpy(m.email,     email);
    strcpy(m.phone,     phone);
    m.team                = team;
    m.role                = role;
    m.violationCount      = 0;
    m.consecutiveAbsences = 0;
    m.totalFine           = 0.0;
    return m;
}

static Account make_account(const char *sid, const char *pass, int role) {
    Account a;
    strcpy(a.studentId, sid);
    strcpy(a.password,  pass);
    a.role      = role;
    a.isLocked  = 0;
    a.failCount = 0;
    return a;
}

static Violation make_violation(const char *sid, int reason,
                                int isPaid, double fine,
                                time_t vtime) {
    Violation v;
    strcpy(v.studentId, sid);
    strcpy(v.note,      "");
    v.reason        = reason;
    v.isPaid        = isPaid;
    v.penalty       = 0;
    v.fine          = fine;
    v.violationTime = vtime;
    return v;
}

/* -------------------------------------------------------
 * Wipe a .dat file
 * ------------------------------------------------------- */
static void clear_file(const char *path) {
    FILE *f = fopen(path, "wb");
    if (f) fclose(f);
}

int main(void) {
    printf("=== FCode Seed Tool ===\n");

    /* --------------------------------------------------
     * 1. MEMBERS & ACCOUNTS
     * team: 0=Academic 1=Planning 2=HR 3=Media
     * role: 0=Member  1=Leader/Vice  2=BCN
     * -------------------------------------------------- */
    Member members[] = {
        /* BCN */
        make_member("HE000001", "Nguyen Van An",   "an.nv@fcode.vn",     "0912345678", 0, 2),
        make_member("HE000002", "Tran Thi Bich",   "bich.tt@fcode.vn",   "0987654321", 1, 2),
        /* Leader / Vice */
        make_member("HE000003", "Le Minh Cuong",   "cuong.lm@fcode.vn",  "0901234567", 0, 1),
        make_member("HE000004", "Pham Quynh Dao",  "dao.pq@fcode.vn",    "0934567890", 1, 1),
        make_member("HE000005", "Hoang Tuan Em",   "em.ht@fcode.vn",     "0945678901", 2, 1),
        make_member("HE000006", "Vu Ngoc Phuong",  "phuong.vn@fcode.vn", "0956789012", 3, 1),
        /* Academic */
        make_member("HE000007", "Do Thanh Giang",  "giang.dt@fcode.vn",  "0967890123", 0, 0),
        make_member("HE000008", "Nguyen Bao Hung", "hung.nb@fcode.vn",   "0978901234", 0, 0),
        make_member("HE000009", "Mai Thi Lan",     "lan.mt@fcode.vn",    "0989012345", 0, 0),
        /* Planning */
        make_member("HE000010", "Dinh Xuan Khanh", "khanh.dx@fcode.vn",  "0990123456", 1, 0),
        make_member("HE000011", "Bui Thanh Liem",  "liem.bt@fcode.vn",   "0901234568", 1, 0),
        make_member("HE000012", "Cao Ngoc Mai",    "mai.cn@fcode.vn",    "0912345679", 1, 0),
        /* HR */
        make_member("HE000013", "Trinh Thi Nam",   "nam.tt@fcode.vn",    "0923456780", 2, 0),
        make_member("HE000014", "Ly Hoang Oanh",   "oanh.lh@fcode.vn",   "0934567891", 2, 0),
        make_member("HE000015", "Ngo Van Phuc",    "phuc.nv@fcode.vn",   "0945678902", 2, 0),
        /* Media */
        make_member("HE000016", "Dang Thi Quynh",  "quynh.dt@fcode.vn",  "0956789013", 3, 0),
        make_member("HE000017", "Luong Van Rong",  "rong.lv@fcode.vn",   "0967890124", 3, 0),
        make_member("HE000018", "Phan Ngoc Son",   "son.pn@fcode.vn",    "0978901235", 3, 0),
        make_member("HE000019", "Tran Bich Thuy",  "thuy.tb@fcode.vn",   "0989012346", 3, 0),
        make_member("HE000020", "Vo Thanh Ung",    "ung.vt@fcode.vn",    "0990123457", 3, 0),
    };
    int memberCount = (int)(sizeof(members) / sizeof(members[0]));

    /* Accounts: BCN dung pass manh, con lai pass = MSSV */
    Account accounts[] = {
        make_account("HE000001", "Fcode@2024", 2),
        make_account("HE000002", "Fcode@2024", 2),
        make_account("HE000003", "HE000003",   1),
        make_account("HE000004", "HE000004",   1),
        make_account("HE000005", "HE000005",   1),
        make_account("HE000006", "HE000006",   1),
        make_account("HE000007", "HE000007",   0),
        make_account("HE000008", "HE000008",   0),
        make_account("HE000009", "HE000009",   0),
        make_account("HE000010", "HE000010",   0),
        make_account("HE000011", "HE000011",   0),
        make_account("HE000012", "HE000012",   0),
        make_account("HE000013", "HE000013",   0),
        make_account("HE000014", "HE000014",   0),
        make_account("HE000015", "HE000015",   0),
        make_account("HE000016", "HE000016",   0),
        make_account("HE000017", "HE000017",   0),
        make_account("HE000018", "HE000018",   0),
        make_account("HE000019", "HE000019",   0),
        make_account("HE000020", "HE000020",   0),
    };

    /* --------------------------------------------------
     * 2. VIOLATIONS
     * reason: 0=Ao CLB 1=Vang hop 2=Khong HD 3=Bao luc
     * fine  : 20000 / 50000 / 30000 / 200000
     * -------------------------------------------------- */
    Violation violations[] = {
        /* HE000007 - Do Thanh Giang: 2 vi pham */
        make_violation("HE000007", 0, 1, 20000.0,  DAYS_AGO(60)),
        make_violation("HE000007", 1, 0, 50000.0,  DAYS_AGO(30)),

        /* HE000008 - Nguyen Bao Hung: 1 vi pham */
        make_violation("HE000008", 2, 0, 30000.0,  DAYS_AGO(45)),

        /* HE000009 - Mai Thi Lan: 2 vi pham da thu het */
        make_violation("HE000009", 1, 1, 50000.0,  DAYS_AGO(90)),
        make_violation("HE000009", 0, 1, 20000.0,  DAYS_AGO(75)),

        /* HE000010 - Dinh Xuan Khanh: 1 vi pham */
        make_violation("HE000010", 1, 0, 50000.0,  DAYS_AGO(15)),

        /* HE000011 - Bui Thanh Liem: 3 vi pham (nhieu nhat) */
        make_violation("HE000011", 0, 1, 20000.0,  DAYS_AGO(100)),
        make_violation("HE000011", 1, 0, 50000.0,  DAYS_AGO(50)),
        make_violation("HE000011", 2, 0, 30000.0,  DAYS_AGO(10)),

        /* HE000013 - Trinh Thi Nam: vi pham bao luc */
        make_violation("HE000013", 3, 0, 200000.0, DAYS_AGO(20)),

        /* HE000016 - Dang Thi Quynh: 2 vi pham gan day */
        make_violation("HE000016", 0, 0, 20000.0,  DAYS_AGO(5)),
        make_violation("HE000016", 1, 0, 50000.0,  DAYS_AGO(3)),

        /* HE000017 - Luong Van Rong: 1 vi pham da thu */
        make_violation("HE000017", 2, 1, 30000.0,  DAYS_AGO(40)),

        /* HE000020 - Vo Thanh Ung: 1 vi pham */
        make_violation("HE000020", 1, 0, 50000.0,  DAYS_AGO(7)),
    };
    int violationCount = (int)(sizeof(violations) / sizeof(violations[0]));

    /* --------------------------------------------------
     * 3. Sync violationCount & totalFine vao member array
     * -------------------------------------------------- */
    for (int i = 0; i < violationCount; i++) {
        for (int j = 0; j < memberCount; j++) {
            if (strcmp(violations[i].studentId, members[j].studentId) == 0) {
                members[j].violationCount++;
                if (violations[i].isPaid == 0)
                    members[j].totalFine += violations[i].fine;
                break;
            }
        }
    }

    /* --------------------------------------------------
     * 4. Wipe old data and rewrite all files
     * -------------------------------------------------- */
    clear_file("data/members.dat");
    clear_file("data/accounts.dat");
    clear_file("data/violations.dat");

    int ok = 1;
    for (int i = 0; i < memberCount; i++) {
        if (!Append_members_dat(members[i]))   { ok = 0; break; }
        if (!Append_accounts_dat(accounts[i])) { ok = 0; break; }
    }
    for (int i = 0; i < violationCount && ok; i++) {
        if (!Append_violations_dat(violations[i])) { ok = 0; break; }
    }

    if (!ok) {
        printf("[FAIL] Loi khi ghi file dat!\n");
        return 1;
    }

    /* --------------------------------------------------
     * 5. Report
     * -------------------------------------------------- */
    printf("[OK] Da seed thanh cong!\n");
    printf("     Members  : %d\n", memberCount);
    printf("     Accounts : %d\n", memberCount);
    printf("     Violations: %d\n\n", violationCount);

    printf("=== Tai khoan de test ===\n");
    printf("  [BCN]    HE000001 / Fcode@2024\n");
    printf("  [BCN]    HE000002 / Fcode@2024\n");
    printf("  [Member] HE000011 / HE000011  (3 vi pham, no 80000 VND)\n");
    printf("  [Member] HE000013 / HE000013  (vi pham bao luc, no 200000 VND)\n");
    printf("  [Member] HE000016 / HE000016  (2 vi pham, no 70000 VND)\n");
    printf("  [Member] HE000009 / HE000009  (da thu het, so du = 0)\n");

    return 0;
}
