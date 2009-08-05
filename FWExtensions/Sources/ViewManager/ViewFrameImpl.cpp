#include "ViewFrameImpl.h"


IViewFrameImpl::IViewFrameImpl()
{
}

bool IViewFrameImpl::FinalizeCreate()
{
  return true;
}

void IViewFrameImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  if (Frame.Get())
    Frame->Destroy();
  Frame.Release();
}

void IViewFrameImpl::Create(Common::SharedPtr<SysDisplays::SysDisplay> display)
{
  Common::ISyncObject Locker(GetSynObj());
  if (Frame.Get())
    return;
  Common::SharedPtr<FrameImpl> NewFrame(new FrameImpl);
  NewFrame->Create(display);
  Frame = NewFrame;
}

RetCode IViewFrameImpl::Show(bool isVisible)
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Frame.Get())
    return retFail;
  Frame->Show(isVisible);
  return retOk;
}

unsigned IViewFrameImpl::GetWndCount() const
{
  Common::ISyncObject Locker(GetSynObj());
  return 0;
}

RetCode IViewFrameImpl::CreateWnd(unsigned *index)
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Frame.Get())
    return retFail;
  return Frame->CreateWnd(index) ? retOk : retFail;
}

RetCode IViewFrameImpl::DestroyWnd(unsigned index)
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Frame.Get())
    return retFail;
  return Frame->DestroyWnd(index) ? retOk : retFail;
}

RetCode IViewFrameImpl::GetCurWndIndex(unsigned *index) const
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Frame.Get())
    return retFail;
  return Frame->GetCurWndIndex(index) ? retOk : retFail;
}

void* IViewFrameImpl::GetCurWnd()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Frame.Get())
    return 0;
  return Frame->GetCurWnd();
}

RetCode IViewFrameImpl::SetCurWnd(unsigned index)
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Frame.Get())
    return retFail;
  return Frame->SetCurWnd(index) ? retOk : retFail;
}
