#ifndef __INAMEDVARIABLE_H__
#define __INAMEDVARIABLE_H__

#include "IFaces.h"
#include "IFacesTools.h"
#include "SyncObj.h"
#include "RefObjPtr.h"
#include "IVariantImpl.h"

#include <string>


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  template
  <
    template <typename, typename > class TCreateStrategy = Common::MultiObject,
    typename TSynObj = System::MutexStub
  >
  class INamedVariableImpl
    : public Common::CoClassBase
        <
          INamedVariableImpl<TCreateStrategy, TSynObj>,
          TYPE_LIST_1(IFaces::INamedVariable),
          TCreateStrategy, TSynObj
        >
  {
  public:
    DECLARE_UUID(249cae34-efba-46b3-aebe-ad72e0ba97ab)

    typedef INamedVariableImpl<TCreateStrategy, TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

    INamedVariableImpl()
      : Name("")
      , Var(IFacesImpl::IVariantImpl<Common::MultiObject, TSynObj>::CreateObject())
    {
    }

    void SetName(const char *name)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Name = name;
    }

    //INamedVariable
    virtual const char* CALL_TYPE GetName() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return Name.c_str();
    }
    virtual RetCode CALL_TYPE Get(IFaces::IVariant **var) const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return Var.QueryInterface(var) ? retOk : retFail;
    }
    virtual RetCode CALL_TYPE Set(IFaces::IVariant *var)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      if (!var)
        return retFail;
      Var = var;
      return retOk;
    }
  private:
    std::string Name;
    mutable Common::RefObjPtr<IFaces::IVariant> Var;
  };

  template <typename TSyn>
  typename INamedVariableImpl<Common::MultiObject, TSyn>::ThisTypePtr
  CreateNamedVariable(const char *name)
  {
    typename INamedVariableImpl<Common::MultiObject, TSyn>::ThisTypePtr Ret =
      INamedVariableImpl<Common::MultiObject, TSyn>::CreateObject();
    Ret->SetName(name);
    return Ret;
  }

  template <typename TSyn>
  typename INamedVariableImpl<Common::MultiObject, TSyn>::ThisTypePtr
  CreateNamedVariable(const char *name, IFaces::IVariant *var)
  {
    typename INamedVariableImpl<Common::MultiObject, TSyn>::ThisTypePtr Ret =
      INamedVariableImpl<Common::MultiObject, TSyn>::CreateObject();
    Ret->SetName(name);
    Ret->Set(var);
    return Ret;
  }
}
#endif  // !__INAMEDVARIABLE_H__
