#include "stm32f1xx_hal.h"
#include <string.h>

UART_HandleTypeDef huart1;

static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void UART_SendChar(char c);
static void UART_SendString(const char *s);

int main(void) {
    HAL_Init();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    UART_SendString("BCA182 FreeRTOS Multisensor\r\n");
    UART_SendString("System starting...\r\n");

    while (1) {
        // Main loop
    }
}

// Required so HAL's internal tick counter (used by HAL_Delay, timeouts, etc.) actually advances
void SysTick_Handler(void) {
    HAL_IncTick();
}

static void UART_SendChar(char c) {
    uint32_t guard = 0;
    while (!(USART1->SR & USART_SR_TXE)) {
        if (++guard > 1000000) return;
    }
    USART1->DR = (uint8_t)c;
}

static void UART_SendString(const char *s) {
    while (*s) {
        UART_SendChar(*s++);
    }
}

static void MX_USART1_UART_Init(void) {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
}

static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
}