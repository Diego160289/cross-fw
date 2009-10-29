#ifndef __MAINDATASRC_H__
#define __MAINDATASRC_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainDataSrcImpl
  : public Common::ServiceBase<>
{
public:
  DECLARE_UUID(e1156d71-fd1a-4a4a-82ed-a1ab73dc9c87)

  IMainDataSrcImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

private:
};

#endif  // !__MAINDATASRC_H__
