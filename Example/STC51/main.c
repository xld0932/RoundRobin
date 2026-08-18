#include "STC89C5xRC.h"
#include "RoundRobin.h"

#define FOSC    (11059200L)
#define T1MS    (65536 - FOSC / 12 / 1000)
#define BAUD    (9600)

////////////////////////////////////////////////////////////////////////////////
#define LED_ON          (0)
#define LED_OFF         (1)

sbit LED1 = P3 ^ 7;
sbit LED2 = P2 ^ 6;

uint8_t RR_BUFFER_DIM(LED);

void BSP_LED_Handler(void)
{
    static uint8_t Count = 0;

    if (Count < 4)
    {
        LED1 = !LED1;
    }
    else
    {
        LED2 = !LED2;
    }

    Count += 1;
    Count %= 6;
}

void BSP_LED_Init(void)
{
    LED1 = LED_OFF;
    LED2 = LED_OFF;

    RoundRobin_AppendTask("LED", 200, BSP_LED_Handler, RR_BUFFER(LED));
}

////////////////////////////////////////////////////////////////////////////////
#define KEY_PRESSED     (0)
#define KEY_RELEASE     (1)

sbit KEY1 = P3 ^ 6;
sbit KEY2 = P2 ^ 7;

uint8_t RR_BUFFER_DIM(KEY);

uint8_t BSP_KEY_GetState(uint8_t Index)
{
    uint8_t State = 0;

    switch (Index)
    {
        case 1:
            State = KEY1;
            break;

        case 2:
            State = KEY2;
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
            if (RoundRobin_GetState("LED") == eRunning)
            {
                RoundRobin_Suspend("LED");
            }
        }
    }

    if (strcmp(Name, "KEY2") == 0)
    {
        if (Input == KEY_PRESSED)
        {
            if (RoundRobin_GetState("LED") == eSuspend)
            {
                RoundRobin_Resume("LED");
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
    static uint8_t KEY1_State = 0, KEY1_Count = 0;
    static uint8_t KEY2_State = 0, KEY2_Count = 0;

    BSP_KEY_Scan("KEY1", &KEY1_State, &KEY1_Count, BSP_KEY_GetState(1));
    BSP_KEY_Scan("KEY2", &KEY2_State, &KEY2_Count, BSP_KEY_GetState(2));
}

void BSP_KEY_Init(void)
{
    RoundRobin_AppendTask("KEY", 10, BSP_KEY_Handler, RR_BUFFER(KEY));
}

////////////////////////////////////////////////////////////////////////////////
void InitTIM0(void)
{
	AUXR &= 0x7F;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0   = T1MS;
	TH0   = T1MS >> 8;
	TF0   = 0;
	TR0   = 1;
    ET0   = 1;
}

void TIM0_ISR(void) interrupt 1
{
    TL0 = T1MS;
    TH0 = T1MS >> 8;

    RoundRobin_Scheduling();
}

////////////////////////////////////////////////////////////////////////////////
bit  TxBusy = 0;
char RxData = 0;

void InitUART1(void)
{
	SCON  = 0x50;
	TMOD &= 0x0F;
	TMOD |= 0x20;
    TH1 = TL1 = -(FOSC/12/32/BAUD);
    ET1   = 0;
    TR1   = 1;
    ES    = 1;
}

void UART1_ISR(void) interrupt 4
{
    if (RI)
    {
        RI     = 0;
        RxData = SBUF;
    }

    if (TI)
    {
        TI     = 0;
        TxBusy = 0;
    }
}

char putchar(char ch)
{
    while (TxBusy)
    {
    }

    TxBusy = 1;
    SBUF   = ch;

    return ch;
}

////////////////////////////////////////////////////////////////////////////////
void main(void)
{
    InitTIM0();

    InitUART1();

    EA = 1;

    printf("\r\n%s %s", __DATE__, __TIME__);

    BSP_LED_Init();
    BSP_KEY_Init();

    RoundRobin_List();

    while (1)
    {
        RoundRobin_Processing();
    }
}

