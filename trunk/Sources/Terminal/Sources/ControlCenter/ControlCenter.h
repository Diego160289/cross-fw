#ifndef __CONTROLCENTER_H__
#define __CONTROLCENTER_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Framework/Include/OGLUI/UI.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IControlCenterImpl
  : public Common::ServiceBase<>
{
public:
  DECLARE_UUID(fe5725f6-86db-4e7b-a78f-54bf9cc89bf1)

  IControlCenterImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

private:
  Common::SharedPtr<OGLUI::ClientContext> ClientContext;
};

#endif  // !__CONTROLCENTER_H__
