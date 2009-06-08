#ifndef __MUTEXIMPL_H__
#define __MUTEXIMPL_H__

#include <windows.h>

#include "Mutex.h"


namespace System
{
  class Mutex::MutexImpl
    : private Common::NoCopyable
  {
  public:
    MutexImpl();
    ~MutexImpl();
    void Lock();
    void Unlock();
  private:
    CRITICAL_SECTION Section;
  };
}

#endif  // !__MUTEXIMPL_H__
