#include "UIImpl.h"

namespace OGLUI
{

  ClientContext::ClientContextImpl::ClientContextImpl(System::WindowHandle wnd, const Rect &startRect)
    : Wnd(wnd)
    , WndRect(startRect)
  {
  }

  ClientContext::ClientContextImpl::~ClientContextImpl()
  {
  }

}
