#include "MainService.h"


IMainServiceImpl::IMainServiceImpl()
{
}

bool IMainServiceImpl::FinalizeCreate()
{
  return true;
}

void IMainServiceImpl::BeforeDestroy()
{
}

bool IMainServiceImpl::OnInit()
{
  try
  {
    ViewManager = CreateObject<IFaces::IViewManager>("dc5597fe-e0ed-4f60-a288-7771b947274c");
    if (!ViewManager->GetDisplayCount())
    {
      return false;
    }
    Common::RefObjPtr<IFaces::IDisplay> Display;
    if (ViewManager->GetDisplay(0, Display.GetPPtr()) != retOk)
    {
      return false;
    }
    Common::RefObjPtr<IFaces::IViewFrame> Frame;
    if (Display->CreateFrame(Frame.GetPPtr()) != retOk)
    {
      return false;
    }
    unsigned WindowIndex = 0;
    if (Frame->CreateWnd(&WindowIndex) != retOk)
    {
      return false;
    }
    if (Frame->SetCurWnd(WindowIndex) != retOk)
    {
      return false;
    }
  }
  catch (std::exception &)
  {
    return false;
  }
  MarkToDoneService();
  return true;
}

void IMainServiceImpl::OnDone()
{
  ViewManager.Release();
}
