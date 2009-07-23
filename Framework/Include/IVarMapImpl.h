#ifndef __IVARMAPIMPL_H__
#define __IVARMAPIMPL_H__

#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"
#include "IVariantImpl.h"
#include "INamedVariableImpl.h"
#include "IEnumImpl.h"


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  class IVarMapImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IVarMap)
        >
  {
  public:
    DECLARE_UUID(6ccdffd6-0ef7-4b5f-af12-d1b1a400c3f9)


    IVarMapImpl()
    {
    }
    virtual ~IVarMapImpl()
    {
    }

    // IVarMap
    virtual RetCode AddVariable(IFaces::INamedVariable *namedVar)
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
    virtual RetCode RemoveVariable(const char *varName)
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
    virtual RetCode GetVariable(const char *varName, IFaces::IVariant **var) const
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
    virtual RetCode EnumVariables(IFaces::IEnum **vars) const
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

  private:
    typedef Common::RefObjPtr<IFaces::INamedVariable> INamedVariablePtr;
    typedef std::vector<INamedVariablePtr> VarPool;
    VarPool Vars;
  };

  DECLARE_RUNTIME_EXCEPTION(IVarMapHelper)

  class IVarMapHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IVarMap> IVarMapPtr;

    IVarMapHelper(IVarMapPtr varMap);
    void AddVariable(Common::RefObjPtr<IFaces::INamedVariable> namedVar);
    template <typename T>
    void AddVariable(const std::string &varName, T var)
    {
      IVariantHelper Var(CreateVariant<System::Mutex>());
      Var = var;
      Common::RefObjPtr<IFaces::INamedVariable> NamedVar =
        CreateNamedVariable<System::Mutex>(varName.c_str(), Var.GetVariant().Get());
      AddVariable(NamedVar);
    }
    void RemoveVariable(const std::string &varName);
    const IVariantHelper GetVariable(const std::string &varName) const;
    const IEnumHelper EnumVariables() const;

  private:
    IVarMapPtr VarMap;
  };

  template <typename TSyn>
  Common::RefObjPtr<IVarMapImpl>
  CreateVarMap()
  {
    return Common::IBaseImpl<IVarMapImpl>::Create<TSyn>();
  }

}

#endif  // !__IVARMAPIMPL_H__
