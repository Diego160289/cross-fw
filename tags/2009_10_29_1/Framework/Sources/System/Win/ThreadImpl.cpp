#include "ThreadImpl.h"

namespace System
{
  Thread::ThreadImpl::ThreadImpl(Common::ICallback *callback)
    : ThreadHandle(0)
  {
    DWORD Id = 0;
    if (!(ThreadHandle = ::CreateThread(0, 0,
        &ThreadImpl::ThreadProc, callback, 0, &Id)))
    {
      throw ThreadException("Can't create thread");
    }
  }

  Thread::ThreadImpl::~ThreadImpl()
  {
    ::WaitForSingleObject(ThreadHandle, INFINITE);
    ::CloseHandle(ThreadHandle);
  }

  DWORD WINAPI Thread::ThreadImpl::ThreadProc(LPVOID prm)
  {
    reinterpret_cast<Common::ICallback *>(prm)->Do();
    return 0;
  }

}
