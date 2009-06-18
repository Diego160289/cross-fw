#include "DllHolderImpl.h"

#include <exception>

namespace System
{
  DllHolder::DllHolderImpl::DllHolderImpl(const char *libName)
    : Dll(::LoadLibrary(libName))
  {
    if (!Dll)
      throw DllHolderException("Can't load dll");
  }

  DllHolder::DllHolderImpl::~DllHolderImpl()
  {
    ::FreeLibrary(Dll);
  }

  void* DllHolder::DllHolderImpl::GetProc(const char *procNmae)
  {
    void *Ret = (void*)(::GetProcAddress(Dll, procNmae));
    if (!Ret)
      throw DllHolderException("Function not found");
    return Ret;
  }
}
