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
    pthread_mutex_t MutexHandle;
  };
}

#endif  // !__MUTEXIMPL_H__
