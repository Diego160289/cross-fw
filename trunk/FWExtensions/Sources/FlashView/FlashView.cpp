#include "FlashView.h"
#include "WindowMessage.h"


IFlashViewImpl::IFlashViewImpl()
{
}

bool IFlashViewImpl::FinalizeCreate()
{
  return true;
}

void IFlashViewImpl::BeforeDestroy()
{
}

bool IFlashViewImpl::OnInit()
{
  try
  {
    FlashCtrl = new FlashCtrlHolder;
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IFlashViewImpl::OnDone()
{
  FlashCtrl.Release();
}

bool IFlashViewImpl::OnMessage(const IFaces::WindowMessage &msg)
{
  if (msg.Msg == WM_CREATE)
  {
    int k = 0;
  }
  return false;
}
