#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include "../../../Framework/Include/Xml/Commands.h"

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"

#include "MainViewIFaces.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


DECLARE_LOGIC_EXCEPTION(IMainViewImpl)

class IMainViewImpl
  : public Common::ServiceBase
      <
        Common::TypeListAdapter
          <
            IFaces::IViewCallback,
            IFaces::IMainView,
            IFaces::IMainViewFuncs
          >
      >
{
public:
  DECLARE_UUID(ac0dd189-8db2-47fb-a590-438fd151910a)

  IMainViewImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

  // !IViewCallback
  virtual RetCode QueryExternalResource(const char *resName, IFaces::IStream **resStream);
  virtual void Execute(IFaces::IFunction *func);

  // IMainView
  virtual void SetCallback(IFaces::IMainViewCallback *callback);
  virtual RetCode ShowView();

  // IMainViewFuncs
  virtual RetCode OnGetBusinessCategories(const IFaces::BusinessCategoriesItem *categories, unsigned count,
    IFaces::ViewRetCode code = IFaces::vrcOk, const wchar_t *codeDescription = 0);
  virtual RetCode OnGetProviders(const IFaces::BusinessCategoriesItem *providers,
    unsigned count, IFaces::ViewRetCode code = IFaces::vrcOk, const wchar_t *codeDescription = 0);
  virtual RetCode ChangeFrame(const wchar_t *frameId);
private:
  Common::RefObjPtr<IFaces::IViewManager> ViewManager;
  Common::RefObjPtr<IFaces::IFlashView> FlashView;
  Common::RefObjPtr<IFaces::IMainViewCallback> Callback;

  typedef Common::Commands::IArgumentPropertyList CmdParams;
  typedef void (IMainViewImpl::*PCmdHandler)(const CmdParams &);
  typedef std::map<std::wstring, PCmdHandler> HandlerPool;
  HandlerPool Handlers;
  void GetBusinessCategoriesHandler(const CmdParams &prm);
  void BusinessCategorySelectedHandler(const CmdParams &prm);
  void GetProvidersHandler(const CmdParams &prm);
  void ProviderSelectedHandler(const CmdParams &prm);
  void CellPhoneNumberEnteredHandler(const CmdParams &prm);
  void CellPhoneNumberVerifiedHandler(const CmdParams &prm);
  void ProcessPaymentHandler(const CmdParams &prm);
};

#endif  // !__MAINVIEW_H__
