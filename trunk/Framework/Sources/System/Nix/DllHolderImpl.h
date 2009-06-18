#ifndef __DLLHOLDERIMPL_H__
#define __DLLHOLDERIMPL_H__

#include <pthread.h>

#include "DllHolder.h"

namespace System
{

  class DllHolder::DllHolderImpl
    : NoCopyable
  {
  public:
    DllHolderImpl(const char *libName);
    ~DllHolderImpl();
    void* GetProc(const char *procNmae);
  private:
    void *DlInstance;
  };
}

#endif  // !__DLLHOLDERIMPL_H__
