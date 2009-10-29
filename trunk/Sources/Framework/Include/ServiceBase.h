#ifndef __SERVICEBASE_H__
#define __SERVICEBASE_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "ServiceParamNames.h"
#include "RefObjQIPtr.h"
#include "IVarMapImpl.h"

#include <vector>

namespace Common
{

  using IFaces::RetCode;
  using IFaces::retFail;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retBadParam;
  using IFaces::retNotImpl;

  DECLARE_RUNTIME_EXCEPTION(ServiceList)

  class ServiceList
    : private NoCopyable
  {
  public:
    typedef RefObjPtr<IFaces::IServiceManager> IServiceManagerPtr;
    typedef RefObjPtr<IFaces::IService> IServicePtr;
    ServiceList(IServiceManagerPtr manager)
      : Manager(manager)
    {
      if (!Manager.Get())
        throw ServiceListException("Empty namager");
    }
    ~ServiceList()
    {
      for (ServicePool::reverse_iterator i = Services.rbegin() ; i != Services.rend() ; ++i)
        Manager->StopService(i->first);
      Services.clear();
    }
    IServicePtr StartService(const std::string &serviceId)
    {
      IServicePtr NewService;
      unsigned long ServiceId = Manager->StartService(serviceId.c_str(),
        reinterpret_cast<IFaces::IBase**>(NewService.GetPPtr()));
      if (!ServiceId)
        throw ServiceListException("Can't start service");
      Services.push_back(std::make_pair(ServiceId, NewService));
      return NewService;
    }
  private:
    IServiceManagerPtr Manager;
    typedef std::pair<unsigned long, IServicePtr> ServiceItem;
    typedef std::vector<ServiceItem> ServicePool;
    ServicePool Services;
  };

  template
  <
    typename TIFacesList = NullType
  >
  class ServiceBase
    : public CoClassBase
        <
          TypeList<IFaces::IService, TIFacesList>
        >
  {
  public:

    ServiceBase()
      : CanDoneService(false)
    {
    }

    // IService
    virtual RetCode SetParams(IFaces::IVarMap *params)
    {
      Common::ISyncObject Locker(this->GetSynObj());
      ParamsMap = params;
      return retOk;
    }
    virtual RetCode Init()
    {
      Common::ISyncObject Locker(this->GetSynObj());
      bool Res = OnInit();
      if (!Res && RunningSrv.Get())
        RunningSrv.Release();
      return Res ? retOk : retFail;
    }
    virtual void Done()
    {
      Common::ISyncObject Locker(this->GetSynObj());
      RunningSrv.Release();
      OnDone();
      ParamsMap.Release();
    }
    virtual bool CanDone() const
    {
      Common::ISyncObject Locker(this->GetSynObj());
      return CanDoneService;
    }

    void MarkToDoneService()
    {
      Common::ISyncObject Locker(this->GetSynObj());
      CanDoneService = true;
    }
    template <typename T>
    RefObjPtr<T> CreateObject(const std::string &classId)
    {
      Common::ISyncObject Locker(this->GetSynObj());
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
      Common::ISyncObject Locker(this->GetSynObj());
      return RefObjQIPtr<IFaces::IServiceManager>(QueryParam(IFacesImpl::PrmServiceManager));
    }
    RefObjPtr<IFaces::IService> StartService(const std::string &serviceId)
    {
      Common::ISyncObject Locker(this->GetSynObj());
      if (!RunningSrv.Get())
        RunningSrv.Reset(new ServiceList(GetServiceManager()));
      return RunningSrv->StartService(serviceId);
    }
  private:
    typedef RefObjPtr<IFaces::IVarMap> IVarMapPtr;
    mutable IVarMapPtr ParamsMap;
    volatile bool CanDoneService;
    AutoPtr<ServiceList> RunningSrv;
    RefObjPtr<IFaces::IBase> QueryParam(const std::string &paramName) const
    {
      Common::ISyncObject Locker(this->GetSynObj());
      try
      {
        return ((RefObjPtr<IFaces::IBase>)(IFacesImpl::IVarMapHelper(ParamsMap).GetVariable(paramName)));
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
