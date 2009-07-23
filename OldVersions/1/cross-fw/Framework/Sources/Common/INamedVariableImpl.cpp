#include "INamedVariableImpl.h"


namespace IFacesImpl
{

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
