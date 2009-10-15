#include "INamedVariableImpl.h"


namespace IFacesImpl
{
  INamedVariableImpl::INamedVariableImpl()
    : Name("")
  {
  }

  void INamedVariableImpl::SetName(const char *name)
  {
    Common::ISyncObject Locker(GetSynObj());
    Name = name;
  }

  const char* INamedVariableImpl::GetName() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Name.c_str();
  }

  RetCode INamedVariableImpl::Get(IFaces::IVariant **var) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Var.QueryInterface(var);
  }

  RetCode INamedVariableImpl::Set(IFaces::IVariant *var)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!var)
      return retFail;
    Var = var;
    return retOk;
  }


  INamedVariableHelper::INamedVariableHelper(INamedVariablePtr namedVar)
    : NamedVar(namedVar)
  {
    if (!NamedVar.Get())
      throw INamedVariableHelperException("Empty named variable");
  }

  const std::string INamedVariableHelper::GetName() const
  {
    return NamedVar->GetName();
  }

  const IVariantHelper INamedVariableHelper::GetValue() const
  {
    Common::RefObjPtr<IFaces::IVariant> Var;
    if (NamedVar->Get(Var.GetPPtr()) != retOk)
      throw INamedVariableHelperException("Can't get value");
    return Var;
  }

  void INamedVariableHelper::SetValue(Common::RefObjPtr<IFaces::IVariant> var)
  {
    if (NamedVar->Set(var.Get()) != retOk)
      throw INamedVariableHelperException("Can't set value");
  }

}
