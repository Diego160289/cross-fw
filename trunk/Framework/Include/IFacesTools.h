#ifndef __IFACESTOOLS_H__
#define __IFACESTOOLS_H__

#include "IFaces.h"
#include "TypeList.h"
#include "RefObjPtr.h"
#include "SyncObj.h"
#include "MutexStub.h"

namespace IFacesTools
{
  template <typename TList>
  inline bool ExistsIFace(const char *ifaceId)
  {
    typedef    typename TList::Head CurType;
    const char *CurIFaceId = CurType::GetIFaceId();
    const char *IFaceId = ifaceId;
    for ( ; *CurIFaceId && *IFaceId ; ++CurIFaceId, ++IFaceId)
    {
      if (*CurIFaceId != *IFaceId)
      return ExistsIFace<typename TList::Tail>(ifaceId);
    }
    return (*CurIFaceId || *IFaceId) ? ExistsIFace<typename TList::Tail>(ifaceId) : true;
  }

  template <>
  inline bool ExistsIFace<Common::NullType>(const char *)
  {
    return false;
  }

  template <typename T, typename TSynObj>
  class MultiObject
  {
  public:
    static RefObjPtr<T> CreateObject()
    {
      RefObjPtr<T> Ret(new T);
      return Ret;
    }
  };

  template <typename T, typename TSynObj>
  class SingleObject
  {
  public:
    static RefObjPtr<T> CreateObject()
    {
      static TSynObj Syn;
      Common::SyncObject<TSynObj> Locker(Syn);
      static RefObjPtr<T> Ret;
      if (Ret.IsEmpty())
      Ret = new T;
      return Ret;
    }
  };

  template <typename, typename, template <typename, typename> class, typename>
  class CoClassBase;

  class ModuleCounter
  {
  public:
    static unsigned long GetModuleCounter();
  private:
    template <typename, typename, template <typename, typename> class, typename>
    friend class CoClassBase;
    static ModuleCounter Instance;
    ModuleCounter();
    ~ModuleCounter();
    void Inc();
    void Dec();
    class CounterImpl;
    CounterImpl *Counter;
  };

  template
  <
    typename TCoClass,
    typename TIFacesList,
    template <typename, typename> class TCreateStrategy = MultiObject,
    typename TSynObj = System::MutexStub
  >
  class CoClassBase
    : virtual public Common::InheritedFromList<TIFacesList>
    , virtual public TCreateStrategy<TCoClass, TSynObj>
    , private Common::NoCopyable
  {
  public:
    typedef Common::TypeList<IFaces::IBase, TIFacesList> ExportIFacesList;
    CoClassBase()
      : Counter(0)
    {
    }
    virtual unsigned long AddRef()
    {
      Common::SyncObject<TSynObj> Locker(GetSynObj());
      ModuleCounter::Instance.Inc();
      return ++Counter;
    }
    virtual unsigned long Release()
    {
      Common::SyncObject<TSynObj> Locker(GetSynObj());
      ModuleCounter::Instance.Dec();
      unsigned long NewCounter = --Counter;
      if (!NewCounter)
      delete this;
      return NewCounter;
    }
    virtual bool QueryInterface(const char *ifaceId, void **iface, IFaces::IErrorInfo *errInfo = 0)
    {
      Common::SyncObject<TSynObj> Locker(GetSynObj());
      if (ExistsIFace<ExportIFacesList>(ifaceId))
      {
        AddRef();
        *iface = this;
        return true;
      }
      return false;
    }
  private:
    TSynObj SynObj;
    unsigned long Counter;
  protected:
    TSynObj& GetSynObj()
    {
      return SynObj;
    }
  };
}

#endif	// !__IFACESTOOLS_H__
