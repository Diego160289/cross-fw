#include "MainView.h"
#include "../../Framework/Include/Xml/XmlTools.h"
#include "../../Framework/Include/Xml/Commands.h"
#include "../../Framework/Include/IFunctionImpl.h"


IMainViewImpl::IMainViewImpl()
{
}

bool IMainViewImpl::FinalizeCreate()
{
  return true;
}

void IMainViewImpl::BeforeDestroy()
{
}
bool IMainViewImpl::OnInit()
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
    Common::RefObjPtr<IFaces::IService> FlashCtrl = 
      StartService("83ef6af0-e752-4601-87fe-90b8be229e1f");
    Common::RefObjQIPtr<IFaces::IFlashView> NewFlashView(FlashCtrl);
    if (!NewFlashView.Get())
      return false;
    Common::RefObjQIPtr<IFaces::IWndMessageHandler> FlashMsgHandler(FlashCtrl);
    if (!FlashMsgHandler.Get())
      return false;
    unsigned WindowIndex = 0;
    if (Frame->CreateWnd(&WindowIndex, FlashMsgHandler.Get()) != retOk)
      return false;
    if (Frame->SetCurWnd(WindowIndex) != retOk)
      return false;
    FlashView = NewFlashView;
    Frame->Show(true);
    NewFlashView->SetViewCallback(Common::RefObjQIPtr<IFaces::IViewCallback>(GetThisIBase()).Get());
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IMainViewImpl::OnDone()
{
  ViewManager.Release();
  FlashView.Release();
}

void IMainViewImpl::SetCallback(IFaces::IMainViewCallback *callback)
{
  Callback = callback;
}

RetCode IMainViewImpl::ShowView()
{
  return FlashView->PlayMovie("Terminal.swf");
}

RetCode IMainViewImpl::QueryExternalResource(const char *resName, IFaces::IStream **resStream)
{
  return !Callback.Get() ? retFail : Callback->OnQueryExternalResource(resName, resStream);
}

void IMainViewImpl::Execute(IFaces::IFunction *func)
{
  try
  {
    {
      using namespace Common::XmlTools;
      // Function to node
      NodePtr Root = IFacesImpl::NodeFromFunction(Common::RefObjPtr<IFaces::IFunction>(func));
      using namespace Common::Commands;
      CommandPtr Cmd = Command::FromNode(Root);
      // Get function name
      std::wstring FuncName = Cmd->GetName();
    }
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
  }
}


/*
std::vector<std::string> Funcs;
    Funcs.push_back("GetBusinessCategories");
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
        IFacesImpl::IStreamHelper(IFacesImpl::IStorageHelper(IFacesImpl::IStorageHelper(Common::RefObjQIPtr<IFaces::IStorage>(DataSrc)).OpenStorage("XmlFuncs")).OpenStream(Path)).CopyTo(Stream);
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
*/