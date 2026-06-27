// HW01: HỆ THỐNG GIÁM SÁT LÒ NHIỆT CÔNG NGHIỆP

#include <stdio.h>  // standard input/output
#include <stdint.h> // fixed-width integer types

// TASK 1: POINTERS & MEMORY : ( Bóc tách gói tin cấu hình )

// const uint8_t *config_packet: con trỏ hằng trỏ đến gói tin cấu hình , dữ liệu đầu vào, gồm 2 byte
// int16_t *high_threshold: con trỏ đến biến lưu giá trị ngưỡng , nơi lưu kết quả sau khi ghép 2 byte thành số nhiệt độ

void parse_config(const uint8_t *config_packet, int16_t *high_threshold)
{
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    // Kiểm tra NULL pointer để tránh crash
    if (config_packet == 0 || high_threshold == 0)
    {
        return;
    }
    // Ghép 2 byte từ config_packet LSB & MSB thành một số nguyên 16-bit và lưu vào high_threshold
    *high_threshold = (int16_t)(((uint16_t)config_packet[0]) |
                                ((uint16_t)config_packet[1] << 8));

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 2: COMPILER & VOLATILE

int16_t read_temperature_reg(void *hw_sensor_reg)
{
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 3: DATA TYPES & BITWISE OPERATIONS

void control_output(uint8_t *control_reg, uint8_t fan_enable, uint8_t alarm_enable)
{
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm, chỉ thay đổi input nếu cần)

int main()
{

    // 1. Test Task 1
    int16_t threshold = 0;
    uint8_t mock_packet[2] = {0x64, 0x00};
    parse_config(mock_packet, &threshold); // Có thể thay mock_packet thành NULL để test chống crash khi gặp NULL
    printf("Threshold Parsed: %d C\n", threshold);

    // 2. Test Task 2
    volatile int16_t mock_hardware_sensor = 105;
    int16_t current_temp = read_temperature_reg((void *)&mock_hardware_sensor);
    printf("Current Temp Read: %d C\n", current_temp);

    // 3. Test Task 3
    uint8_t system_control_register = 0xFC; // 1111 1100
    if (current_temp >= threshold)
    {
        // Yêu cầu: fan = 1, alarm = 0
        control_output(&system_control_register, 1, 0);
    }
    printf("Control Reg Output: 0x%02X\n", system_control_register);

    return 0;
}