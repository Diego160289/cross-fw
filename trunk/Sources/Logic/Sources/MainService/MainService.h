#ifndef __MAINSERVICE_H__
#define __MAINSERVICE_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainServiceImpl
  : public Common::ServiceBase
      <
        TYPE_LIST_1(IFaces::IViewCallback)
      >
{
public:
  DECLARE_UUID(1a35f7c9-b563-46f2-9ba5-461f57055901)

  IMainServiceImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

  // !IViewCallback
  virtual RetCode QueryExternalResource(const char *resName, IFaces::IStream **resStream);
  virtual void Execute(IFaces::IFunction *func);

private:
  Common::RefObjPtr<IFaces::IStorage> DataSrc;
  Common::RefObjPtr<IFaces::IFlashView> FlashView;
  unsigned FlashServiceHandle;
  Common::RefObjPtr<IFaces::IViewManager> ViewManager;
};

#endif  // !__MAINSERVICE_H__
