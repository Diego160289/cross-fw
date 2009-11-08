#include "OGLUI/UI.h"
#include "UIImpl.h"


namespace OGLUI
{

  ClientContext::ClientContext(System::WindowHandle wnd, const Rect &startRect)
    : Impl(new ClientContextImpl(wnd, startRect))
  {
  }

  ClientContext::~ClientContext()
  {
    delete Impl;
  }

  void ClientContext::Draw()
  {
    Impl->Draw();
  }
}