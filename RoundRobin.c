/*
 * RoundRobin V0.1
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

#include "RoundRobin.h"

RoundRobin_TypeDef *Head = NULL;

void RoundRobin_AppendTask(char *Name, uint32_t Interval, Function Handler, uint8_t * const RR_Buffer)
{
    RoundRobin_TypeDef *Node = Head;

    if (Head == NULL)
    {
        Head = (RoundRobin_TypeDef *)RR_Buffer;

        memset(Head->Name, 0x00, sizeof(Head->Name));
        memcpy(Head->Name, Name, (strlen(Name) > sizeof(Head->Name)) ? sizeof(Head->Name) : strlen(Name));

        Head->Interval = Interval;
        Head->RunFlag  = 0;
        Head->RunTick  = 0;
        Head->Handler  = Handler;
        Head->State    = eRunning;
        Head->Next     = NULL;
    }
    else
    {
        while (Node->Next != NULL)
        {
            if (Node->Handler == Handler)
            {
                memset(Node->Name, 0x00, sizeof(Node->Name));
                memcpy(Node->Name, Name, (strlen(Name) > sizeof(Node->Name)) ? sizeof(Node->Name) : strlen(Name));

                Node->Interval = Interval;

                return;
            }

            Node = Node->Next;
        }

        Node->Next = (RoundRobin_TypeDef *)RR_Buffer;

        Node = Node->Next;

        memset(Node->Name, 0x00, sizeof(Node->Name));
        memcpy(Node->Name, Name, (strlen(Name) > sizeof(Node->Name)) ? sizeof(Node->Name) : strlen(Name));

        Node->Interval = Interval;
        Node->RunFlag  = 0;
        Node->RunTick  = 0;
        Node->Handler  = Handler;
        Node->State    = eRunning;
        Node->Next     = NULL;
    }
}

RoundRobin_State RoundRobin_GetState(char *Name)
{
    RoundRobin_TypeDef *Node = Head;

    while (Node != NULL)
    {
        if (strcmp(Node->Name, Name) == 0)
        {
            return (Node->State);
        }

        Node = Node->Next;
    }

    return (eInvalid);
}

void RoundRobin_Suspend(char *Name)
{
    RoundRobin_TypeDef *Node = Head;

    while (Node != NULL)
    {
        if (strcmp(Node->Name, Name) == 0)
        {
            Node->State = eSuspend;
            break;
        }

        Node = Node->Next;
    }
}

void RoundRobin_Resume(char *Name)
{
    RoundRobin_TypeDef *Node = Head;

    while (Node != NULL)
    {
        if (strcmp(Node->Name, Name) == 0)
        {
            Node->State = eRunning;
            break;
        }

        Node = Node->Next;
    }
}

void RoundRobin_Scheduling(void)
{
    RoundRobin_TypeDef *Node = Head;

    while (Node != NULL)
    {
        Node->RunTick++;

        if (Node->Interval)
        {
            if ((Node->RunTick % Node->Interval) == 0)
            {
                if (Node->State == eRunning)
                {
                    Node->RunFlag = 1;
                }
            }
        }

        Node = Node->Next;
    }
}

void RoundRobin_Processing(void)
{
    uint32_t StartRunTick = 0;
    RoundRobin_TypeDef *Node = Head;

    while (Node != NULL)
    {
        if (Node->RunFlag == 1)
        {
            if (Node->State == eRunning)
            {
                StartRunTick  = Node->RunTick;
                Node->Handler();
                Node->RunTime = Node->RunTick - StartRunTick;
            }

            Node->RunFlag = 0;
            Node->RunTick = 0;
        }

        Node = Node->Next;
    }
}

void RoundRobin_List(void)
{
    RoundRobin_TypeDef *Node = Head;

    printf("\r\n\r\n%s :", __FUNCTION__);

    while (Node != NULL)
    {
        printf("\r\n%s, %dms", Node->Name, Node->Interval);

        Node = Node->Next;
    }

    printf("\r\n");
}

