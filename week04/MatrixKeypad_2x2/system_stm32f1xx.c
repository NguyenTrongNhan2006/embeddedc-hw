#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

#define HSE_VALUE 8000000U
#define HSE_STARTUP_TIMEOUT 0x5000U

/* System core clock frequency */
uint32_t SystemCoreClock = 72000000U;

/*
 * Prescaler lookup tables required by STM32 HAL RCC driver.
 *
 * Giá trị trong bảng là số bit dịch phải để suy ra
 * tần số clock sau bộ chia AHB/APB.
 */
const uint8_t AHBPrescTable[16U] =
    {
        0U, 0U, 0U, 0U,
        0U, 0U, 0U, 0U,
        1U, 2U, 3U, 4U,
        6U, 7U, 8U, 9U};

const uint8_t APBPrescTable[8U] =
    {
        0U, 0U, 0U, 0U,
        1U, 2U, 3U, 4U};

/**
 * @brief  System Clock Configuration
 *
 * Clock configuration:
 * - HSE:    8 MHz
 * - PLL:    HSE x 9
 * - SYSCLK: 72 MHz
 * - HCLK:   72 MHz
 * - PCLK1:  36 MHz
 * - PCLK2:  72 MHz
 */
void SystemInit(void)
{
    __IO uint32_t startup_counter = 0U;
    __IO uint32_t hse_status = 0U;

    /*
     * Reset RCC clock configuration to default state.
     */

    /* Enable HSI. */
    RCC->CR |= RCC_CR_HSION;

    /*
     * Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits.
     */
    RCC->CFGR &= 0xF0FF0000U;

    /*
     * Disable HSE, CSS and PLL.
     */
    RCC->CR &= 0xFEF6FFFFU;

    /*
     * Disable HSE bypass.
     */
    RCC->CR &= 0xFFFBFFFFU;

    /*
     * Reset PLL source, PLL prescaler, multiplier and USB prescaler.
     */
    RCC->CFGR &= 0xFF80FFFFU;

    /*
     * Disable RCC interrupts and clear pending interrupt flags.
     */
    RCC->CIR = 0x009F0000U;

    /*
     * Enable external high-speed oscillator.
     */
    RCC->CR |= RCC_CR_HSEON;

    /*
     * Wait until HSE becomes ready or timeout occurs.
     */
    do
    {
        hse_status = RCC->CR & RCC_CR_HSERDY;
        startup_counter++;
    } while ((hse_status == 0U) &&
             (startup_counter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != 0U)
    {
        hse_status = 1U;
    }
    else
    {
        hse_status = 0U;
    }

    if (hse_status == 1U)
    {
        /*
         * Enable Flash prefetch buffer.
         */
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        /*
         * Use two Flash wait states for 72 MHz operation.
         */
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        FLASH->ACR |= FLASH_ACR_LATENCY_2;

        /*
         * Clear all prescaler fields before applying
         * the required clock division values.
         */
        RCC->CFGR &=
            ~(RCC_CFGR_HPRE |
              RCC_CFGR_PPRE1 |
              RCC_CFGR_PPRE2 |
              RCC_CFGR_ADCPRE);

        /*
         * HCLK = SYSCLK / 1 = 72 MHz.
         */
        RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

        /*
         * PCLK2 = HCLK / 1 = 72 MHz.
         */
        RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

        /*
         * PCLK1 = HCLK / 2 = 36 MHz.
         */
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

        /*
         * ADC clock = PCLK2 / 6 = 12 MHz.
         */
        RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

        /*
         * USB clock = PLL clock / 1.5 = 48 MHz.
         */
        RCC->CFGR &= ~RCC_CFGR_USBPRE;

        /*
         * Configure PLL:
         * PLL source = HSE
         * PLL input  = 8 MHz
         * Multiplier = x9
         * PLL output = 72 MHz
         */
        RCC->CFGR &=
            ~(RCC_CFGR_PLLSRC |
              RCC_CFGR_PLLXTPRE |
              RCC_CFGR_PLLMULL);

        RCC->CFGR |=
            RCC_CFGR_PLLSRC |
            RCC_CFGR_PLLMULL9;

        /*
         * Enable PLL.
         */
        RCC->CR |= RCC_CR_PLLON;

        /*
         * Wait until PLL becomes ready.
         */
        while ((RCC->CR & RCC_CR_PLLRDY) == 0U)
        {
        }

        /*
         * Select PLL as system clock source.
         */
        RCC->CFGR &= ~RCC_CFGR_SW;
        RCC->CFGR |= RCC_CFGR_SW_PLL;

        /*
         * Wait until PLL is actually used as SYSCLK.
         */
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        {
        }

        SystemCoreClock = 72000000U;
    }
    else
    {
        /*
         * HSE failed. Continue running from HSI at 8 MHz.
         */
        RCC->CFGR &= ~RCC_CFGR_SW;
        RCC->CFGR |= RCC_CFGR_SW_HSI;

        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
        {
        }

        SystemCoreClock = 8000000U;
    }
}

/**
 * @brief  Update SystemCoreClock variable according to
 *         the current RCC register configuration.
 */
void SystemCoreClockUpdate(void)
{
    uint32_t system_clock_source;
    uint32_t pll_multiplier;
    uint32_t pll_source;
    uint32_t ahb_prescaler_index;

    /*
     * Read current SYSCLK source.
     */
    system_clock_source = RCC->CFGR & RCC_CFGR_SWS;

    switch (system_clock_source)
    {
    case RCC_CFGR_SWS_HSI:
    {
        SystemCoreClock = 8000000U;
        break;
    }

    case RCC_CFGR_SWS_HSE:
    {
        SystemCoreClock = HSE_VALUE;
        break;
    }

    case RCC_CFGR_SWS_PLL:
    {
        pll_multiplier = RCC->CFGR & RCC_CFGR_PLLMULL;
        pll_source = RCC->CFGR & RCC_CFGR_PLLSRC;

        /*
         * Decode PLL multiplier.
         *
         * STM32F1 encoding:
         * 0000 -> x2
         * 0001 -> x3
         * ...
         * 0111 -> x9
         */
        pll_multiplier = (pll_multiplier >> 18U) + 2U;

        if (pll_multiplier > 16U)
        {
            pll_multiplier = 16U;
        }

        if (pll_source == 0U)
        {
            /*
             * PLL source is HSI divided by two.
             */
            SystemCoreClock =
                (8000000U >> 1U) * pll_multiplier;
        }
        else
        {
            /*
             * PLL source is HSE.
             */
            if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != 0U)
            {
                SystemCoreClock =
                    (HSE_VALUE >> 1U) * pll_multiplier;
            }
            else
            {
                SystemCoreClock =
                    HSE_VALUE * pll_multiplier;
            }
        }

        break;
    }

    default:
    {
        SystemCoreClock = 8000000U;
        break;
    }
    }

    /*
     * Apply AHB prescaler.
     */
    ahb_prescaler_index =
        (RCC->CFGR & RCC_CFGR_HPRE) >> 4U;

    SystemCoreClock >>=
        AHBPrescTable[ahb_prescaler_index];
}