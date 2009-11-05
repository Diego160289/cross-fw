#include "MainView.h"
#include "../../Framework/Include/Xml/XmlTools.h"
#include "../../Framework/Include/IFunctionImpl.h"

using Common::Commands::ArgObj;
using Common::Commands::ArgProp;
using Common::Commands::ArgArr;
using Common::Commands::ArgStr;

IMainViewImpl::IMainViewImpl()
{
  Handlers[L"GetBusinessCategories"] = &IMainViewImpl::GetBusinessCategoriesHandler;
  Handlers[L"BusinessCategorySelected"] = &IMainViewImpl::BusinessCategorySelectedHandler;
  Handlers[L"GetProviders"] = &IMainViewImpl::GetProvidersHandler;
  Handlers[L"ProviderSelected"] = &IMainViewImpl::ProviderSelectedHandler;
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
  Common::ISyncObject Locker(GetSynObj());
  Callback = callback;
}

RetCode IMainViewImpl::ShowView()
{
  Common::ISyncObject Locker(GetSynObj());
  return FlashView->PlayMovie("Terminal.swf");
}

RetCode IMainViewImpl::QueryExternalResource(const char *resName, IFaces::IStream **resStream)
{
  Common::ISyncObject Locker(GetSynObj());
  return !Callback.Get() ? retFail : Callback->OnQueryExternalResource(resName, resStream);
}

void IMainViewImpl::Execute(IFaces::IFunction *func)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    using namespace Common::XmlTools;
    NodePtr Root = IFacesImpl::NodeFromFunction(Common::RefObjPtr<IFaces::IFunction>(func));
    using namespace Common::Commands;
    CommandPtr Cmd = Command::FromNode(Root);
    HandlerPool::iterator Iter = Handlers.find(Cmd->GetName());
    if (Iter == Handlers.end())
      throw IMainViewImplException("Handler not found");
    (this->*Iter->second)((*Cmd.Get())[ArgObj][ArgProp]);
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
  }
}

void IMainViewImpl::GetBusinessCategoriesHandler(const CmdParams &prm)
{
  if (!Callback.Get())
    return;
  RetCode Ret = Callback->GetBusinessCategories(
      prm[L"objectId"][ArgStr].GetString().c_str(),
      prm[L"frameId"][ArgStr].GetString().c_str()
    );

  // TODO: проверить ret и что-то с этим сделать :)
}

RetCode IMainViewImpl::OnGetBusinessCategories(const IFaces::BusinessCategoriesItem *categories, unsigned count,
                                               IFaces::ViewRetCode code, const wchar_t *codeDescription)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Common::Commands::Invoker Inv(L"OnGetBusinessCategories");
    Common::Commands::ObjectPtr Manifest = Inv.ObjectArg()->AddProperty(L"manifest")->ObjectArg()->AddProperty(L"stateInfo")->ObjectArg();
    Manifest->AddProperty(L"code")->StringArg(Common::ToWString(static_cast<unsigned>(code)));
    Manifest->AddProperty(L"description")->StringArg(codeDescription ? codeDescription : L"Success");
    Common::Commands::ArrayPtr Arr = Inv.ObjectArg()->AddProperty(L"arguments")->ArrayArg();
    for (unsigned i = 0 ; i < count ; ++i)
    {
      Common::Commands::ObjectPtr Item = Arr->AddProperty()->ObjectArg();
      Item->AddProperty(L"id")->StringArg(Common::ToWString(categories[i].Id));
      Item->AddProperty(L"name")->StringArg(categories[i].Name);
      Item->AddProperty(L"description")->StringArg(categories[i].Description);
      Item->AddProperty(L"resource")->StringArg(categories[i].Resource);
    }
    return FlashView->CallFunction(IFacesImpl::FunctionFromNode(*Inv.ToNode().Get(), GetSynObj()).Get());
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retFail;
}

void IMainViewImpl::BusinessCategorySelectedHandler(const CmdParams &prm)
{
  if (!Callback.Get())
    return;
  RetCode Ret = Callback->BusinessCategorySelected(
      prm[L"objectId"][ArgStr].GetString().c_str(),
      prm[L"frameId"][ArgStr].GetString().c_str(),
      prm[L"params"][ArgObj][ArgProp][L"businessCategoryId"][ArgStr].GetString().c_str()
    );
}

void IMainViewImpl::GetProvidersHandler(const CmdParams &prm)
{
  if (!Callback.Get())
    return;
  RetCode Ret = Callback->GetProviders(
      prm[L"objectId"][ArgStr].GetString().c_str(),
      prm[L"frameId"][ArgStr].GetString().c_str()
    );
}

RetCode IMainViewImpl::OnGetProviders(const IFaces::BusinessCategoriesItem *providers,
                                      unsigned count, IFaces::ViewRetCode code,
                                      const wchar_t *codeDescription)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Common::Commands::Invoker Inv(L"OnGetProviders");
    Common::Commands::ObjectPtr Manifest = Inv.ObjectArg()->AddProperty(L"manifest")->ObjectArg()->AddProperty(L"stateInfo")->ObjectArg();
    Manifest->AddProperty(L"code")->StringArg(Common::ToWString(static_cast<unsigned>(code)));
    Manifest->AddProperty(L"description")->StringArg(codeDescription ? codeDescription : L"Success");
    Common::Commands::ArrayPtr Arr = Inv.ObjectArg()->AddProperty(L"arguments")->ArrayArg();
    for (unsigned i = 0 ; i < count ; ++i)
    {
      Common::Commands::ObjectPtr Item = Arr->AddProperty()->ObjectArg();
      Item->AddProperty(L"id")->StringArg(Common::ToWString(providers[i].Id));
      Item->AddProperty(L"name")->StringArg(providers[i].Name);
      Item->AddProperty(L"description")->StringArg(providers[i].Description);
      Item->AddProperty(L"resource")->StringArg(providers[i].Resource);
    }
    return FlashView->CallFunction(IFacesImpl::FunctionFromNode(*Inv.ToNode().Get(), GetSynObj()).Get());
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retFail;
}

void IMainViewImpl::ProviderSelectedHandler(const CmdParams &prm)
{
  if (!Callback.Get())
    return;
  RetCode Ret = Callback->ProviderSelected(
      prm[L"objectId"][ArgStr].GetString().c_str(),
      prm[L"frameId"][ArgStr].GetString().c_str(),
      prm[L"params"][ArgObj][ArgProp][L"providerId"][ArgStr].GetString().c_str()
    );
}
