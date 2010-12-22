//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DllHolderImpl.h"

#include <exception>


namespace System
{
  DllHolder::DllHolderImpl::DllHolderImpl(const char *libName)
    : Dll(dlopen(libName, RTLD_LAZY))
  {
    if (!Dll)
      throw DllHolderException("Can't load dll");
  }

  DllHolder::DllHolderImpl::~DllHolderImpl()
  {
    dlclose(Dll);
  }

  void* DllHolder::DllHolderImpl::GetProc(const char *procName)
  {
    void *Ret = dlsym(Dll, procName);
    if (!Ret)
      throw DllHolderException("Function not found");
    return Ret;
  }
}
