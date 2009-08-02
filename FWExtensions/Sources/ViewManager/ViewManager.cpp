#include "ViewManager.h"


IViewManagerImpl::IViewManagerImpl()
{
}

bool IViewManagerImpl::FinalizeCreate()
{
  Displays = new SysDisplays;
  return true;
}

void IViewManagerImpl::BeforeDestroy()
{
  Displays.Release();
}

unsigned IViewManagerImpl::GetDisplayCount() const
{
  Common::ISyncObject Locker(GetSynObj());
  return static_cast<unsigned>(Displays->GetDisplays().size());
}

RetCode IViewManagerImpl::GetDisplay(unsigned index, IFaces::IDisplay **display)
{
  return retFail;
}
