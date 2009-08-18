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

#include "../../Framework/Include/IStorageFileImpl.h"
#include "../../Framework/Include/IStreamFileImpl.h"
#include "../../Framework/Include/IStorageHelper.h"
#include "../../Framework/Include/IStreamHelper.h"

#include <conio.h>

bool IMainServiceImpl::OnInit()
{
  try
  {
    // TODO: убрать в компоненту источника данных. Ее пока нет!

    DataSrc = IFacesImpl::OpenFileStorage<System::MutexStub>("./FlashData", false);
    
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
    FlashView->SetViewCallback(Common::RefObjQIPtr<IFaces::IViewCallback>(GetThisIBase()).Get());
    FlashView->PlayMovie("test.swf");
  }
  catch (std::exception &)
  {
    return false;
  }
  /*MarkToDoneService();
  return true;*/
  _getch();
  MarkToDoneService();
  return true;
}

void IMainServiceImpl::OnDone()
{
  if (FlashServiceHandle)
    GetServiceManager()->StopService(FlashServiceHandle);
  ViewManager.Release();
}

RetCode IMainServiceImpl::QueryExternalResource(const char *resName, IFaces::IStream **resStream)
{
  try
  {
    Common::RefObjPtr<IFaces::IStream> Stream = IFacesImpl::OpenMemoryStream<System::MutexStub>();
    IFacesImpl::IStreamHelper(IFacesImpl::IStorageHelper(DataSrc).OpenStream(resName)).CopyTo(Stream);
    IFacesImpl::IStreamHelper Sh(Stream);
    return Stream.QueryInterface(resStream);
  }
  catch (std::exception &)
  {
  }
  return retFail;
}

void IMainServiceImpl::Execute(IFaces::IFunction *func)
{
  try
  {
    std::vector<std::string> Funcs;
    Funcs.push_back("GetAnimation");
    Funcs.push_back("GetBuildingsByProductId");
    Funcs.push_back("GetContent");
    Funcs.push_back("GetContentByGroupId");
    Funcs.push_back("GetGroups");
    Funcs.push_back("GetHallByNavigationId");
    Funcs.push_back("GetProductsDetails");
    Funcs.push_back("GetSchedule");
    Funcs.push_back("GetSegment");
    Funcs.push_back("SystemInitialize");


    std::string FunctionName = func->GetFunctionName();

    for (std::vector<std::string>::const_iterator i = Funcs.begin() ; i != Funcs.end() ; ++i)
    {
      if (FunctionName == *i)
      {
        std::string Path = "On" + (*i) + ".xml";
        Common::RefObjPtr<IFaces::IStream> Stream = IFacesImpl::OpenMemoryStream<System::MutexStub>();
        IFacesImpl::IStreamHelper(IFacesImpl::IStorageHelper(IFacesImpl::IStorageHelper(DataSrc).OpenStorage("XmlFuncs")).OpenStream(Path)).CopyTo(Stream);
        IFacesImpl::IStreamHelper Sh(Stream);
        Sh.SeekToEnd();
        Sh.Write("\0", 1);
        Sh.SeekToBegin();
        //Flash->CallFlash(AStringToWString((const char *)Common::RefObjQIPtr<IFaces::IRawDataBuffer>(Stream)->GetData(), true).c_str());
        break;
      }
    }
  }
  catch (std::exception &)
  {
  }
}
