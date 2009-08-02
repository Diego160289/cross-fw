#ifndef __MAINSERVICE_H__
#define __MAINSERVICE_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../FWExtensions/Include/WFExtensions.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainServiceImpl
  : public Common::ServiceBase<>
{
public:
  DECLARE_UUID(1a35f7c9-b563-46f2-9ba5-461f57055901)

  IMainServiceImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

private:
  Common::RefObjPtr<IFaces::IViewManager> ViewManager;
};

#endif  // !__MAINSERVICE_H__
