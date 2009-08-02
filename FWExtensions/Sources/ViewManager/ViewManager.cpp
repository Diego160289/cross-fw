#include "ViewManager.h"


IViewManagerImpl::IViewManagerImpl()
{
}

bool IViewManagerImpl::FinalizeCreate()
{
  return true;
}

void IViewManagerImpl::BeforeDestroy()
{
}

unsigned IViewManagerImpl::GetDisplayCount() const
{
  return 0;
}

RetCode IViewManagerImpl::GetDisplay(unsigned index, IFaces::IDisplay **display)
{
  return retFail;
}
