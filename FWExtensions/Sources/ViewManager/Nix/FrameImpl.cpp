#include "FrameImpl.h"

#include "WindowMessage.h"


FrameImpl::FrameImpl()
{
}

FrameImpl::~FrameImpl()
{
}

void FrameImpl::Show(bool isVisible)
{
}

void FrameImpl::Create(Common::SharedPtr<SysDisplays::SysDisplay> display)
{
  throw FrameImplException("Empty display pointer");
}

void FrameImpl::Destroy()
{
}

unsigned FrameImpl::GetWndCount() const
{
  return 0;
}

bool FrameImpl::CreateWnd(unsigned *index, IFaces::IWndMessageHandler *handler)
{
  return false;
}

bool FrameImpl::DestroyWnd(unsigned index)
{
  return false;
}

bool FrameImpl::GetCurWnd(unsigned *index) const
{
  return false;
}

bool FrameImpl::SetCurWnd(unsigned index)
{
  return false;
}
