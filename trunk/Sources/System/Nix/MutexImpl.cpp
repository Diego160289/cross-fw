//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Mutex.h"
#include "MutexImpl.h"


namespace System
{
  Mutex::MutexImpl::MutexImpl()
  {
    if (pthread_mutexattr_settype(&MutexAttr, PTHREAD_MUTEX_RECURSIVE_NP) ||
        pthread_mutex_init(&MutexHandle, &MutexAttr))
    {
      throw MutexException("Can't create mutex");
    }
    if (pthread_mutexattr_destroy(&MutexAttr))
    {
      pthread_mutex_destroy(&MutexHandle);
      throw MutexException("Can't create mutex");
    }
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
