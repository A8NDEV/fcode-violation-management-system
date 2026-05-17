/*
 * seed.c - Seed du lieu mau cho FCode Violation Management System
 * Compile & run: gcc -Iinclude seed.c src/types.c src/fileio.c -o seed.exe && ./seed.exe
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "include/types.h"
#include "include/fileio.h"

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#define MAKE_DIR(p) mkdir(p, 0777)
#endif

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
    MAKE_DIR("data");
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
        make_member("SE200001", "Nguyen Van An",   "an.nv@fcode.vn",     "0912345678", 0, 2),
        make_member("SE200002", "Tran Thi Bich",   "bich.tt@fcode.vn",   "0987654321", 1, 2),
        /* Leader / Vice */
        make_member("SE200003", "Le Minh Cuong",   "cuong.lm@fcode.vn",  "0901234567", 0, 1),
        make_member("SE200004", "Pham Quynh Dao",  "dao.pq@fcode.vn",    "0934567890", 1, 1),
        make_member("SE200005", "Hoang Tuan Em",   "em.ht@fcode.vn",     "0945678901", 2, 1),
        make_member("SE200006", "Vu Ngoc Phuong",  "phuong.vn@fcode.vn", "0956789012", 3, 1),
        /* Academic */
        make_member("SE200007", "Do Thanh Giang",  "giang.dt@fcode.vn",  "0967890123", 0, 0),
        make_member("SE200008", "Nguyen Bao Hung", "hung.nb@fcode.vn",   "0978901234", 0, 0),
        make_member("SE200009", "Mai Thi Lan",     "lan.mt@fcode.vn",    "0989012345", 0, 0),
        /* Planning */
        make_member("SE200010", "Dinh Xuan Khanh", "khanh.dx@fcode.vn",  "0990123456", 1, 0),
        make_member("SE200011", "Bui Thanh Liem",  "liem.bt@fcode.vn",   "0901234568", 1, 0),
        make_member("SE200012", "Cao Ngoc Mai",    "mai.cn@fcode.vn",    "0912345679", 1, 0),
        /* HR */
        make_member("SE200013", "Trinh Thi Nam",   "nam.tt@fcode.vn",    "0923456780", 2, 0),
        make_member("SE200014", "Ly Hoang Oanh",   "oanh.lh@fcode.vn",   "0934567891", 2, 0),
        make_member("SE200015", "Ngo Van Phuc",    "phuc.nv@fcode.vn",   "0945678902", 2, 0),
        /* Media */
        make_member("SE200016", "Dang Thi Quynh",  "quynh.dt@fcode.vn",  "0956789013", 3, 0),
        make_member("SE200017", "Luong Van Rong",  "rong.lv@fcode.vn",   "0967890124", 3, 0),
        make_member("SE200018", "Phan Ngoc Son",   "son.pn@fcode.vn",    "0978901235", 3, 0),
        make_member("SE200019", "Tran Bich Thuy",  "thuy.tb@fcode.vn",   "0989012346", 3, 0),
        make_member("SE200020", "Vo Thanh Ung",    "ung.vt@fcode.vn",    "0990123457", 3, 0),
    };
    int memberCount = (int)(sizeof(members) / sizeof(members[0]));

    /* Accounts: BCN dung pass manh, con lai pass = MSSV */
    Account accounts[] = {
        make_account("SE200001", "Fcode@2026", 2),
        make_account("SE200002", "Fcode@2026", 2),
        make_account("SE200003", "SE200003",   1),
        make_account("SE200004", "SE200004",   1),
        make_account("SE200005", "SE200005",   1),
        make_account("SE200006", "SE200006",   1),
        make_account("SE200007", "SE200007",   0),
        make_account("SE200008", "SE200008",   0),
        make_account("SE200009", "SE200009",   0),
        make_account("SE200010", "SE200010",   0),
        make_account("SE200011", "SE200011",   0),
        make_account("SE200012", "SE200012",   0),
        make_account("SE200013", "SE200013",   0),
        make_account("SE200014", "SE200014",   0),
        make_account("SE200015", "SE200015",   0),
        make_account("SE200016", "SE200016",   0),
        make_account("SE200017", "SE200017",   0),
        make_account("SE200018", "SE200018",   0),
        make_account("SE200019", "SE200019",   0),
        make_account("SE200020", "SE200020",   0),
    };

    /* --------------------------------------------------
     * 2. VIOLATIONS
     * reason: 0=Ao CLB 1=Vang hop 2=Khong HD 3=Bao luc
     * fine  : 20000 / 50000 / 30000 / 200000
     * -------------------------------------------------- */
    Violation violations[] = {
        /* SE200007 - Do Thanh Giang: 2 vi pham */
        make_violation("SE200007", 0, 1, 20000.0,  DAYS_AGO(60)),
        make_violation("SE200007", 1, 0, 50000.0,  DAYS_AGO(30)),

        /* SE200008 - Nguyen Bao Hung: 1 vi pham */
        make_violation("SE200008", 2, 0, 30000.0,  DAYS_AGO(45)),

        /* SE200009 - Mai Thi Lan: 2 vi pham da thu het */
        make_violation("SE200009", 1, 1, 50000.0,  DAYS_AGO(90)),
        make_violation("SE200009", 0, 1, 20000.0,  DAYS_AGO(75)),

        /* SE200010 - Dinh Xuan Khanh: 3 vi pham vang hop (canh bao) */
        make_violation("SE200010", 1, 0, 50000.0,  DAYS_AGO(15)),
        make_violation("SE200010", 1, 0, 50000.0,  DAYS_AGO(10)),
        make_violation("SE200010", 1, 0, 50000.0,  DAYS_AGO(5)),

        /* SE200011 - Bui Thanh Liem: 3 vi pham (nhieu nhat) */
        make_violation("SE200011", 0, 1, 20000.0,  DAYS_AGO(100)),
        make_violation("SE200011", 1, 0, 50000.0,  DAYS_AGO(50)),
        make_violation("SE200011", 2, 0, 30000.0,  DAYS_AGO(10)),

        /* SE200013 - Trinh Thi Nam: vi pham bao luc */
        make_violation("SE200013", 3, 0, 200000.0, DAYS_AGO(20)),

        /* SE200016 - Dang Thi Quynh: 2 vi pham gan day */
        make_violation("SE200016", 0, 0, 20000.0,  DAYS_AGO(5)),
        make_violation("SE200016", 1, 0, 50000.0,  DAYS_AGO(3)),

        /* SE200017 - Luong Van Rong: 1 vi pham da thu */
        make_violation("SE200017", 2, 1, 30000.0,  DAYS_AGO(40)),

        /* SE200020 - Vo Thanh Ung: 1 vi pham */
        make_violation("SE200020", 1, 0, 50000.0,  DAYS_AGO(7)),
    };
    int violationCount = (int)(sizeof(violations) / sizeof(violations[0]));

    /* --------------------------------------------------
     * 3. Sync violationCount, totalFine & absences vao member array
     * -------------------------------------------------- */
    for (int i = 0; i < violationCount; i++) {
        for (int j = 0; j < memberCount; j++) {
            if (strcmp(violations[i].studentId, members[j].studentId) == 0) {
                members[j].violationCount++;
                if (violations[i].isPaid == 0)
                    members[j].totalFine += violations[i].fine;
                
                if (violations[i].reason == 1) {
                    members[j].consecutiveAbsences++;
                }
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

    printf("=== Tai khoan de test (2026 Edition) ===\n");
    printf("  [BCN]    SE200001 / Fcode@2026\n");
    printf("  [BCN]    SE200002 / Fcode@2026\n");
    printf("  [Member] SE200010 / SE200010  (3 vi pham vang hop lien tiep)\n");
    printf("  [Member] SE200011 / SE200011  (3 vi pham, no 80000 VND)\n");
    printf("  [Member] SE200013 / SE200013  (vi pham bao luc, no 200000 VND)\n");
    printf("  [Member] SE200016 / SE200016  (2 vi pham, no 70000 VND)\n");
    printf("  [Member] SE200009 / SE200009  (da thu het, so du = 0)\n");

    return 0;
}
