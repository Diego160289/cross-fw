#ifndef __IMAINVIEWIFACES_H__
#define __IMAINVIEWIFACES_H__

#include "../../Framework/Include/IFaces.h"

namespace IFaces
{
  struct IMainViewCallback;

  struct BusinessCategoriesItem
  {
    unsigned long Id;
    std::wstring Name;
    std::wstring Description;
    std::wstring Resource;
  };

  enum ViewRetCode {
    vrcOk
  };

  struct IMainViewFuncs
    : public IBase
  {
    DECLARE_UUID(e063c3cf-101d-4412-93fa-faa66b0dd842)

    virtual RetCode OnGetBusinessCategories(const BusinessCategoriesItem *categories, unsigned count,
      ViewRetCode code = vrcOk, const wchar_t *codeDescription = 0) = 0;
    virtual RetCode OnGetProviders(const BusinessCategoriesItem *providers, unsigned count,
      ViewRetCode code = vrcOk, const wchar_t *codeDescription = 0) = 0;
  };

  struct IMainView
    : public IBase
  {
    DECLARE_UUID(485dae28-81e7-41e8-83e4-66618b4efb80)

    virtual void SetCallback(IMainViewCallback *callback) = 0;
    virtual RetCode ShowView() = 0;
  };


  struct IMainViewCallback
    : public IBase
  {
    DECLARE_UUID(97b825fb-42bb-4598-9472-c47fda66e5a9)
    
    virtual RetCode OnQueryExternalResource(const char *resName, IStream **stream) = 0;
    virtual RetCode GetBusinessCategories(const wchar_t *objectId, const wchar_t *frameId) = 0;
    virtual RetCode BusinessCategorySelected(const wchar_t *objectId, const wchar_t *frameId,
      const wchar_t *businessCategoryId) = 0;
    virtual RetCode GetProviders(const wchar_t *objectId, const wchar_t *frameId) = 0;
    virtual RetCode ProviderSelected(const wchar_t *objectId, const wchar_t *frameId,
      const wchar_t *providerId) = 0;
  };

}

#endif  // !__IMAINVIEWIFACES_H__
