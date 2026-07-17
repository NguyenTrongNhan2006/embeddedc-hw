#include "stm32f1xx_hal.h"

/* Hai hàng của ma trận */
#define ROW1_PIN GPIO_PIN_0
#define ROW2_PIN GPIO_PIN_1

/* Hai cột của ma trận */
#define COL1_PIN GPIO_PIN_2
#define COL2_PIN GPIO_PIN_3

/* Bốn LED */
#define LED1_PIN GPIO_PIN_4
#define LED2_PIN GPIO_PIN_5
#define LED3_PIN GPIO_PIN_6
#define LED4_PIN GPIO_PIN_7

static void GPIO_Init(void);

/*
 * Giữ phần này vì project hiện tại của bạn
 * không có SysTick_Handler trong stm32f1xx_it.c.
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

int main(void)
{
    HAL_Init();

    GPIO_Init();

    while (1)
    {
        /*
         * Quét hàng 1:
         * PA0 = 0
         * PA1 = 1
         */
        HAL_GPIO_WritePin(GPIOA, ROW1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, ROW2_PIN, GPIO_PIN_SET);

        /*
         * Chờ tín hiệu ổn định.
         */
        HAL_Delay(1);

        /*
         * SW1: ROW1 + COL1
         * Điều khiển LED tại PA4.
         */
        if (HAL_GPIO_ReadPin(GPIOA, COL1_PIN) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(GPIOA, LED1_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, LED1_PIN, GPIO_PIN_RESET);
        }

        /*
         * SW2: ROW1 + COL2
         * Điều khiển LED tại PA5.
         */
        if (HAL_GPIO_ReadPin(GPIOA, COL2_PIN) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(GPIOA, LED2_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, LED2_PIN, GPIO_PIN_RESET);
        }

        /*
         * Quét hàng 2:
         * PA0 = 1
         * PA1 = 0
         */
        HAL_GPIO_WritePin(GPIOA, ROW1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, ROW2_PIN, GPIO_PIN_RESET);

        HAL_Delay(1);

        /*
         * SW3: ROW2 + COL1
         * Điều khiển LED tại PA6.
         */
        if (HAL_GPIO_ReadPin(GPIOA, COL1_PIN) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(GPIOA, LED3_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, LED3_PIN, GPIO_PIN_RESET);
        }

        /*
         * SW4: ROW2 + COL2
         * Điều khiển LED tại PA7.
         */
        if (HAL_GPIO_ReadPin(GPIOA, COL2_PIN) == GPIO_PIN_RESET)
        {
            HAL_GPIO_WritePin(GPIOA, LED4_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, LED4_PIN, GPIO_PIN_RESET);
        }

        /*
         * Trả cả hai hàng về mức 1.
         */
        HAL_GPIO_WritePin(GPIOA, ROW1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, ROW2_PIN, GPIO_PIN_SET);

        HAL_Delay(5);
    }
}

static void GPIO_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    /*
     * PA0 đến PA7 đều thuộc GPIOA.
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*
     * Hai hàng ban đầu ở mức 1.
     */
    HAL_GPIO_WritePin(
        GPIOA,
        ROW1_PIN | ROW2_PIN,
        GPIO_PIN_SET);

    /*
     * Bốn LED ban đầu tắt.
     */
    HAL_GPIO_WritePin(
        GPIOA,
        LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN,
        GPIO_PIN_RESET);

    /*
     * PA0 và PA1:
     * Digital Output cho hai hàng.
     *
     * PA4, PA5, PA6 và PA7:
     * Digital Output cho bốn LED.
     */
    gpio.Pin = ROW1_PIN |
               ROW2_PIN |
               LED1_PIN |
               LED2_PIN |
               LED3_PIN |
               LED4_PIN;

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &gpio);

    /*
     * PA2 và PA3:
     * Digital Input Pull-up cho hai cột.
     *
     * Không nhấn: đọc được 1.
     * Có nhấn: đọc được 0.
     */
    gpio.Pin = COL1_PIN | COL2_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOA, &gpio);
}