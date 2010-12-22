//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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

  void* DllHolder::InternalGetProc(const char *procName)
  {
    return Dll->GetProc(procName);
  }
}
