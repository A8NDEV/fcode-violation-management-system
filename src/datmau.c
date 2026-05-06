#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cần sửa khớp với struct
#define MAX_ID_LEN 9
#define MAX_PASS_LEN 50

struct account {
    char studentId[MAX_ID_LEN]; 
    char password[MAX_PASS_LEN];
    int role;      
    int isLocked;  
    int failCount; 
};

typedef struct account Account;

int main() {
    
    Account ds_tai_khoan[] = {
        // { studentId, password, role, isLocked, failCount }
        {"ADMIN001", "admin123", 1, 0, 0},   // [0] Test: BCN đăng nhập
        {"SE123456", "SE123456", 0, 0, 0},   // [1] Test: Member bình thường
        {"SE654321", "SE654321", 0, 1, 3},   // [2] Test: Member đã bị khóa (failCount = 3, isLocked = 1)
        {"SE999999", "123456",   0, 0, 1}    // [3] Test: Member fail 1 lần (isLocked = 0)
    };

    int so_luong = sizeof(ds_tai_khoan) / sizeof(Account);

    FILE *file = fopen("accounts.dat", "wb");
    if (file == NULL) {
        printf("Lỗi: Không thể tạo tệp accounts.dat. Vui lòng kiểm tra quyền ghi.\n");
        return 1;
    }

    size_t so_luong_ghi = fwrite(ds_tai_khoan, sizeof(Account), so_luong, file);
    
    if (so_luong_ghi == so_luong) {
        printf("Thành công: Đã tạo tệp accounts.dat với %zu tài khoản mẫu.\n", so_luong_ghi);
        printf("Kích thước mỗi Account: %zu bytes.\n", sizeof(Account));
        printf("Tổng dung lượng tệp: %zu bytes.\n", sizeof(Account) * so_luong);
    } else {
        printf("Lỗi: Quá trình ghi tệp bị gián đoạn.\n");
    }

    fclose(file);
    return 0;
}