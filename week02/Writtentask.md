## LỜI GIẢI CÁC CÂU HỎI TỰ LUẬN (TASK 4 & TASK 5)

---

###  TASK 4: MEMORY MAP & CRASH LAB

#### 4.2 Sắp xếp các biến vào các phân vùng bộ nhớ tương ứng

Dựa trên bản đồ bộ nhớ (Memory Map) của hệ điều hành và vi điều khiển, các biến trong chương trình được phân bổ cụ thể như sau:

| Tên biến | Phân vùng bộ nhớ | Giải thích chi tiết bản chất kỹ thuật |
| :--- | :--- | :--- |
| **`BIKE_MODEL`** | **`.rodata`** *(Read-Only Data)* | Biến hằng cục bộ hoặc toàn cục có từ khóa `const` và đã được khởi tạo giá trị chuỗi cố định. Phân vùng này nằm ở **ROM/Flash** nhằm ngăn chặn mọi hành vi vô tình ghi đè trong quá trình Runtime. |
| **`total_odometer`** | **`.bss`** *(Block Started by Symbol)* | Biến toàn cục (global) chưa được khởi tạo giá trị ban đầu (hoặc khởi tạo bằng `0`). Phân vùng này nằm ở **RAM** và sẽ được đoạn mã khởi động (`Startup code`) tự động xóa sạch (clear) về `0` trước khi hàm `main()` được gọi. |
| **`current_speed`** | **`Stack`** *(Ngăn xếp)* | Biến cục bộ (local) thông thường khai báo bên trong hàm `main()`. Vùng nhớ này nằm ở **RAM**, tự động cấp phát khi luồng thực thi đi vào hàm và tự động giải phóng (thu hồi) ngay khi thoát khỏi phạm vi hàm. |

> **Thứ tự địa chỉ thực tế (Thông thường):** Địa chỉ vùng `Stack` (đỉnh RAM) > Địa chỉ vùng `.bss` (đáy RAM) > Địa chỉ vùng `.rodata` (ROM/Flash).

---

#### 4.3 Hiện tượng sụp đổ hệ thống khi xảy ra sự cố tràn ngăn xếp (Stack Overflow)

* **Trên Máy tính (Môi trường giả lập OS - Windows/Linux):**
    * **Hiện tượng:** Chương trình bị ngắt ngay lập tức và hệ điều hành sẽ đưa ra thông báo lỗi: `Segmentation fault (core dumped)`.
    * **Nguyên nhân:** Hệ điều hành quản lý bộ nhớ thông qua bộ MMU (Memory Management Unit) và Virtual Memory. Khi hàm đệ quy liên tục ép con trỏ `Stack Pointer (SP)` vượt ra ngoài biên của trang bộ nhớ được cấp phép cho Stack (vi phạm vùng trang bảo vệ - Guard Page), OS sẽ can thiệp để tắt tiến trình nhằm bảo vệ tính toàn vẹn của hệ thống.
* **Trên Vi điều khiển (Thiết bị Nhúng thực tế - Ví dụ: ARM Cortex-M):**
    * **Hiện tượng:** Vi điều khiển lập tức ngừng hoạt động bình thường, hệ thống bị treo hoặc tự động Reset liên tục. Nếu debug, ta sẽ thấy CPU nhảy thẳng vào trình phục vụ ngat ngoại lệ: `HardFault_Handler()`.
    * **Nguyên nhân:** Vi điều khiển bare-metal thường không có OS bảo vệ bộ nhớ. Khi Stack bị tràn, con trỏ `SP` sẽ lấn sang và ghi đè trực tiếp lên dữ liệu của các phân vùng lân cận như `.data` hoặc `.bss` (gây sai lệch biến toàn cục). Ngay khi con trỏ `SP` nhảy ra khỏi dải địa chỉ vật lý hợp lệ của RAM hoặc thực hiện sai lệch lệnh lấy địa chỉ trả về (`LR - Link Register`), phần cứng CPU sẽ kích hoạt một bẫy ngoại lệ nghiêm trọng (Hardware Exception) mang tên **HardFault**.

---

###  TASK 5: TỐI ƯU HÓA DUNG LƯỢNG VỚI LINKER SCRIPT

#### 1. Lựa chọn giải pháp
> **CHỌN CÁCH 1:** `uint16_t trip_history[20000] = {0};`

#### 2. Phân tích định lượng bộ nhớ của mảng
Mảng có kích thước: $20,000 \text{ phần tử} \times 2 \text{ bytes (uint16\_t)} = 40,000 \text{ bytes} \approx \mathbf{40\text{ KB}}$.
* **Flash còn trống:** $20\text{ KB}$
* **RAM còn trống:** $60\text{ KB}$

#### 3. Giải thích bản chất cơ chế hoạt động của Linker

* **Vì sao Cách 1 THÀNH CÔNG:**
    * Khi ta khởi tạo mảng bằng `{0};` (hoặc để trống không khởi tạo), Linker dựa theo quy tắc thiết kế sẽ xếp mảng này vào phân vùng **`.bss`** trong RAM.
    * **Đặc điểm vùng `.bss`:** Bản chất phân vùng `.bss` **không chiếm dụng không gian vật lý trong file thực thi (`.bin`/`.hex`) lưu ở Flash**. Linker chỉ lưu lại đúng 2 thông tin (siêu dữ liệu - metadata) cấu hình: *Địa chỉ bắt đầu* và *Độ lớn vùng nhớ (40 KB)*. Khi bo mạch khởi động, đoạn mã `Startup code` sẽ đọc thông số này và chạy một vòng lặp để xóa trắng 40 KB trên RAM về `0`.
    * **Kết quả:** Flash tốn thêm $\approx 0\text{ KB}$ (thỏa mãn $< 20\text{ KB}$ trống). RAM tiêu tốn $40\text{ KB}$ (thỏa mãn $< 60\text{ KB}$ trống). Chương trình biên dịch và nạp xuống chip thành công.

* **Vì sao Cách 2 THẤT BẠI:**
    * Khi ta khởi tạo mảng bằng các giá trị khác không `{1, 2, 3, ...};`, Linker bắt buộc phải xếp toàn bộ mảng này vào phân vùng **`.data`**.
    * **Đặc điểm vùng `.data`:** Để giữ được các giá trị ban đầu (`1, 2, 3...`) không bị mất đi khi mất điện, **toàn bộ 40 KB dữ liệu định sẵn này bắt buộc phải được lưu trực tiếp trong bộ nhớ Flash**. Khi cấp nguồn, mã Startup mới tiến hành sao chép (copy) nguyên vẹn khối 40 KB dữ liệu thô này từ Flash sang vùng RAM động.
    * **Kết quả:** Flash bắt buộc phải gánh thêm trọn vẹn $40\text{ KB}$. Tuy nhiên dung lượng trống hiện tại của Flash chỉ còn $20\text{ KB}$. Trình liên kết sẽ lập tức báo lỗi vượt ngưỡng bộ nhớ: `Linker Error: Flash Overflow / Region FLASH overflowed`. Chương trình bị hủy bỏ ngay lập tức ở bước Link.
