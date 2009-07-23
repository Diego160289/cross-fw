#include "ManualEventImpl.h"

namespace System
{

  ManualEvent::ManualEventImpl::ManualEventImpl()
    : EventHandle(::CreateEventA(0, TRUE, TRUE, 0))
  {
    if (!EventHandle)
      throw ManualEventException("Can't create manual event");
  }

  ManualEvent::ManualEventImpl::~ManualEventImpl()
  {
    ::CloseHandle(EventHandle);
  }

  void ManualEvent::ManualEventImpl::Set()
  {
    if (!::SetEvent(EventHandle))
      throw ManualEventException("Can't set event");
  }

  void ManualEvent::ManualEventImpl::Reset()
  {
    if (!::ResetEvent(EventHandle))
      throw ManualEventException("Can't reset event");
  }

  bool ManualEvent::ManualEventImpl::Wait()
  {
    DWORD Res = ::WaitForSingleObject(EventHandle, INFINITE);
    if (Res == WAIT_FAILED)
      throw ManualEventException("Error wait event");
    return Res == WAIT_OBJECT_0;
  }

}
