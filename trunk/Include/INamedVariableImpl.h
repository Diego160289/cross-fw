//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __INAMEDVARIABLE_H__
#define __INAMEDVARIABLE_H__

#include "IFaces.h"
#include "IFacesTools.h"
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

  class INamedVariableImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::INamedVariable)
        >
  {
  public:
    DECLARE_UUID(249cae34-efba-46b3-aebe-ad72e0ba97ab)

    INamedVariableImpl();

    void SetName(const char *name);

    //INamedVariable
    virtual const char* GetName() const;
    virtual RetCode Get(IFaces::IVariant **var) const;
    virtual RetCode Set(IFaces::IVariant *var);
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
  Common::RefObjPtr<INamedVariableImpl>
  CreateNamedVariable(const char *name)
  {
    Common::RefObjPtr<INamedVariableImpl> Ret =
      Common::IBaseImpl<INamedVariableImpl>::Create<TSyn>();
    Ret->SetName(name);
    return Ret;
  }

  template <typename TSyn>
  Common::RefObjPtr<INamedVariableImpl>
  CreateNamedVariable(const char *name, IFaces::IVariant *var)
  {
    Common::RefObjPtr<INamedVariableImpl> Ret =
      Common::IBaseImpl<INamedVariableImpl>::Create<TSyn>();
    Ret->SetName(name);
    Ret->Set(var);
    return Ret;
  }
}
#endif  // !__INAMEDVARIABLE_H__
