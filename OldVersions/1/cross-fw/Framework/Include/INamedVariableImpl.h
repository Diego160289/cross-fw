#ifndef __INAMEDVARIABLE_H__
#define __INAMEDVARIABLE_H__

#include "IFaces.h"
#include "IFacesTools.h"
#include "SyncObj.h"
#include "RefObjPtr.h"
#include "IVariantImpl.h"
#include "Mutex.h"

#include <string>


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  template
  <
    typename TSynObj = System::MutexStub
  >
  class INamedVariableImpl
    : public Common::CoClassBase
        <
          INamedVariableImpl<TSynObj>,
          TYPE_LIST_1(IFaces::INamedVariable),
          Common::MultiObject, TSynObj
        >
  {
  public:
    DECLARE_UUID(249cae34-efba-46b3-aebe-ad72e0ba97ab)

    typedef INamedVariableImpl<TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

    INamedVariableImpl()
      : Name("")
      , Var(IVariantImpl<TSynObj>::CreateObject())
    {
    }

    void SetName(const char *name)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Name = name;
    }

    //INamedVariable
    virtual const char* GetName() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return Name.c_str();
    }
    virtual RetCode Get(IFaces::IVariant **var) const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return Var.QueryInterface(var);
    }
    virtual RetCode Set(IFaces::IVariant *var)
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

  DECLARE_RUNTIME_EXCEPTION(INamedVariableHelper)

  class INamedVariableHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::INamedVariable> INamedVariablePtr;

    INamedVariableHelper(INamedVariablePtr namedVar);
    const std::string GetName() const;
    const IVariantHelper GetValue() const;
    void SetValue(Common::RefObjPtr<IFaces::IVariant> var);
    template <typename T>
    void SetValue(T var)
    {
      Common::RefObjPtr<IFaces::IVariant> Var = CreateVariant<System::Mutex>();
      {
        IVariantHelper Helper(Var);
        Helper = var;
      }
      SetValue(Var);
    }
  private:
    INamedVariablePtr NamedVar;
  };

  template <typename TSyn>
  typename INamedVariableImpl<TSyn>::ThisTypePtr
  CreateNamedVariable(const char *name)
  {
    typename INamedVariableImpl<TSyn>::ThisTypePtr Ret =
      INamedVariableImpl<TSyn>::CreateObject();
    Ret->SetName(name);
    return Ret;
  }

  template <typename TSyn>
  typename INamedVariableImpl<TSyn>::ThisTypePtr
  CreateNamedVariable(const char *name, IFaces::IVariant *var)
  {
    typename INamedVariableImpl<TSyn>::ThisTypePtr Ret =
      INamedVariableImpl<TSyn>::CreateObject();
    Ret->SetName(name);
    Ret->Set(var);
    return Ret;
  }
}
#endif  // !__INAMEDVARIABLE_H__
