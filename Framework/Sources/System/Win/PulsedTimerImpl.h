#ifndef __PULSEDTIMERIMPL_H__
#define __PULSEDTIMERIMPL_H__

#include "PulsedTimer.h"

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
#endif

#include <windows.h>


namespace System
{

  class PulsedTimer::PulsedTimerImpl
    : private Common::NoCopyable
  {
  public:
    PulsedTimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter);
    ~PulsedTimerImpl();
  private:
    HANDLE TimerQueue_;
    HANDLE Timer_;
    static VOID CALLBACK TimerFunc(PVOID param, BOOLEAN);
  };

}

#endif  // !__PULSEDTIMERIMPL_H__
