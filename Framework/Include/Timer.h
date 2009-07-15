#ifndef __PULSEDTIMER_H__
#define __PULSEDTIMER_H__

#include "NoCopyable.h"
#include "Exceptions.h"
#include "SimpleIFaces.h"

namespace System
{

  DECLARE_RUNTIME_EXCEPTION(Timer)

  class Timer
    : private Common::NoCopyable
  {
  public:
    Timer(Common::ICallbackPtr callback, unsigned period, unsigned startAfter);
    ~Timer();
  private:
    Common::ICallbackPtr Callback;
    class TimerImpl;
    TimerImpl *Impl;
  };

}

#endif  // !__PULSEDTIMER_H__
