#ifndef __SERVICEBASE_H__
#define __SERVICEBASE_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "ServiceParamNames.h"
#include "RefObjQIPtr.h"
#include "IVarMapImpl.h"


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
      Common::ISyncObject Locker(GetSynObj());
      ParamsMap = params;
      return retOk;
    }
    virtual RetCode Init()
    {
      Common::ISyncObject Locker(GetSynObj());
      return OnInit() ? retOk : retFail;
    }
    virtual void Done()
    {
      Common::ISyncObject Locker(GetSynObj());
      OnDone();
      ParamsMap.Release();
    }
    virtual bool CanDone() const
    {
      Common::ISyncObject Locker(GetSynObj());
      return CanDoneService;
    }

    void MarkToDoneService()
    {
      Common::ISyncObject Locker(GetSynObj());
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
    volatile bool CanDoneService;
    RefObjPtr<IFaces::IBase> QueryParam(const std::string &paramName) const
    {
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
