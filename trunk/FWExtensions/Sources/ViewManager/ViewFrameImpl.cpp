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
  Frame.Release();
}

void IViewFrameImpl::Create(Common::SharedPtr<SysDisplays::SysDisplay> display)
{
  Common::ISyncObject Locker(GetSynObj());
  if (Frame.Get())
    return;
  Frame = new FrameImpl(display);
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
  return retFail;
}

RetCode IViewFrameImpl::DestroyWnd(unsigned index)
{
  Common::ISyncObject Locker(GetSynObj());
  return retFail;
}

RetCode IViewFrameImpl::GetCurWndIndex(unsigned *index) const
{
  Common::ISyncObject Locker(GetSynObj());
  return retFail;
}

void* IViewFrameImpl::GetCurWndHandle()
{
  Common::ISyncObject Locker(GetSynObj());
  return 0;
}

RetCode IViewFrameImpl::SetCurWnd(unsigned index)
{
  Common::ISyncObject Locker(GetSynObj());
  return retFail;
}
