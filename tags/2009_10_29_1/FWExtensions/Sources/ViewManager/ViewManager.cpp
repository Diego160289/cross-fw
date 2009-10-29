#include "ViewManager.h"


IViewManagerImpl::IViewManagerImpl()
{
}

bool IViewManagerImpl::FinalizeCreate()
{
  Common::ISyncObject Locker(GetSynObj());
  DisplayHolder = new SysDisplays;
  return true;
}

void IViewManagerImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  DisplayHolder.Release();
}

unsigned IViewManagerImpl::GetDisplayCount() const
{
  Common::ISyncObject Locker(GetSynObj());
  return static_cast<unsigned>(DisplayHolder->GetDisplays().size());
}

RetCode IViewManagerImpl::GetDisplay(unsigned index, IFaces::IDisplay **display)
{
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    DisplayPool::iterator Iter = Displays.find(index);
    if (Iter != Displays.end())
      return Iter->second.QueryInterface(display);
    if (index >= DisplayHolder->GetDisplays().size())
      return retFail;
    DisplayPtr NewDisplay = Common::IBaseImpl<IDisplayImpl>::CreateWithSyn(GetSynObj());
    NewDisplay->SetDisplay(IDisplayImpl::SysDisplayPtr(new SysDisplays::SysDisplay(DisplayHolder->GetDisplays()[index])));
    Displays[index] = NewDisplay;
    return NewDisplay.QueryInterface(display);
  }
  catch (std::exception &)
  {
  }
  return retFail;
}
