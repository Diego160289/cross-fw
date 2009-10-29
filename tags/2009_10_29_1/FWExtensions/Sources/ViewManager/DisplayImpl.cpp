#include "DisplayImpl.h"


IDisplayImpl::IDisplayImpl()
{
}

void IDisplayImpl::SetDisplay(SysDisplayPtr display)
{
  Display = display;
}

bool IDisplayImpl::FinalizeCreate()
{
  return true;
}

void IDisplayImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  if (Frame.Get())
    Frame.Release();
}

bool IDisplayImpl::ExistsFrame() const
{
  Common::ISyncObject Locker(GetSynObj());
  return !!Frame.Get();
}

RetCode IDisplayImpl::CreateFrame(IFaces::IViewFrame **frame)
{
  if (!Display.Get())
    return retFail;
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    Common::RefObjPtr<IViewFrameImpl> NewFrame = Common::IBaseImpl<IViewFrameImpl>::CreateWithSyn(GetSynObj());
    NewFrame->Create(Display);
    Frame = NewFrame;
    return Frame.QueryInterface(frame);
  }
  catch (std::exception &)
  {
  }
  return retFail;
}

RetCode IDisplayImpl::GetFrame(IFaces::IViewFrame **frame)
{
  Common::ISyncObject Locker(GetSynObj());
  return !Frame.Get() ? retFail : Frame.QueryInterface(frame);
}
