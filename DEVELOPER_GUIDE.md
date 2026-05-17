# 🛠 F-Code Project Developer Guide

Tài liệu này hướng dẫn cách phát triển và đóng góp code cho dự án Quản lý vi phạm. Vui lòng đọc kỹ để đảm bảo tính thống nhất.

## 1. Cấu trúc thư mục (Folder Structure)
*   `include/`: Chứa các file `.h`. Chỉ khai báo hàm và cấu trúc dữ liệu tại đây.
*   `src/`: Chứa các file `.c`. Viết logic xử lý chi tiết tại đây.
*   `data/`: Chứa file `.dat`. Tuyệt đối không đẩy file này lên Git (đã cấu hình trong `.gitignore`).
*   `main.c`: Chỉ dùng để load dữ liệu và điều hướng. **Không viết logic nghiệp vụ vào đây.**

## 2. Quy trình thêm chức năng mới
Khi bạn được giao làm một chức năng mới (Ví dụ: Module "Khen thưởng"):
1.  Tạo file header: `include/reward.h`
2.  Tạo file source: `src/reward.c`
3.  Kết nối giao diện: Thêm lựa chọn vào `src/menu.c`.
4.  Biên dịch: Sử dụng **Makefile** (Nhấn F11 trong Dev-C++ nếu đã cấu hình Custom Makefile).

## 3. Quy tắc đặt tên & Code Style
*   **Ngôn ngữ:** Tên hàm, tên biến dùng **Tiếng Anh**.
*   **Header Guard:** Luôn có `#ifndef FILE_H`, `#define FILE_H` ở đầu file `.h`.
*   **UI:** Sử dụng các define màu ANSI trong `auth.h` để in thông báo (Ví dụ: `ANSI_COLOR_RED` cho lỗi).
*   **Single Responsibility:** Mỗi hàm chỉ nên làm một việc duy nhất. Nếu hàm quá dài (>50 dòng), hãy cân nhắc tách hàm con.
