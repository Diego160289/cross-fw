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
#include "../../Framework/Include/Xml/XmlTools.h"
#include "../../Framework/Include/IFunctionImpl.h"

#include <conio.h>
#include <iostream>

bool IMainServiceImpl::OnInit()
{
  try
  {
    Common::RefObjPtr<IFaces::IStorage> NewDataSrc = IFacesImpl::OpenFileStorage<System::MutexStub>("./FlashData", false);
    
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
    Common::RefObjQIPtr<IFaces::IFlashView> NewFlashView(FlashCtrl);
    if (!NewFlashView.Get())
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

    FlashView = NewFlashView;
    DataSrc = NewDataSrc;

    Frame->Show(true);
    NewFlashView->SetViewCallback(Common::RefObjQIPtr<IFaces::IViewCallback>(GetThisIBase()).Get());
    NewFlashView->PlayMovie("NewTest.swf");
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
  FlashView.Release();
  ViewManager.Release();
  DataSrc.Release();
}

RetCode IMainServiceImpl::QueryExternalResource(const char *resName, IFaces::IStream **resStream)
{
  try
  {
    Common::RefObjPtr<IFaces::IStream> Stream = IFacesImpl::OpenMemoryStream<System::MutexStub>();
    IFacesImpl::IStreamHelper(IFacesImpl::IStorageHelper(DataSrc).OpenStream(resName)).CopyTo(Stream);
    return Stream.QueryInterface(resStream);
  }
  catch (std::exception &)
  {
    int k = 0;
  }
  return retFail;
}

void IMainServiceImpl::Execute(IFaces::IFunction *func)
{
  try
  {
    {
      using namespace Common::XmlTools;
      // Function to node
      NodePtr Root = IFacesImpl::NodeFromFunction(Common::RefObjPtr<IFaces::IFunction>(func));
      // Get function name
      std::string FuncName = Root->GetPropertiesMap()["name"];
      // Get properties map
      NodeListPtr Props = Root->GetChildNodes()[Tag("arguments")][0]->GetChildNodes()[Tag("object")][0]->GetChildListByName("property");
      // Get callback name from properties map
      std::wstring CallbackProp = (*Props.Get())[Attribute("id", "Callback")][0]->GetChildNodes()[Tag("string")][0]->GetValue();
      // Get invoker name from properties map
      std::wstring InvokerProp = (*Props.Get())[Attribute("id", "Invoker")][0]->GetChildNodes()[Tag("string")][0]->GetValue();
    }

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
        Common::XmlTools::NodePtr Node = Common::XmlTools::XmlToNode((const char *)Common::RefObjQIPtr<IFaces::IRawDataBuffer>(Stream)->GetData());
        Common::RefObjPtr<IFaces::IFunction> Function =
          IFacesImpl::FunctionFromNode(*Node.Get(), GetSynObj());
        Common::RefObjPtr<IFaces::IFlashView> View = FlashView;
        if (View.Get() && View->CallFunction(Function.Get()) != IFaces::retOk)
        {
          std::cerr << "Error call: " << i->c_str() << std::endl;
        }
        break;
      }
    }
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
  }
}
