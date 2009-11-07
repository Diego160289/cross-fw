#ifndef __UIIMPL_H__
#define __UIIMPL_H__


#include "OGLUI/UI.h"


namespace OGLUI
{

  class ClientContext::ClientContextImpl
    : private Common::NoCopyable
  {
  public:
    ClientContextImpl(System::WindowHandle wnd, const Rect &startRect);
    ~ClientContextImpl();
  private:
    System::WindowHandle Wnd;
    Rect WndRect;
  };

}

#endif  // !__UIIMPL_H__
