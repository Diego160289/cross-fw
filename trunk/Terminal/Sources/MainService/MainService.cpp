#include "MainService.h"


IMainServiceImpl::IMainServiceImpl()
  : FlashServiceHandle(0)
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
    Common::RefObjPtr<IFaces::IBase> FlashCtrl;
    if (!(FlashServiceHandle = GetServiceManager()->StartService("83ef6af0-e752-4601-87fe-90b8be229e1f", FlashCtrl.GetPPtr())))
    {
      return false;
    }
    Common::RefObjQIPtr<IFaces::IFlashView> FlashView(FlashCtrl);
    if (!FlashView.Get())
    {
      return false;
    }
    Common::RefObjQIPtr<IFaces::IWndMessageHandler> FlashMsgHandler(FlashCtrl);
    if (!FlashMsgHandler.Get())
    {
      return false;
    }
    unsigned WindowIndex = 0;
    if (Frame->CreateWnd(&WindowIndex, FlashMsgHandler.Get()) != retOk)
    {
      return false;
    }
    if (Frame->SetCurWnd(WindowIndex) != retOk)
    {
      return false;
    }
    Frame->Show(true);
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
  if (FlashServiceHandle)
    GetServiceManager()->StopService(FlashServiceHandle);
  ViewManager.Release();
}
