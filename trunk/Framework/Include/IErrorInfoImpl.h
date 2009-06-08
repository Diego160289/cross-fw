#ifndef __IERRORINFOIMPL_H__
#define __IERRORINFOIMPL_H__

#include "IFacesTools.h"

namespace IFacesImpl
{
  template <template <typename , typename > class TCreateStrategy = IFacesTools::MultiObject, typename TSynObj = System::MutexStub>
  class IErrorInfoImpl
    : public IFacesTools::CoClassBase<IErrorInfoImpl<TCreateStrategy, TSynObj> , TYPE_LIST_1(IFaces::IErrorInfo), TCreateStrategy, TSynObj>
  {
  public:
    virtual void AddError(unsigned long code, const char *message)
    {
    }
  };
}

#endif	// !__IERRORINFOIMPL_H__
