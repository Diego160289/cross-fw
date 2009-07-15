#ifndef __PULSEDTIMERIMPL_H__
#define __PULSEDTIMERIMPL_H__

#include "PulsedTimer.h"

#include <sys/time.h>
#include <sys/signal.h>
#include <pthread.h>


namespace System
{

  class PulsedTimer::PulsedTimerImpl
    : private Common::NoCopyable
  {
  public:
    PulsedTimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter);
    ~PulsedTimerImpl();
  private:
    sigevent SigEvent;
    timer_t TimerId;
    itimerspec Value;
    static void TimerFunc(union sigval val);
  };

}

#endif  // !__PULSEDTIMERIMPL_H__
