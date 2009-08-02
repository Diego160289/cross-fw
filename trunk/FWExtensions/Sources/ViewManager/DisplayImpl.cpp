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
}

bool IDisplayImpl::ExistsFrame() const
{
  return false;
}

RetCode IDisplayImpl::CreateFrame(IFaces::IViewFrame **frame)
{
  return retFail;
}

RetCode IDisplayImpl::GetFrame(IFaces::IViewFrame **frame)
{
  return retFail;
}
