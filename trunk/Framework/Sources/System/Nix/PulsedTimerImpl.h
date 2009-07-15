#ifndef __PULSEDTIMERIMPL_H__
#define __PULSEDTIMERIMPL_H__

#include "PulsedTimer.h"


namespace System
{

  class PulsedTimer::PulsedTimerImpl
    : private Common::NoCopyable
  {
  public:
    PulsedTimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter);
    ~PulsedTimerImpl();
  private:
  };

}

#endif  // !__PULSEDTIMERIMPL_H__
