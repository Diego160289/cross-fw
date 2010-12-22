//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================



#include "IVarMapImpl.h"


namespace IFacesImpl
{

  IVarMapImpl::IVarMapImpl()
  {
  }

  IVarMapImpl::~IVarMapImpl()
  {
  }

  RetCode IVarMapImpl::AddVariable(IFaces::INamedVariable *namedVar)
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      std::string Name = INamedVariableHelper(INamedVariablePtr(namedVar)).GetName();
      for (VarPool::const_iterator i = Vars.begin() ; i != Vars.end() ; ++i)
      {
        if (INamedVariableHelper(*i).GetName() == Name)
          return retFail;
      }
      Vars.push_back(INamedVariablePtr(namedVar));
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IVarMapImpl::RemoveVariable(const char *varName)
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      for (VarPool::iterator i = Vars.begin() ; i != Vars.end() ; ++i)
      {
        if (INamedVariableHelper(*i).GetName() == varName)
        {
          Vars.erase(i);
          return retOk;
        }
      }
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retFail;
  }

  RetCode IVarMapImpl::GetVariable(const char *varName, IFaces::IVariant **var) const
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      for (VarPool::const_iterator i = Vars.begin() ; i != Vars.end() ; ++i)
      {
        if (INamedVariableHelper(*i).GetName() == varName)
          return (*i)->Get(var);
      }
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retFail;
  }

  RetCode IVarMapImpl::EnumVariables(IFaces::IEnum **vars) const
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      Common::RefObjPtr<IEnumImpl> NewEnum = CreateEnum<System::Mutex>();
      for (VarPool::const_iterator i = Vars.begin() ; i != Vars.end() ; ++i)
        NewEnum->AddItem(*i);
      return NewEnum.QueryInterface(vars);
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }


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
