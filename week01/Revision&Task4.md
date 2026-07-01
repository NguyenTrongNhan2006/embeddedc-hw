# HW01: HỆ THỐNG GIÁM SÁT LÒ NHIỆT CÔNG NGHIỆP

note : Đây là những phần em tự học và hiểu code thông qua bài tập về code và FSM của lab 

## Task 1 (Pointers & Memory): Bóc tách gói tin cấu hình

- Hiểu cách dùng con trỏ để truyền địa chỉ biến vào hàm.
- Biết kiểm tra con trỏ `NULL` để tránh chương trình bị crash.
- Biết dùng toán tử `*` để ghi giá trị trực tiếp vào vùng nhớ mà con trỏ đang trỏ tới.
- Hiểu `config_packet` là mảng dữ liệu thô gồm các byte cấu hình.
- Biết ghép 2 byte thành một số `int16_t`.
- Hiểu quy tắc Little Endian: byte thấp LSB nằm trước, byte cao MSB nằm sau.
- Biết dùng phép dịch bit `<< 8` để đưa byte cao về đúng vị trí.
- Biết ép kiểu dữ liệu khi xử lý từ `uint8_t` sang `uint16_t`, sau đó lưu về `int16_t`.

## Task 2 (Compiler & Volatile): Đọc thanh ghi cảm biến phần cứng

- Hiểu `void *` là con trỏ tổng quát, cần ép kiểu trước khi đọc dữ liệu.
- Biết ép `void *` về `volatile int16_t *` để đọc thanh ghi cảm biến.
- Hiểu ý nghĩa của `volatile` trong lập trình nhúng.
- Biết `volatile` giúp compiler không tối ưu hóa sai khi đọc giá trị phần cứng trong vòng lặp.
- Hiểu giá trị trong hardware register có thể thay đổi ngoài chương trình C.
- Biết dùng toán tử `*` để đọc giá trị thật từ địa chỉ thanh ghi.
- Biết kiểm tra `NULL` để hàm trả về an toàn khi địa chỉ cảm biến không hợp lệ.

## Task 3 (Data Types & Bitwise): Thao tác bit điều khiển thiết bị xuất
- Hiểu cách điều khiển thiết bị ngoại vi thông qua thanh ghi 8-bit.
- Biết Bit 0 dùng để điều khiển quạt làm mát.
- Biết Bit 1 dùng để điều khiển đèn báo động.
- Biết các bit từ Bit 2 đến Bit 7 phải được giữ nguyên.
- Biết dùng bit mask để chọn đúng bit cần thay đổi.
- Biết dùng toán tử `|` để bật bit.
- Biết dùng toán tử `&` và `~` để xóa bit.
- Biết dùng `<<` để tạo mask theo vị trí bit.
- Hiểu quy trình clear-then-set: xóa bit cũ trước, sau đó gán trạng thái mới.
- Tránh lỗi gán trực tiếp như `*control_reg = 0x01` vì sẽ làm mất dữ liệu các bit khác.

### Testing & Debugging 

- Biết viết testcase để kiểm tra từng task.
- Biết test trường hợp `NULL` để tránh crash chương trình.
- Biết test các giá trị biên như `INT16_MAX`, `INT16_MIN`, `-1`.
- Biết test Little Endian bằng các byte như `{0x34, 0x12}`.
- Biết dùng biến giả lập để test thanh ghi cảm biến phần cứng.
- Biết test bitwise bằng cách kiểm tra giá trị thanh ghi trước và sau khi điều khiển.
- Hiểu tầm quan trọng của việc kiểm tra cả input bình thường và input dễ lỗi.

  
# Task 4 - FSM 

## Description
Thiết kế sơ đồ FSM theo yêu cầu bài tập "  dùng moore machine 

- Hiểu cách mô tả hệ thống bằng Finite State Machine.
- Biết chia hệ thống thành các trạng thái: `STATE_SAFE`, `STATE_WARNING`, `STATE_EMERGENCY`.
- Hiểu trạng thái `STATE_SAFE`: quạt tắt, đèn tắt.
- Hiểu trạng thái `STATE_WARNING`: quạt bật, đèn tắt khi nhiệt độ vượt ngưỡng.
- Hiểu trạng thái `STATE_EMERGENCY`: hệ thống vào cảnh báo khẩn cấp khi nhiệt độ vượt ngưỡng quá 5 giây.
- Biết xác định điều kiện chuyển trạng thái dựa trên nhiệt độ hiện tại và `high_threshold`.
- Hiểu FSM giúp hệ thống hoạt động rõ ràng, an toàn và dễ kiểm soát hơn.


## States
- **SAFE**: Normal state. The fan and LED are OFF because the temperature is below the threshold.
- **WARNING**: The fan and LED are ON. The timer starts when the temperature reaches or exceeds the threshold.
- **EMERGENCY**: Activated when the temperature remains above the threshold for more than 5 seconds. The system enters emergency mode.

## FSM Diagram ( using paper write ) 
<img width="1920" height="2560" alt="z7997610256349_d79774af33e5760f6a90b04a5b2fcd6b" src="https://github.com/user-attachments/assets/f87bff83-f642-4d62-a8c2-399ceadf7b1a" />



