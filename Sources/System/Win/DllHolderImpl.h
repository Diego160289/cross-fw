//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DLLHOLDERIMPL_H__
#define __DLLHOLDERIMPL_H__

#include <windows.h>

#include "DllHolder.h"


namespace System
{

  class DllHolder::DllHolderImpl
    : NoCopyable
  {
  public:
    DllHolderImpl(const char *libName);
    ~DllHolderImpl();
    void* GetProc(const char *procName);
  private:
    HINSTANCE Dll;
  };
}

#endif  // !__DLLHOLDERIMPL_H__
