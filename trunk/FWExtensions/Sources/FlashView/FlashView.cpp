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
  Common::SharedPtr<Common::ISynObj> NewViewCbSynObj(GetSynObj().Clone());
  try
  {
    FlashCtrl = new FlashCtrlHolder;
  }
  catch (std::exception &)
  {
    return false;
  }
  ViewCbSynObj = NewViewCbSynObj;
  return true;
}

void IFlashViewImpl::OnDone()
{
  {
    FlashCtrl->Done();
    Common::ISyncObject Locker(GetSynObj());
    FlashCtrl.Release();
  }
  {
    Common::ISyncObject Locker(*ViewCbSynObj.Get());
    ViewCallback.Release();
  }
}

RetCode IFlashViewImpl::SetViewCallback(IFaces::IViewCallback *callback)
{
  {
    if (!ViewCbSynObj.Get())
      return retFail;
    Common::ISyncObject Locker(*ViewCbSynObj.Get());
    ViewCallback = callback;
  }
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    FlashCtrl->SetViewCallback(ViewCallback);
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

RetCode IFlashViewImpl::PlayMovie(const char *movieName)
{
  FlashCtrlHolderPtr Ctrl;
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!FlashCtrl.Get())
      return retFail;
    Ctrl = FlashCtrl;
  }
  try
  {
    Ctrl->PlayMovie(movieName);
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

long IFlashViewImpl::OnMessage(const IFaces::WindowMessage &msg)
{
  FlashCtrlHolderPtr Ctrl;
  {
    Common::ISyncObject Locker(GetSynObj());
    Ctrl = FlashCtrl;
  }
  return Ctrl.Get() ? Ctrl->OnMessage(msg) : 0;
}
