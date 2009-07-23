#include "IVarMapImpl.h"


namespace IFacesImpl
{

  IVarMapHelper::IVarMapHelper(IVarMapPtr varMap)
    : VarMap(varMap)
  {
    if (!VarMap.Get())
      throw IVarMapHelperException("Empty varmap ptr");
  }

  void IVarMapHelper::AddVariable(Common::RefObjPtr<IFaces::INamedVariable> namedVar)
  {
    if (VarMap->AddVariable(namedVar.Get()) != retOk)
      throw IVarMapHelperException("Can't add variable");
  }

  void IVarMapHelper::RemoveVariable(const std::string &varName)
  {
    if (VarMap->RemoveVariable(varName.c_str()) != retOk)
      throw IVarMapHelperException("Can't remove variable");
  }

  const IVariantHelper IVarMapHelper::GetVariable(const std::string &varName) const
  {
    Common::RefObjPtr<IFaces::IVariant> Var;
    if (VarMap->GetVariable(varName.c_str(), Var.GetPPtr()) != retOk)
      throw IVarMapHelperException("Can't get variable");
    return Var;
  }

  const IEnumHelper IVarMapHelper::EnumVariables() const
  {
    Common::RefObjPtr<IFaces::IEnum> Enum;
    if (VarMap->EnumVariables(Enum.GetPPtr()) != retOk)
      throw IVarMapHelperException("Can't get variable");
    return Enum;
  }

}
