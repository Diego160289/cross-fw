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

RetCode IFlashViewImpl::AttachFrame(IFaces::IViewFrame *frame)
{
  return retFail;
}
