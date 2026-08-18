# RoundRobin

#### Description
RoundRobin is a framework that can be used for managing task time slice scheduling in embedded MCU software

#### Software Architecture
Software architecture description

1.  Based on the time-based timer, the RoundRobin_Scheduling function is called in the timer interrupt to judge the time slice of the task
2.  In the main loop, call the RoundRobin_Processing function to run every task that has reached its time slice
3.  By calling the RoundRobin_AppendTask function, a time slice task is added

#### Instructions（STM32）

1.  Timer Interrupt

```
void SysTick_Handler(void)
{
    RoundRobin_Scheduling();
}
```

2.  Main Loop

```
int main(void)
{
    ......

    RoundRobin_List();

    while (1)
    {
        RoundRobin_Processing();
    }
}
```

3.  Append Task
- Define Task Struct

```
uint8_t RR_BUFFER_DIM(LED);
```

- Task Function

```
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
```

- Create Task

```
RoundRobin_AppendTask("LED", 200, BSP_LED_Handler, RR_BUFFER(LED));
```
Create an LED task and call the BSP_LED_Handler function every 200ms

- Task Control

```
RoundRobin_State RoundRobin_GetState(char *Name);    //Get the current running status of the task
void RoundRobin_Suspend(char *Name);                 //Suspend the task
void RoundRobin_Resume(char *Name);                  //Resume the task
```
