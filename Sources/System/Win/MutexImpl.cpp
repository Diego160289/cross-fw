//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "MutexImpl.h"


namespace System
{
  Mutex::MutexImpl::MutexImpl()
  {
    ::InitializeCriticalSection(&Section);
  }

  Mutex::MutexImpl::~MutexImpl()
  {
    ::DeleteCriticalSection(&Section);
  }

  void Mutex::MutexImpl::Lock()
  {
    ::EnterCriticalSection(&Section);
  }

  void Mutex::MutexImpl::Unlock()
  {
    ::LeaveCriticalSection(&Section);
  }
}
