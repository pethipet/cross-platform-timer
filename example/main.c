/**
 *  Copyright 2023 Peter Himmler
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "crosstimer.h"

#if defined(WIN32)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#    include <WinSock2.h>
#    include <windows.h>
#else
#    include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

void threadSleep(unsigned long s)
{
#if defined(WIN32)
    Sleep(s);
#else
    sleep(s / 1000);
#endif
}

int  counter = 0;

void cb(void * arg)
{
    volatile int * pc = (int *)arg;
    printf("Hallo cb: %d\n", *pc);
    (*pc)++;
}

void cb2(void * arg)
{
    volatile int * pc = (int *)arg;
    printf("Hallo cb2: %d\n", *pc);
    (*pc)++;
}

int main(int argc, char ** argv)
{
    TCrossTimer * t1,*t2 = NULL;

    argc;
    argv;
    startupTimerSystem(1);

    printf("Hallo Anfang\n");

    t1 = timerCreate(2, 1, cb, &counter);
    t2 = timerCreate(1, 1, cb2, &counter);
    timerStart(t1);

    threadSleep(4500);

    timerStart(t2);

    threadSleep(3000);

    threadSleep(1500);
    timerDestroy(t1);

    printf("end of Timer t1\n");

    threadSleep(4000);
    shutdownTimerSystem();
    printf("after shutdown\n");


    return 0;
}
