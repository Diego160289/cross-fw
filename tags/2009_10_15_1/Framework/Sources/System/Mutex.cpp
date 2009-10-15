#include "Mutex.h"
#include "MutexImpl.h"

namespace System
{
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
