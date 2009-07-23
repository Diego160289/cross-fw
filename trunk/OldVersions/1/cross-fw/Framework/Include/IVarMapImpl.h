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

  template
  <
    typename TSynObj = System::MutexStub
  >
  class IVarMapImpl
    : public Common::CoClassBase
        <
          IVarMapImpl<TSynObj>,
          TYPE_LIST_1(IFaces::IVarMap),
          Common::MultiObject, TSynObj
        >
  {
  public:
    DECLARE_UUID(6ccdffd6-0ef7-4b5f-af12-d1b1a400c3f9)

    typedef IVarMapImpl<TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

    IVarMapImpl()
    {
    }
    virtual ~IVarMapImpl()
    {
    }

    // IVarMap
    virtual RetCode AddVariable(IFaces::INamedVariable *namedVar)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
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
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
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
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
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
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      try
      {
        IEnumImpl<System::Mutex>::ThisTypePtr NewEnum = CreateEnum<System::Mutex>();
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
  typename IVarMapImpl<TSyn>::ThisTypePtr
  CreateVarMap()
  {
    return IVarMapImpl<TSyn>::CreateObject();
  }

}

#endif  // !__IVARMAPIMPL_H__
