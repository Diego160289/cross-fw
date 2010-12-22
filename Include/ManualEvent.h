//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __MANUALEVENT_H__
#define __MANUALEVENT_H__

#include "NoCopyable.h"
#include "Exceptions.h"


namespace System
{

  DECLARE_RUNTIME_EXCEPTION(ManualEvent)

  class ManualEvent
    : private Common::NoCopyable
  {
  public:
    ManualEvent();
    ~ManualEvent();
    void Set();
    void Reset();
    bool Wait();
  private:
    class ManualEventImpl;
    ManualEventImpl *Impl;
  };

}

#endif  // !__MANUALEVENT_H__
