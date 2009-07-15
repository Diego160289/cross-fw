#ifndef __PULSEDTIMER_H__
#define __PULSEDTIMER_H__

#include "NoCopyable.h"
#include "Exceptions.h"
#include "SimpleIFaces.h"

namespace System
{

  DECLARE_RUNTIME_EXCEPTION(PulsedTimer)

  class PulsedTimer
    : private Common::NoCopyable
  {
  public:
    PulsedTimer(Common::ICallbackPtr callback, unsigned period, unsigned startAfter);
    ~PulsedTimer();
  private:
    Common::ICallbackPtr Callback;
    class PulsedTimerImpl;
    PulsedTimerImpl *Impl;
  };

}

#endif  // !__PULSEDTIMER_H__
