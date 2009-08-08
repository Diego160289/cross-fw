#include "FlashView.h"


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
    (FlashCtrl = new FlashCtrlHolder)->Create();
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IFlashViewImpl::OnDone()
{
  FlashCtrl->Destroy();
  Common::ISyncObject Locker(GetSynObj());
  FlashCtrl.Release();
}

long IFlashViewImpl::OnMessage(const IFaces::WindowMessage &msg)
{
  Common::ISyncObject Locker(GetSynObj());
  return FlashCtrl.Get() ? FlashCtrl->OnMessage(msg) : 0;
}
