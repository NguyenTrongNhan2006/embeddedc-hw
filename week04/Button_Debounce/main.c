#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

/* =========================================================
 * PIN DEFINITIONS
 * ========================================================= */

/* LED ngoài nối vào PA0 */
#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_0

/* Nút nhấn nối vào PA1 */
#define BUTTON_PORT GPIOA
#define BUTTON_PIN GPIO_PIN_1

#define DEBOUNCE_TIME_MS 20U

/* =========================================================
 * FUNCTION PROTOTYPES
 * ========================================================= */

static void GPIO_Init(void);
static bool Button_Pressed_And_Released(void);

/* =========================================================
 * SYSTICK INTERRUPT
 * ========================================================= */

/*
 * HAL_Delay() và HAL_GetTick() cần SysTick tăng mỗi 1 ms.
 * Template hiện tại không có stm32f1xx_it.c nên xử lý tại đây.
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/* =========================================================
 * MAIN
 * ========================================================= */

int main(void)
{
    bool led_state = false;

    /*
     * HAL_Init() khởi tạo HAL và SysTick.
     *
     * SystemInit() trong system_stm32f1xx.c đã cấu hình clock.
     */
    HAL_Init();

    GPIO_Init();

    while (1)
    {
        /*
         * Hàm trả về true đúng một lần sau chu trình:
         *
         * nhấn -> giữ -> thả
         */
        if (Button_Pressed_And_Released() == true)
        {
            /*
             * Đảo trạng thái logic của LED.
             */
            led_state = !led_state;

            if (led_state == true)
            {
                /*
                 * PA0 = HIGH, khoảng 3.3 V.
                 * LED sáng nếu mắc từ PA0 qua điện trở xuống GND.
                 */
                HAL_GPIO_WritePin(
                    LED_PORT,
                    LED_PIN,
                    GPIO_PIN_SET);
            }
            else
            {
                /*
                 * PA0 = LOW, LED tắt.
                 */
                HAL_GPIO_WritePin(
                    LED_PORT,
                    LED_PIN,
                    GPIO_PIN_RESET);
            }
        }
    }
}

/* =========================================================
 * GPIO INITIALIZATION
 * ========================================================= */

static void GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_config = {0};

    /*
     * PA0 và PA1 đều thuộc GPIOA.
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* -----------------------------------------------------
     * PA0: LED Output Push-Pull
     * ----------------------------------------------------- */

    /*
     * Đặt PA0 ở LOW trước khi cấu hình Output,
     * để LED ban đầu tắt.
     */
    HAL_GPIO_WritePin(
        LED_PORT,
        LED_PIN,
        GPIO_PIN_RESET);

    gpio_config.Pin = LED_PIN;
    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Pull = GPIO_NOPULL;
    gpio_config.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(
        LED_PORT,
        &gpio_config);

    /* -----------------------------------------------------
     * PA1: Button Input Pull-up
     * ----------------------------------------------------- */

    /*
     * Với Pull-up:
     *
     * Không nhấn -> PA1 đọc HIGH
     * Nhấn       -> PA1 đọc LOW
     */
    gpio_config.Pin = BUTTON_PIN;
    gpio_config.Mode = GPIO_MODE_INPUT;
    gpio_config.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(
        BUTTON_PORT,
        &gpio_config);
}

/* =========================================================
 * BUTTON DEBOUNCE
 * ========================================================= */

/*
 * Hàm chống dội và chống giữ.
 *
 * Hàm chỉ trả về true đúng một lần sau khi:
 *
 * 1. Nút được nhấn ổn định.
 * 2. Nút được thả ổn định.
 */
static bool Button_Pressed_And_Released(void)
{
    /*
     * Trạng thái đọc tức thời từ PA1.
     */
    static GPIO_PinState raw_state = GPIO_PIN_SET;

    /*
     * Trạng thái đã được xác nhận sau debounce.
     */
    static GPIO_PinState stable_state = GPIO_PIN_SET;

    /*
     * Thời điểm trạng thái tức thời thay đổi.
     */
    static uint32_t debounce_start_tick = 0U;

    /*
     * Ghi nhớ rằng đã có một lần nhấn hợp lệ.
     */
    static bool press_detected = false;

    GPIO_PinState current_state;

    current_state = HAL_GPIO_ReadPin(
        BUTTON_PORT,
        BUTTON_PIN);

    /*
     * Nếu tín hiệu vừa thay đổi,
     * bắt đầu tính lại thời gian debounce.
     */
    if (current_state != raw_state)
    {
        raw_state = current_state;
        debounce_start_tick = HAL_GetTick();
    }

    /*
     * Chỉ chấp nhận trạng thái mới nếu giữ ổn định
     * ít nhất DEBOUNCE_TIME_MS.
     */
    if ((HAL_GetTick() - debounce_start_tick) >= DEBOUNCE_TIME_MS)
    {
        /*
         * Chỉ xử lý khi trạng thái ổn định thực sự thay đổi.
         */
        if (stable_state != raw_state)
        {
            stable_state = raw_state;

            /*
             * Vì dùng Input Pull-up:
             *
             * GPIO_PIN_RESET = đang nhấn.
             */
            if (stable_state == GPIO_PIN_RESET)
            {
                press_detected = true;
            }

            /*
             * GPIO_PIN_SET = đã thả.
             *
             * Chỉ tạo sự kiện nếu trước đó đã có lần nhấn.
             */
            else if ((stable_state == GPIO_PIN_SET) &&
                     (press_detected == true))
            {
                press_detected = false;

                return true;
            }
        }
    }

    return false;
}