#ifndef __FRAMEIMPL_H__
#define __FRAMEIMPL_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"
#include "../../../../Framework/Include/Thread.h"
#include "../../../../Framework/Include/ManualEvent.h"

#include "SysDisplays.h"

DECLARE_RUNTIME_EXCEPTION(FrameImpl)

class FrameImpl
  : Common::NoCopyable
{
public:
  FrameImpl(Common::SharedPtr<SysDisplays::SysDisplay> display);
  ~FrameImpl();

  void Show(bool isVisible);

private:
  static const char FrameClassName[];
  static LRESULT CALLBACK FrameProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

  class WndClassHolder;
  friend class WndClassHolder;

  Common::SharedPtr<SysDisplays::SysDisplay> Display;
  HWND Wnd;
  volatile bool IsCreated;
  System::ManualEvent WndEvent;
  Common::SharedPtr<System::Thread> WndThread;
  void WndThreadFunc();
};

#endif // !__FRAMEIMPL_H__
