#include "DllHolderImpl.h"

#include <exception>

namespace System
{
  DllHolder::DllHolderImpl::DllHolderImpl(const char *libName)
    : Dll(::LoadLibraryA(libName))
  {
    if (!Dll)
      throw DllHolderException("Can't load dll");
  }

  DllHolder::DllHolderImpl::~DllHolderImpl()
  {
    ::FreeLibrary(Dll);
  }

  void* DllHolder::DllHolderImpl::GetProc(const char *procName)
  {
    void *Ret = (void*)(::GetProcAddress(Dll, procName));
    if (!Ret)
      throw DllHolderException("Function not found");
    return Ret;
  }
}
