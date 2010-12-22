//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __TIMERIMPL_H__
#define __TIMERIMPL_H__

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
    TimerImpl(Common::ICallback *callback, unsigned period);
    ~TimerImpl();
  private:
    HANDLE TimerQueue_;
    HANDLE Timer_;
    static VOID CALLBACK TimerFunc(PVOID param, BOOLEAN);
  };

}

#endif  // !__TIMERIMPL_H__
