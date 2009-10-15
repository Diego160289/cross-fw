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


    IVarMapImpl();
    virtual ~IVarMapImpl();

    // IVarMap
    virtual RetCode AddVariable(IFaces::INamedVariable *namedVar);
    virtual RetCode RemoveVariable(const char *varName);
    virtual RetCode GetVariable(const char *varName, IFaces::IVariant **var) const;
    virtual RetCode EnumVariables(IFaces::IEnum **vars) const;

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
