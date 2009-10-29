#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"
#include "MainViewIFaces.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainViewImpl
  : public Common::ServiceBase
      <
        Common::TypeListAdapter
          <
            IFaces::IViewCallback,
            IFaces::IMainView
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

private:
  Common::RefObjPtr<IFaces::IViewManager> ViewManager;
  Common::RefObjPtr<IFaces::IFlashView> FlashView;
  Common::RefObjPtr<IFaces::IMainViewCallback> Callback;
};

#endif  // !__MAINVIEW_H__
