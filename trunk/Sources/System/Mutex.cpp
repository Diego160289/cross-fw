//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
