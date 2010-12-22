//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __OSTYPEDEFS_H__
#define __OSTYPEDEFS_H__


namespace System
{
  class WindowHandle
  {
  public:
    WindowHandle()
      : Handle(0)
    {
    }
    template <typename T>
    WindowHandle(T handle)
      : Handle(reinterpret_cast<void *>(handle))
    {
    }
    template <typename T>
    operator T ()
    {
      return reinterpret_cast<T>(Handle);
    }
  private:
    void *Handle;
  };
}

#endif  // !__OSTYPEDEFS_H__
