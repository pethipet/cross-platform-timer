#ifndef CROSSTIMER_H
#define CROSSTIMER_H

#if defined(_WIN32) || defined(WIN32)
/* Windows Style */
#define CROSSTIMER_WINDOWS 1
#elif !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
/* UNIX-style OS. */
/* request definitions for timer API */
#define _POSIX_C_SOURCE 199309L
#define CROSSTIMER_POSIX 1
#else
#error "PLATFORM NOT SUPPORTED"
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief The timer callback type
     * @param[in] arg A callback argument
     */
    typedef void (*TTimerCallback)(void *arg);

    typedef struct TCrossTimer TCrossTimer;

    typedef long TTimeSec;

    /**
     * @brief Initialize TimerSubSystem
     * @return 0 ok, != 0 nok
     */
    int startupTimerSystem(int);
    /**
     * @brief Shutdown TimerSubSystem, cancel timer
     * @return 0 ok, != 0 nok
     */
    int shutdownTimerSystem();

    /**
     * @brief Create a new timer
     * @param[in] startDelay in secs. > 0 timer will call cb after startDelay
     * @param[in] period in secs. > 0 Timer will periodicly call cb, First after
     * startDelay + periodic.
     * @param[in] cb The callback called when timeout expires,
     * @param[in] cbArg The argument to pass to callback
     * @return Zero on errors, non-zero otherwise
     */
    TCrossTimer *timerCreate(TTimeSec startDelay, TTimeSec periodic, TTimerCallback cb, void *cbArg);

    /**
     * @brief Free a timer
     * @param[in] timer to be freed.
     */
    int timerDestroy(TCrossTimer *pT);

    int timerStart(TCrossTimer *pT);
    int timerStop(TCrossTimer *pT);

#ifdef __cplusplus
}
#endif

#endif /* CROSSTIMER_H */
