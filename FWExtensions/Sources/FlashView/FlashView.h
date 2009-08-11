#ifndef __FLASHVIEW_H__
#define __FLASHVIEW_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Framework/Include/Pointers.h"

#include "WFExtensions.h"

#include "FlashCtrlHolder.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IFlashViewImpl
  : public Common::ServiceBase
      <
        Common::TypeListAdapter
          <
            IFaces::IFlashView,
            IFaces::IWndMessageHandler
          >
      >
{
public:
  DECLARE_UUID(83ef6af0-e752-4601-87fe-90b8be229e1f)

  IFlashViewImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

  // IFlashView
  virtual RetCode SetDataSource(IFaces::IStorage *dataSource);
  virtual RetCode PlayMovie(const char *movieName);

  // IWndMessageHandler
  virtual long OnMessage(const IFaces::WindowMessage &msg);

private:
  typedef Common::SharedPtr<FlashCtrlHolder> FlashCtrlHolderPtr;
  FlashCtrlHolderPtr FlashCtrl;
  Common::SharedPtr<Common::ISynObj> DataSourceSynObj;
  Common::RefObjPtr<IFaces::IStorage> DataSource;
};

#endif  // !__FLASHVIEW_H__
