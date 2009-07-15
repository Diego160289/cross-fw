#ifndef __PULSEDTIMERIMPL_H__
#define __PULSEDTIMERIMPL_H__

#include "Timer.h"

#include <sys/time.h>
#include <sys/signal.h>
#include <pthread.h>


namespace System
{

  class Timer::TimerImpl
    : private Common::NoCopyable
  {
  public:
    TimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter);
    ~TimerImpl();
  private:
    sigevent SigEvent;
    timer_t TimerId;
    itimerspec Value;
    static void TimerFunc(union sigval val);
  };

}

#endif  // !__PULSEDTIMERIMPL_H__
