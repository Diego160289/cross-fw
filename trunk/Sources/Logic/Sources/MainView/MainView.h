#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainViewImpl
  : public Common::ServiceBase<>
{
public:
  DECLARE_UUID(ac0dd189-8db2-47fb-a590-438fd151910a)

  IMainViewImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

private:
};

#endif  // !__MAINVIEW_H__
