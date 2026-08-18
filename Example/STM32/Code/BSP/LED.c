#include "LED.h"
#include "RoundRobin.h"

#define LED_ON              Bit_RESET
#define LED_OFF             Bit_SET

#define LED1_GPIO_RCC       RCC_APB2Periph_GPIOC
#define LED1_GPIO           GPIOC
#define LED1_PIN            GPIO_Pin_1

#define LED2_GPIO_RCC       RCC_APB2Periph_GPIOC
#define LED2_GPIO           GPIOC
#define LED2_PIN            GPIO_Pin_0

uint8_t RR_BUFFER_DIM(LED);

void BSP_LED_Handler(void)
{
    static uint8_t Count = 0;

    if (Count < 4)
    {
        GPIO_WriteBit(LED1_GPIO, LED1_PIN, GPIO_ReadOutputDataBit(LED1_GPIO, LED1_PIN) ? LED_ON : LED_OFF);
    }
    else
    {
        GPIO_WriteBit(LED2_GPIO, LED2_PIN, GPIO_ReadOutputDataBit(LED2_GPIO, LED2_PIN) ? LED_ON : LED_OFF);
    }

    Count += 1;
    Count %= 6;
}

void BSP_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* LED1 */
    RCC_APB2PeriphClockCmd(LED1_GPIO_RCC, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = LED1_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_GPIO, &GPIO_InitStruct);

    GPIO_WriteBit(LED1_GPIO, LED1_PIN, LED_OFF);

    /* LED2 */
    RCC_APB2PeriphClockCmd(LED2_GPIO_RCC, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = LED2_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LED2_GPIO, &GPIO_InitStruct);

    GPIO_WriteBit(LED2_GPIO, LED2_PIN, LED_OFF);

    /* LED Handler */
    RoundRobin_AppendTask("LED", 200, BSP_LED_Handler, RR_BUFFER(LED));
}

