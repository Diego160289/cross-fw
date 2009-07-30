#ifndef __MAINSERVICE_H__
#define __MAINSERVICE_H__

#include "../../Framework/Include/ServiceBase.h"


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
};

#endif  // !__MAINSERVICE_H__
