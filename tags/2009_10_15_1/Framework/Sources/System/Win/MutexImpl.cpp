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
