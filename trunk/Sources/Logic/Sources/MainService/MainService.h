#ifndef __MAINSERVICE_H__
#define __MAINSERVICE_H__

#include "../../Framework/Include/ServiceBase.h"
#include "../../Terminal/Include/Terminal.h"
#include "MainDataSrcIFaces.h"
#include "MainViewIFaces.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


class IMainServiceImpl
  : public Common::ServiceBase
      <
      TYPE_LIST_1(IFaces::IMainViewCallback)
      >
{
public:
  DECLARE_UUID(1a35f7c9-b563-46f2-9ba5-461f57055901)

  IMainServiceImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  virtual bool OnInit();
  virtual void OnDone();

  // IMainViewCallback
  virtual RetCode OnQueryExternalResource(const char *resName, IFaces::IStream **stream);
  virtual RetCode GetBusinessCategories(const wchar_t *objectId, const wchar_t *frameId);
  virtual RetCode BusinessCategorySelected(const wchar_t *objectId, const wchar_t *frameId,
    const wchar_t *businessCategoryId);
  virtual RetCode GetProviders(const wchar_t *objectId, const wchar_t *frameId);
  virtual RetCode ProviderSelected(const wchar_t *objectId, const wchar_t *frameId,
      const wchar_t *providerId);
private:
  Common::RefObjQIPtr<IFaces::IMainDataSource> DataSrc;
  Common::RefObjQIPtr<IFaces::IMainView> View;
};

#endif  // !__MAINSERVICE_H__
