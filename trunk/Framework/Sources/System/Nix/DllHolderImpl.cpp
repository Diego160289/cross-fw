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

  void* DllHolder::DllHolderImpl::GetProc(const char *procNmae)
  {
    void *Ret = dlsym(Dll, procNmae);
    if (!Ret)
      throw DllHolderException("Function not found");
    return Ret;
  }
}
