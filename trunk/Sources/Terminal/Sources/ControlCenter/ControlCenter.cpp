#include "ControlCenter.h"


IControlCenterImpl::IControlCenterImpl()
{
}

bool IControlCenterImpl::FinalizeCreate()
{
  return true;
}

void IControlCenterImpl::BeforeDestroy()
{
}

bool IControlCenterImpl::OnInit()
{
  try
  {
    ViewManager = CreateObject<IFaces::IViewManager>("dc5597fe-e0ed-4f60-a288-7771b947274c");
    if (!ViewManager->GetDisplayCount())
      return false;
    Common::RefObjPtr<IFaces::IDisplay> Display;
    if (ViewManager->GetDisplay(0, Display.GetPPtr()) != retOk)
      return false;
    Common::RefObjPtr<IFaces::IViewFrame> Frame;
    if (Display->CreateFrame(Frame.GetPPtr()) != retOk)
      return false;
    unsigned WndIndex = 0;
    if (Frame->CreateWnd(&WndIndex, Common::RefObjQIPtr<IFaces::IWndMessageHandler>(GetThisIBase()).Get()) != retOk)
      return false;
    if (Frame->SetCurWnd(WndIndex) != retOk|| Frame->Show(true) != retOk)
      return false;
    Common::RefObjPtr<IFaces::IView> CurView;
    if (Frame->GetView(WndIndex, CurView.GetPPtr()) != retOk)
      return false;
    System::WindowHandle HWnd;
    int X = 0, Y = 0, Width = 0, Height = 0;
    if (CurView->GetHandle(&HWnd) != retOk ||
      CurView->GetPos(&X, &Y) != retOk ||
      CurView->GetSize(&Width, &Height) != retOk)
    {
      return false;
    }
    ClientContext = new OGLUI::ClientContext(HWnd,
      OGLUI::Rect(X, Y, Width, Height));
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IControlCenterImpl::OnDone()
{
  ClientContext.Release();
  ViewManager.Release();
}

#include <windows.h>
#include "../../Include/Win/WindowMessage.h"

long IControlCenterImpl::OnMessage(const IFaces::WindowMessage &msg)
{
  if (msg.Msg == WM_DESTROY)
    GetServiceManager()->PostStopToServiceManager();
  return 0;
}
