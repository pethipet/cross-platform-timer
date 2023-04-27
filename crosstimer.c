
#include "crosstimer.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(CROSSTIMER_WINDOWS)
#include <windows.h>
#elif defined(CROSSTIMER_POSIX)
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#else
#error "PLATFORM NOT SUPPORTED"
#endif

#ifndef MAX_CROSSTIMER
/** Maximum number of timer usable. Default to 5, not dynamic */
#define MAX_CROSSTIMER 5
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        ECT_DISABLED = 0, /**< Initial state - not active */
        ECT_ENABLED = 1,  /**< ? */
        ECT_STOPPED = 1,  /**< STOPPED --> STARTED */
        ECT_STARTED = 2   /**<  STARTED --> STOPPED */
    } ECrossTimerState;

#define CT_INIT_COMMON(p)                                                                                              \
    {                                                                                                                  \
        (p)->state = ECT_DISABLED;                                                                                     \
        (p)->startDelay = (p)->periodicInterval = 0;                                                                   \
        (p)->cb = NULL;                                                                                                \
        (p)->arg = NULL;

    struct TCrossTimer
    {
        ECrossTimerState state;
        TTimeSec startDelay;
        TTimeSec periodicInterval;
        TTimerCallback cb;
        void *arg;
#if defined(CROSSTIMER_WINDOWS)
        HANDLE hTimer;
#elif defined(CROSSTIMER_POSIX)
        timer_t timerId;
#else
#error "PLATFORM NOT SUPPORTED"
#endif
    };

#if defined(CROSSTIMER_WINDOWS)

#define CT_INIT(p)                                                                                                     \
    CT_INIT_COMMON(p)                                                                                                  \
    (p)->hTimer = NULL;                                                                                                \
    }

    static HANDLE g_hTimerQueue = NULL;

#elif defined(CROSSTIMER_POSIX)

#define CT_INIT(p)                                                                                                     \
    CT_INIT_COMMON(p)                                                                                                  \
    (p)->timerId = 0;                                                                                                  \
    }

#else
#error "PLATFORM NOT SUPPORTED"
#endif

    static TCrossTimer gTimerSlots[MAX_CROSSTIMER];

    int startupTimerSystem(int maxAmount /* UNUSED */)
    {
#if defined(CROSSTIMER_WINDOWS)
        if (!g_hTimerQueue)
            return 0;
        for (int i = 0; i < MAX_CROSSTIMER; i++)
        {
            CT_INIT(gTimerSlots + i)
        }
        g_hTimerQueue = CreateTimerQueue();
        if (NULL == g_hTimerQueue)
        {
            return EXIT_FAILURE; /* timer queue not created */
        }
#elif (CROSSTIMER_POSIX)

#else
#error "PLATFORM NOT SUPPORTED"
#endif
        return EXIT_SUCCESS;
    }

    int shutdownTimerSystem()
    {

        for (int i = 0; i < MAX_CROSSTIMER; i++)
        {
            if (ECT_STARTED == gTimerSlots[i].state)
            {
                timerDestroy((gTimerSlots + i));
            }
#if defined(CROSSTIMER_WINDOWS)
            if (g_hTimerQueue && !DeleteTimerQueueEx(g_hTimerQueue, INVALID_HANDLE_VALUE))
            {

                fprintf(stderr, "DeleteTimerQueue failed (%lu)\n", GetLastError());
                return EXIT_FAILURE; /* timer queue kill failed */
            }
#endif
        }
        return EXIT_SUCCESS;
    }

#if defined(CROSSTIMER_WINDOWS)

    VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN notRelevant)
    {
        TCrossTimer *p = (TCrossTimer *)lpParameter;
        if (p && p->cb)
        {
            p->cb(p->arg);
        }
        return;
    }

#elif defined(CROSSTIMER_POSIX)

static void threadedHandler(union sigval sv)
{
    /* TODO: Check if pointer is in range ... */
    TCrossTimer *p = (TCrossTimer *)sv.sival_ptr;
    if (p && p->cb)
    {
        p->cb(p->arg);
    }
    return;
}

#else
#error "PLATFORM NOT SUPPORTED"
#endif

    TCrossTimer *timerCreate(TTimeSec startDelay, TTimeSec periodic, TTimerCallback cb, void *cbArg)
    {

        TCrossTimer *pRet = NULL;

        for (int i = 0; i < MAX_CROSSTIMER; i++)
        {
            if (gTimerSlots[i].state == ECT_DISABLED)
            {
                pRet = &gTimerSlots[i];
                break;
            }
        } /* for */
        if (pRet)
        {
            pRet->state = ECT_ENABLED;
            pRet->startDelay = startDelay;
            pRet->periodicInterval = periodic;
            pRet->arg = cbArg;
            pRet->cb = cb;

#if defined(CROSSTIMER_WINDOWS)

#elif defined(CROSSTIMER_POSIX)
        struct sigevent sev;

        sev.sigev_notify = SIGEV_THREAD;
        sev.sigev_signo = 0;
        sev.sigev_value.sival_ptr = pRet; /* pass our timer structure */
        sev.sigev_notify_function = threadedHandler;
        sev.sigev_notify_attributes = NULL;

        if (timer_create(CLOCK_MONOTONIC, &sev, &(pRet->timerId)))
        {
            /* errro if != 0 */
            /* ... */
            fprintf(stderr, "timer_create failed errno (%d)\n", errno);
            return NULL;
        }
        fprintf(stderr, "timer_create: timeId %p\n", pRet->timerId);
#endif
            return pRet;
        }
        return NULL;
    }

    /**
     * @brief Destroy/Disable a timer
     * @param[in] timer to be freed.
     */
    int timerDestroy(TCrossTimer *pT)
    {
        TCrossTimer *p = gTimerSlots;

        for (; p < gTimerSlots + MAX_CROSSTIMER - 1; p++)
        {
            if (p == pT)
            {
                if (ECT_STARTED == p->state)
                {
                    if (!timerStop(p))
                    {
                        CT_INIT(p); /* ECT_DISABLED */
                    }
                }
                else if (ECT_STOPPED == p->state)
                {
                    CT_INIT(p); /* ECT_DISABLED */
                }
                else
                {
                    return EXIT_FAILURE;
                }
                return EXIT_SUCCESS; /* ok */
            }
        }
        return 2; /* EXIT needs to bbe defined */
    }

    int timerStart(TCrossTimer *pT)
    {
        if (pT)
        {
#if defined(CROSSTIMER_WINDOWS)
            pT->state = ECT_STARTED;
            if (!CreateTimerQueueTimer(&(pT->hTimer), g_hTimerQueue, (WAITORTIMERCALLBACK)WaitOrTimerCallback, pT,
                                       pT->startDelay * 1000, pT->periodicInterval * 1000, 0))
            {
                printf("CreateTimerQueueTimer failed (%lu)\n", GetLastError());
                CT_INIT(pT); /* ECT_DISABLED */
                return EXIT_FAILURE;
            }
#elif defined(CROSSTIMER_POSIX)
        struct itimerspec its = {.it_interval = {.tv_sec = pT->periodicInterval, .tv_nsec = 0l},
                                 .it_value = {.tv_sec = pT->startDelay, .tv_nsec = 0l}};

        pT->state = ECT_STARTED;

        /* relative time*/
        if (timer_settime(pT->timerId, 0, &its, NULL))
        {
            CT_INIT(pT); /* ECT_DISABLED */
            return EXIT_FAILURE;
            /* errno(); */
        }
#else
#error "PLATFORM NOT SUPPORTED"
#endif
            return EXIT_SUCCESS;
        }
        return 2; /* TBD: defined EXIT error code > EXIT_ERROR_BASE */
    }

    int timerStop(TCrossTimer *pT)
    {
#if defined(CROSSTIMER_WINDOWS)
        if (pT && (pT->state == ECT_STARTED) && pT->hTimer)
        {
            if (!DeleteTimerQueueTimer(g_hTimerQueue, pT->hTimer, 0))
            {
                printf("DeleteTimerQueueTimer failed (%lu)\n", GetLastError());
                CT_INIT(pT);         /* ECT_DISABLED */
                return EXIT_FAILURE; /* DeleteTimerQueueTimer failed */
            }
            pT->state = ECT_STOPPED;
            return EXIT_SUCCESS; /* ok */
        }
#elif defined(CROSSTIMER_POSIX)
    if (pT && (pT->state == ECT_STARTED) && pT->timerId)
    {
        static struct itimerspec stop = {{0, 0}, {0, 0}};

        /* relative time*/
        if (timer_settime(pT->timerId, 0, &stop, NULL))
        {
            CT_INIT(pT); /* ECT_DISABLED */
            return EXIT_FAILURE;
            /* errno(); */
        }
        pT->state = ECT_STOPPED;
        return EXIT_SUCCESS; /* ok */
    }
#else
#error "PLATFORM NOT SUPPORTED"
#endif
        return 2; /* wrong timer state */
    }

#ifdef __cplusplus
}
#endif
