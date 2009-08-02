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
}

unsigned IViewFrameImpl::GetWndCount() const
{
  return 0;
}

RetCode IViewFrameImpl::CreateWnd(unsigned *index)
{
  return retFail;
}

RetCode IViewFrameImpl::DestroyWnd(unsigned index)
{
  return retFail;
}

RetCode IViewFrameImpl::GetCurWndIndex(unsigned *index) const
{
  return retFail;
}

void* IViewFrameImpl::GetCurWndHandle()
{
  return 0;
}

RetCode IViewFrameImpl::SetCurWnd(unsigned index)
{
  return retFail;
}
