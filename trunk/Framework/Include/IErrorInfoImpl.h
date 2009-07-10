#ifndef __IERRORINFOIMPL_H__
#define __IERRORINFOIMPL_H__

#include "IFacesTools.h"

namespace IFacesImpl
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFail;

  template
  <
    template <typename, typename > class TCreateStrategy = Common::MultiObject,
    typename TSynObj = System::MutexStub
  >
  class IErrorInfoImpl
    : public Common::CoClassBase<IErrorInfoImpl<TCreateStrategy, TSynObj> , TYPE_LIST_1(IFaces::IErrorInfo), TCreateStrategy, TSynObj>
  {
  public:
    DECLARE_UUID(631e8a5c-9e5f-4ea3-b115-f88d84119923)

    virtual RetCode CALL_TYPE AddError(unsigned long code, const char *message)
    {
      return retOk;
    }
    virtual RetCode CALL_TYPE AddError(unsigned long code, const char *message, const char *file, unsigned long line)
    {
      return retOk;
    }
  };
}

#endif	// !__IERRORINFOIMPL_H__
