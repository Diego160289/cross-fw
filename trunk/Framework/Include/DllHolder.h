#ifndef __DLLHOLDER_H__
#define __DLLHOLDER_H__

#include "NoCopyable.h"
#include "Exceptions.h"

namespace System
{
  DECLARE_RUNTIME_EXCEPTION(DllHolder)

  class DllHolder
    : private Common::NoCopyable
  {
  public:
    DllHolder(const char *libName);
    ~DllHolder();
    template <typename T>
    T GetProc(const char *procNmae)
    {
      return (T)(InternalGetProc(procNmae));
    }
  private:
    class DllHolderImpl;
    DllHolderImpl *Dll;
    void* InternalGetProc(const char *procNmae);
  };
}

#endif  // !__DLLHOLDER_H__
