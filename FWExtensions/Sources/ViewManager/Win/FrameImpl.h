#ifndef __FRAMEIMPL_H__
#define __FRAMEIMPL_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"

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
  HWND Wnd;
};

#endif // !__FRAMEIMPL_H__
