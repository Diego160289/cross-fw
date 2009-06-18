#ifndef __IERRORINFOIMPL_H__
#define __IERRORINFOIMPL_H__

#include "IFacesTools.h"

namespace IFacesImpl
{
  template
  <
    template <typename, typename > class TCreateStrategy = IFacesTools::MultiObject,
    typename TSynObj = System::MutexStub
  >
  class IErrorInfoImpl
    : public IFacesTools::CoClassBase<IErrorInfoImpl<TCreateStrategy, TSynObj> , TYPE_LIST_1(IFaces::IErrorInfo), TCreateStrategy, TSynObj>
  {
  public:
    COCLASS_UUID(631e8a5c-9e5f-4ea3-b115-f88d84119923)
    virtual void AddError(unsigned long code, const char *message)
    {
    }
    virtual void AddError(unsigned long code, const char *message, const char *file, unsigned long line)
    {
    }
  };
}

#endif	// !__IERRORINFOIMPL_H__
