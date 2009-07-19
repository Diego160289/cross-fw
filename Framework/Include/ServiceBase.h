#ifndef __SERVICEBASE_H__
#define __SERVICEBASE_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "ServiceParamNames.h"
#include "RefObjQIPtr.h"
#include "IVarMapImpl.h"

#include <string>


namespace Common
{

  using IFaces::RetCode;
  using IFaces::retFail;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retBadParam;
  using IFaces::retNotImpl;

  template
  <
    typename TServiceCoClass
  >
  class ServiceBase
    : public CoClassBase
        <
          TServiceCoClass,
          TYPE_LIST_1(IFaces::IService),
          MultiObject,
          System::Mutex
        >
  {
  public:
    typedef System::Mutex TSynObj;

    ServiceBase()
      : CanDoneService(false)
    {
    }

    // IService
    virtual void SetInstanceUUID(const char *instanceUUID)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      if (instanceUUID)
        ServiceInstanceUUID = instanceUUID;
      else
        ServiceInstanceUUID.clear();
    }
    virtual RetCode SetParams(IFaces::IVarMap *params)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      ParamsMap = params;
      return retOk;
    }
    virtual RetCode Init()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return OnInit() ? retOk : retFail;
    }
    virtual void Done()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      OnDone();
      ParamsMap.Release();
    }
    virtual bool CanDone() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return CanDoneService;
    }

    const std::string GetInstanceUUID() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return ServiceInstanceUUID;
    }
    void MarkToDoneService()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      CanDoneService = true;
    }
    template <typename T>
    RefObjPtr<T> CreateObject(const std::string &classId)
    {
      RefObjQIPtr<IFaces::IClassFactory> Factory = QueryParam(IFacesImpl::PrmClassFactorry);
      if (!Factory.Get())
        return RefObjPtr<T>();
      RefObjPtr<IFaces::IBase> Ret;
      if (Factory->CreateObject(classId.c_str(), Ret.GetPPtr()) != retOk)
        return RefObjPtr<T>();
      return RefObjQIPtr<T>(Ret);
    }
    RefObjPtr<IFaces::IServiceManager> GetServiceManager() const
    {
      return RefObjQIPtr<IFaces::IServiceManager>(QueryParam(IFacesImpl::PrmServiceManager));
    }
  private:
    typedef RefObjPtr<IFaces::IVarMap> IVarMapPtr;
    mutable IVarMapPtr ParamsMap;
    std::string ServiceInstanceUUID;
    volatile bool CanDoneService;
    RefObjPtr<IFaces::IBase> QueryParam(const std::string &paramName) const
    {
      try
      {
        return RefObjPtr<IFaces::IBase>((IFaces::IBase*)(IFacesImpl::IVarMapHelper(ParamsMap).GetVariable(paramName)));
      }
      catch (std::exception &)
      {
        RefObjPtr<IFaces::IBase>();
      }
      return RefObjPtr<IFaces::IBase>();
    }

  protected:
    virtual bool OnInit()
    {
      return true;
    }
    virtual void OnDone()
    {
    }
  };

}

#endif // !__SERVICEBASE_H__
