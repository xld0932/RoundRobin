#include "KEY.h"
#include "RoundRobin.h"

#define KEY_PRESSED         Bit_SET
#define KEY_RELEASE         Bit_RESET

#define KEY1_GPIO_RCC       RCC_APB2Periph_GPIOC
#define KEY1_GPIO           GPIOC
#define KEY1_PIN            GPIO_Pin_13

uint8_t RR_BUFFER_DIM(KEY);

uint8_t BSP_KEY_GetState(uint8_t Index)
{
    uint8_t State = Bit_RESET;

    switch (Index)
    {
        case 1:
            State = GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN);
            break;

        default:
            break;
    }

    return (State);
}

void BSP_KEY_Function(char *Name, uint8_t Input)
{
    printf("\r\n%s %s.", Name, (Input) ? "Release" : "Pressed");

    if (strcmp(Name, "KEY1") == 0)
    {
        if (Input == KEY_PRESSED)
        {
            if (RoundRobin_GetState("LED") != eRunning)
            {
                RoundRobin_Resume("LED");
            }
            else
            {
                RoundRobin_Suspend("LED");
            }
        }
    }
}

void BSP_KEY_Scan(char *Name, uint8_t *State, uint8_t *Count, uint8_t Input)
{
    if (*State == 0)
    {
        if (Input == KEY_PRESSED)
        {
            *Count += 1;

            if (*Count >= 5)
            {
                *Count = 0;
                *State = 1;

                BSP_KEY_Function(Name, Input);
            }
        }
        else
        {
            *Count = 0;
        }
    }
    else
    {
        if (Input == KEY_RELEASE)
        {
            *Count += 1;

            if (*Count >= 5)
            {
                *Count = 0;
                *State = 0;

                BSP_KEY_Function(Name, Input);
            }
        }
        else
        {
            *Count = 0;
        }
    }
}

void BSP_KEY_Handler(void)
{
    static uint8_t KEY1_State = 0;
    static uint8_t KEY1_Count = 0;

    BSP_KEY_Scan("KEY1", &KEY1_State, &KEY1_Count, BSP_KEY_GetState(1));
}

void BSP_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* KEY1 */
    RCC_APB2PeriphClockCmd(KEY1_GPIO_RCC, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = KEY1_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_Init(KEY1_GPIO, &GPIO_InitStruct);

    /* KEY Handler */
    RoundRobin_AppendTask("KEY", 10, BSP_KEY_Handler, RR_BUFFER(KEY));
}

