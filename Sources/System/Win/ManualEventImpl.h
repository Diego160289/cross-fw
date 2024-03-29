//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __MANUALEVENTIMPL_H__
#define __MANUALEVENTIMPL_H__

#include "ManualEvent.h"

#include <windows.h>


namespace System
{

  class ManualEvent::ManualEventImpl
    : private Common::NoCopyable
  {
  public:
    ManualEventImpl();
    ~ManualEventImpl();
    void Set();
    void Reset();
    bool Wait();
  private:
    HANDLE EventHandle;
  };

}

#endif  // !__MANUALEVENTIMPL_H__
