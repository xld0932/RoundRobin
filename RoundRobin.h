/*
 * RoundRobin V0.1.1
 * 
 * MIT License
 * 
 * Copyright (c) 2026 xld0932, xld0932@vip.qq.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://github.com/xld0932/RoundRobin
 * https://gitee.com/xld0932/round-robin
 * 
 * Change Logs:
 * Date           Author        Notes
 * 2026-08-18     xld0932       V0.1.0
 */

#ifndef __ROUNDROBIN_H__
#define __ROUNDROBIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#ifdef __C51__
typedef unsigned char   uint8_t;
typedef unsigned int    uint16_t;
typedef unsigned long   uint32_t;
#else
#include <stdint.h>
#endif

typedef void (* Function)(void);

typedef enum
{
    eRunning = 0,
    eSuspend,
    eInvalid
} RoundRobin_State;

typedef struct RoundRobin
{
    char Name[10];
    uint16_t Interval;
    uint16_t RunTick;
    uint16_t RunFlag;
    uint16_t RunTime;
    Function Handler;
    RoundRobin_State State;
    struct RoundRobin *Next;
} RoundRobin_TypeDef;

#define RR_BUFFER_SIZE          sizeof(RoundRobin_TypeDef)
#define RR_BUFFER(NAME)         NAME##_RR_Buffer
#define RR_BUFFER_DIM(NAME)     NAME##_RR_Buffer[RR_BUFFER_SIZE]

void RoundRobin_AppendTask(char *Name, uint16_t Interval, Function Handler, uint8_t * const RR_Buffer);
RoundRobin_State RoundRobin_GetState(char *Name);
void RoundRobin_Suspend(char *Name);
void RoundRobin_Resume(char *Name);
void RoundRobin_Scheduling(void);
void RoundRobin_Processing(void);
void RoundRobin_List(void);

#ifdef __cplusplus
}
#endif

#endif

