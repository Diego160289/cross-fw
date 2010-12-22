//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __MUTEXIMPL_H__
#define __MUTEXIMPL_H__

#include <pthread.h>

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
    pthread_mutexattr_t MutexAttr;
    pthread_mutex_t MutexHandle;
  };
}

#endif  // !__MUTEXIMPL_H__
