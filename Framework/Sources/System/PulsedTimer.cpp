#include "PulsedTimer.h"
#include "PulsedTimerImpl.h"

namespace System
{

  PulsedTimer::PulsedTimer(Common::ICallbackPtr callback, unsigned period, unsigned startAfter)
    : Callback(callback), Impl(new PulsedTimerImpl(Callback.Get(), period, startAfter))
  {
  }

  PulsedTimer::~PulsedTimer()
  {
    delete Impl;
  }

}
