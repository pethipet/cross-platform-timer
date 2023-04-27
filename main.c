#include "crosstimer.h"

#include <stdio.h>

#if defined(_WIN32)
#    include <windows.h>
#else
#include <unistd.h>
#endif

void threadSleep(unsigned long s)
{
#if defined(_WIN32)
    Sleep(s);
#else
    sleep(s / 1000);
#endif
}

int counter = 0;

void cb(void *arg)
{
    volatile int *pc = (int *)arg;
    printf("Hallo cb: %d\n", *pc);
    (*pc)++;
}

void cb2(void *arg)
{
    volatile int *pc = (int *)arg;
    printf("Hallo cb2: %d\n", *pc);
    (*pc)++;
}


#include <stdlib.h>

int main(int argc, char ** argv)
{
    TCrossTimer * t1,*t2 = NULL;

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
