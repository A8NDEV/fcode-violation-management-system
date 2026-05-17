# QUẢN LÝ THÀNH VIÊN VI PHẠM CLB F-CODE
*F-Code Violation Management System*

> **Dự án cuối kỳ môn lập trình C (Final Project)**  
> **Đơn vị tổ chức**: Ban Học Thuật CLB F-Code, Trường ĐH FPT TP.HCM  
> **Nhóm thực hiện**: Light Stars (Lâm Hoàng An - SE201018, Nguyễn Văn Duy Nhân - SE211932, Nguyễn Thành Triết - SE211615, Nguyễn Văn Gia Bình - SE203555, Trần Khánh Tường - SE211449)

---

## 🌟 Giới thiệu tổng quan (Project Description)

Hệ thống quản lý kỷ luật và theo dõi vi phạm của câu lạc bộ F-Code được thiết kế để giải quyết bài toán theo dõi hoạt động, quản lý điểm danh, và minh bạch hóa các khoản thu phạt tiền/kỷ luật của các thành viên.

Chương trình hoạt động 100% trên giao diện dòng lệnh (Terminal/CLI) với hiệu ứng giao diện trực quan, bảng màu rõ nét và lưu trữ cơ sở dữ liệu vĩnh viễn trên tệp nhị phân (`.dat`).

### Các tính năng nổi bật:
- **Xác thực và phân quyền (Auth & Roles)**: Bảo mật bằng mật khẩu, tự động che sao `*****`, kiểm tra quy tắc mật khẩu mạnh và phân chia giao diện riêng biệt cho **Ban Chủ Nhiệm (BCN)** và **Member**.
- **Quản lý dữ liệu lớn**: Khả năng xử lý mượt mà lên tới 1.000 thành viên và hàng nghìn bản ghi vi phạm.
- **Điểm danh tự động (Batch Attendance)**: Hỗ trợ tự động đọc danh sách điểm danh cuộc họp từ file `data/attendance.txt`.
- **Hệ thống cảnh báo thông minh**: Tự động cảnh báo khi thành viên vắng 2 buổi liên tiếp và đề xuất xử lý Out CLB khi vắng quá 3 buổi.
- **Thống kê & Báo cáo**: Xuất báo cáo công nợ tự động ra tệp văn bản (`report_*.txt`), thống kê quỹ theo từng ban (Academic, Planning, HR, Media).

---

## 🛠 Hướng dẫn Cài đặt & Biên dịch (How to Compile)

### 1. Yêu cầu hệ thống (Prerequisites)
- Trình biên dịch: GCC (MinGW trên Windows / Linux GCC).
- Môi trường chạy khuyến nghị: **Git Bash**, PowerShell hoặc VS Code Terminal (hỗ trợ hiển thị bảng mã ANSI và UTF-8 icon).

### 2. Biên dịch nhanh bằng Makefile
Trong thư mục gốc của dự án, mở terminal và gõ lệnh:
```bash
mingw32-make
```
*(Nếu sử dụng Linux/macOS, sử dụng lệnh `make`).*

### 3. Biên dịch thủ công bằng dòng lệnh GCC
Nếu không dùng Makefile, bạn có thể chạy trực tiếp câu lệnh GCC sau:
```bash
gcc -Iinclude main.c src/*.c -o FCodeViolationManagement.exe
```

---

## 🚀 Hướng dẫn Chạy chương trình (How to Run)

Sau khi biên dịch thành công, khởi động hệ thống bằng lệnh:
```bash
./FCodeViolationManagement.exe
```

### 🔑 Khởi tạo dữ liệu mẫu (Seed Data - 2026 Edition)
Nếu bạn muốn đặt lại toàn bộ cơ sở dữ liệu mẫu ban đầu để kiểm thử, hãy chạy tệp khởi tạo:
```bash
gcc -Iinclude seed.c src/types.c src/fileio.c -o seed.exe && ./seed.exe
```
Hệ thống sẽ tự động tạo 20 tài khoản với các ca vi phạm và nợ tiền phạt khác nhau để trải nghiệm.

---

## 📋 Tài khoản Kiểm thử (Sample Test Accounts)

| Quyền hạn | Student ID (Tài khoản) | Mật khẩu (Password) | Tình trạng mẫu để Test |
|---|---|---|---|
| **Ban Chủ Nhiệm** (Admin) | `SE200001` (hoặc `SE200002`) | `Fcode@2026` | Toàn quyền quản trị |
| **Leader / Vice** | `SE200003` -> `SE200006` | Trùng với ID | Quản lý cấp Ban |
| **Member** (Thành viên) | `SE200010` | `SE200010` | Bị cảnh báo 3 buổi vắng họp liên tiếp |
| **Member** (Thành viên) | `SE200011` | `SE200011` | Nợ tiền phạt 80.000 VND |
| **Member** (Thành viên) | `SE200013` | `SE200013` | Vi phạm lỗi bạo lực, nợ 200.000 VND |
| **Member** (Thành viên) | `SE200009` | `SE200009` | Đã nộp hết tiền phạt, số dư = 0 |

---

## 📂 Kiến trúc Thư mục (Directory Structure)

```
fcode-violation-management-system/
├── include/           # Chứa toàn bộ các tệp khai báo tiêu đề (.h)
├── src/               # Chứa mã nguồn thực thi logic hệ thống (.c)
├── data/              # Thư mục chứa các tệp nhị phân dữ liệu (.dat)
├── README.md          # Tài liệu hướng dẫn sử dụng
├── Makefile           # Tệp cấu hình tự động biên dịch
├── seed.c             # Trình tạo dữ liệu mẫu
└── main.c             # Điểm khởi chạy của ứng dụng (Entry point)
```

