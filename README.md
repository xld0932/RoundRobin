# RoundRobin

#### 介绍
RoundRobin是一套可以作为嵌入式MCU软件管理任务时间片调度的框架

#### 软件架构
软件架构说明
1.  基于时基定时器，在定时器中断中调用RoundRobin_Scheduling函数，对任务的时间片进行判断
2.  在主循环中调用RoundRobin_Processing函数，运行每一个达到时间片的任务
3.  通过调用RoundRobin_AppendTask函数，添加一个时间片任务

#### 使用说明（STM32为例）

1.  时基定时器中断

```
void SysTick_Handler(void)
{
    RoundRobin_Scheduling();
}
```

2.  主循环

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

3.  创建添加任务
- 定义任务结构体

```
uint8_t RR_BUFFER_DIM(LED);
```

- 实现任务函数

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
- 创建任务

```
RoundRobin_AppendTask("LED", 200, BSP_LED_Handler, RR_BUFFER(LED));
```
创建LED任务，每间隔200ms调用一次BSP_LED_Handler函数

- 任务控制

```
RoundRobin_State RoundRobin_GetState(char *Name);    //获取任务当前的运行状态
void RoundRobin_Suspend(char *Name);                 //挂起任务，任务暂停运行
void RoundRobin_Resume(char *Name);                  //恢复任务，任务重启运行
```
