#include "DllHolder.h"
#include "DllHolderImpl.h"

namespace System
{
  DllHolder::DllHolder(const char *libName)
    : Dll(new DllHolderImpl(libName))
  {
  }

  DllHolder::~DllHolder()
  {
    delete Dll;
  }

  void* DllHolder::InternalGetProc(const char *procNmae)
  {
    return Dll->GetProc(procNmae);
  }
}
