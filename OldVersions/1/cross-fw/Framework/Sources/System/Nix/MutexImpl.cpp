#include "Mutex.h"
#include "MutexImpl.h"

namespace System
{
  Mutex::MutexImpl::MutexImpl()
  {
    if (pthread_mutex_init(&MutexHandle, 0))
      throw MutexException("Can't create mutex");
  }

  Mutex::MutexImpl::~MutexImpl()
  {
    pthread_mutex_destroy(&MutexHandle);
  }

  void Mutex::MutexImpl::Lock()
  {
    if (pthread_mutex_lock(&MutexHandle))
      throw MutexException("Can't lock mutex");
  }

  void Mutex::MutexImpl::Unlock()
  {
    if (pthread_mutex_unlock(&MutexHandle))
      throw MutexException("Can't unlock mutex");
  }
}
