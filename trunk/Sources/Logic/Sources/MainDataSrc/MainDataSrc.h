#ifndef __MAINDATASRC_H__
#define __MAINDATASRC_H__

#include "../../Framework/Include/IStorageFileImpl.h"
#include "../../Framework/Include/IStreamFileImpl.h"
#include "../../Framework/Include/IStorageHelper.h"
#include "../../Framework/Include/IStreamHelper.h"

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"
#include "MainDataSrcIFaces.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainDataSrcImpl
  : public Common::ServiceBase
        <
          Common::TypeListAdapter
            <
              IFacesImpl::IStorageFileImpl,
              IFaces::IMainDataSource
            >
        >
{
public:
  DECLARE_UUID(e1156d71-fd1a-4a4a-82ed-a1ab73dc9c87)

  IMainDataSrcImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

  // IMainDataSource
  virtual RetCode GetResource(const char *resName, IFaces::IStream **stream);

private:
};

#endif  // !__MAINDATASRC_H__
