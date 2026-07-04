// HW02: KIẾN TRÚC PHẦN MỀM VÀ QUẢN LÝ BỘ NHỚ SMART BIKE

#include <stdio.h>
#include <stdint.h>

// TASK 1: STRUCT, UNION & BIT-FIELDS

typedef union {
    uint16_t raw_value;
    struct {
        // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

        uint16_t PWR_ON        : 1;
        uint16_t ASSIST_LEVEL  : 2;
        uint16_t LIGHT_BRIGHT  : 4;
        uint16_t RESERVED      : 9;

        // HỌC VIÊN KẾT THÚC VIẾT CODE
    } fields;
} Bike_Status_t;


// TASK 2: FUNCTION POINTERS & TABLE-DRIVEN ARCHITECTURE

void drive_eco(void) {
    printf("Bike Mode: ECO.\n");
}
void drive_normal(void) {
    printf("Bike Mode: NORMAL.\n");
}
void drive_sport(void) {
    printf("Bike Mode: SPORT.\n");
}

// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

typedef void (*Drive_Mode_Handler_t)(void);

#define DRIVE_MODE_COUNT 3U

Drive_Mode_Handler_t drive_modes[DRIVE_MODE_COUNT] = {
    drive_eco,
    drive_normal,
    drive_sport
};


// HỌC VIÊN KẾT THÚC VIẾT CODE


// TASK 3: ARCHITECTURE CALLBACKS

void Battery_Monitor(void (*overheat_cb)(void)) {
    int battery_temp = 45; 
    
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    if ((battery_temp > 40) && (overheat_cb != NULL)) {
        (*overheat_cb)();
    }


    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

void Critical_Battery_Handler(void) {
    printf("[WARNING] Battery is overheated!\n");
}


// TASK 4: MEMORY MAP & CRASH LAB

const char BIKE_MODEL[] = "E-Bike X2026"; 
uint32_t total_odometer = 0;             

static void stack_overflow_recursive(unsigned depth) {
    volatile uint8_t stack_load[1024];
    volatile unsigned keep_running = 1U;

    stack_load[0] = (uint8_t)depth;
    stack_load[sizeof(stack_load) - 1U] = (uint8_t)(depth >> 8);

    if (keep_running != 0U) {
        stack_overflow_recursive(depth + 1U);
    }
}

void crash_simulation(void) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    stack_overflow_recursive(0U);


    // HỌC VIÊN KẾT THÚC VIẾT CODE
}


// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm)

int main() {
    Bike_Status_t my_bike;
    
    // 1. Test Task 1
    my_bike.raw_value = 0x0025; 
    printf("REGISTER STRUCTURE: \n");
    printf("PWR_ON: %d | ASSIST_LEVEL: %d | LIGHT_BRIGHT: %d\n\n", 
           my_bike.fields.PWR_ON, my_bike.fields.ASSIST_LEVEL, my_bike.fields.LIGHT_BRIGHT);

    // 2. Test Task 2
    printf("ENGINE CONTROLLING: \n");
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

    if ((my_bike.fields.ASSIST_LEVEL < DRIVE_MODE_COUNT) &&
        (drive_modes[my_bike.fields.ASSIST_LEVEL] != NULL)) {
        (*drive_modes[my_bike.fields.ASSIST_LEVEL])();
    } else {
        printf("Invalid assist level.\n");
    }


    // HỌC VIÊN KẾT THÚC VIẾT CODE

    // 3. Test Task 3
    Battery_Monitor(Critical_Battery_Handler);
    printf("\n");

    // 4. Test Task 4
    int current_speed = 25; 
    printf("MEMORY ADDRESSES OF VARIABLES \n");
    printf("BIKE_MODEL (ROM/.rodata):   %p\n", (void*)BIKE_MODEL);
    printf("total_odometer (RAM/.bss):  %p\n", (void*)&total_odometer);
    printf("current_speed (RAM/Stack):  %p\n", (void*)&current_speed);

    // Bỏ comment dòng dưới để chạy thử bài Crash Lab
    // crash_simulation();

    return 0;
}
