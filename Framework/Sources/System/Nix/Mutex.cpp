#include <pthread.h>

#include "Mutex.h"

namespace System
{
  class Mutex::MutexImpl
    : private Common::NoCopyable
  {
  public:
    MutexImpl()
    {
      if (pthread_mutex_init(&MutexHandle, 0))
        throw MutexException("Can't create mutex");
    }
    ~MutexImpl()
    {
      pthread_mutex_destroy(&MutexHandle);
    }
    void Lock()
    {
      if (pthread_mutex_lock(&MutexHandle))
        throw MutexException("Can't lock mutex");
    }
    void Unlock()
    {
      if (pthread_mutex_unlock(&MutexHandle))
        throw MutexException("Can't unlock mutex");
    }
  private:
    pthread_mutex_t MutexHandle;
  };

  Mutex::Mutex() :
    Impl(new MutexImpl)
  {
  }

  Mutex::~Mutex()
  {
    delete Impl;
  }

  void Mutex::Lock()
  {
    Impl->Lock();
  }

  void Mutex::Unlock()
  {
    Impl->Unlock();
  }
}
