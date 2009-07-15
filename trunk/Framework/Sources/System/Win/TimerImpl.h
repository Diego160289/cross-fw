#ifndef __TimerIMPL_H__
#define __TimerIMPL_H__

#include "Timer.h"

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
#endif

#include <windows.h>


namespace System
{

  class Timer::TimerImpl
    : private Common::NoCopyable
  {
  public:
    TimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter);
    ~TimerImpl();
  private:
    HANDLE TimerQueue_;
    HANDLE Timer_;
    static VOID CALLBACK TimerFunc(PVOID param, BOOLEAN);
  };

}

#endif  // !__TimerIMPL_H__
