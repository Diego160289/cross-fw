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
