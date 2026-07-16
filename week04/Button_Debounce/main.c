#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#define BUTTON_PORT GPIOA
#define BUTTON_PIN GPIO_PIN_0

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_1

#define DEBOUNCE_TIME_MS 20U

static void SystemClock_Config(void);
static void GPIO_Init(void);
static bool Button_Pressed_And_Released(void);

int main(void)
{
    bool led_state = false;

    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    while (1)
    {
        /*
         * Hàm chỉ trả về true đúng một lần sau chu trình:
         * nhấn -> giữ -> thả.
         */
        if (Button_Pressed_And_Released())
        {
            led_state = !led_state;

            HAL_GPIO_WritePin(
                LED_PORT,
                LED_PIN,
                led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}

/*
 * Clock mặc định dùng HSI 8 MHz.
 * Không PLL để cấu hình đơn giản và ổn định.
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef oscillator_config = {0};
    RCC_ClkInitTypeDef clock_config = {0};

    oscillator_config.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    oscillator_config.HSIState = RCC_HSI_ON;
    oscillator_config.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    oscillator_config.PLL.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&oscillator_config) != HAL_OK)
    {
        while (1)
        {
        }
    }

    clock_config.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    clock_config.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clock_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clock_config.APB1CLKDivider = RCC_HCLK_DIV1;
    clock_config.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clock_config, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1)
        {
        }
    }
}

static void GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_config = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*
     * PA1: LED Output Push-Pull
     */
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

    gpio_config.Pin = LED_PIN;
    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_PORT, &gpio_config);

    /*
     * PA0: Button Input Pull-up
     */
    gpio_config.Pin = BUTTON_PIN;
    gpio_config.Mode = GPIO_MODE_INPUT;
    gpio_config.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(BUTTON_PORT, &gpio_config);
}

static bool Button_Pressed_And_Released(void)
{
    static GPIO_PinState raw_state = GPIO_PIN_SET;
    static GPIO_PinState stable_state = GPIO_PIN_SET;

    static uint32_t debounce_start_tick = 0U;
    static bool press_detected = false;

    GPIO_PinState current_state =
        HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

    /*
     * Tín hiệu vừa thay đổi:
     * bắt đầu tính lại thời gian debounce.
     */
    if (current_state != raw_state)
    {
        raw_state = current_state;
        debounce_start_tick = HAL_GetTick();
    }

    /*
     * Chỉ công nhận trạng thái mới sau khi ổn định 20 ms.
     */
    if ((HAL_GetTick() - debounce_start_tick) >= DEBOUNCE_TIME_MS)
    {
        if (stable_state != raw_state)
        {
            stable_state = raw_state;

            /*
             * Pull-up:
             * GPIO_PIN_RESET nghĩa là đang nhấn.
             */
            if (stable_state == GPIO_PIN_RESET)
            {
                press_detected = true;
            }
            /*
             * Chỉ đảo LED khi đã nhấn rồi thả.
             */
            else if (press_detected)
            {
                press_detected = false;
                return true;
            }
        }
    }

    return false;
}