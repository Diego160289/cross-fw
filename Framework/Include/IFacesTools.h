#ifndef __IFACESTOOLS_H__
#define __IFACESTOOLS_H__

#include "IFaces.h"
#include "TypeList.h"
#include "RefObjPtr.h"
#include "SyncObj.h"
#include "MutexStub.h"


namespace Common
{
  template <typename TList>
  inline bool ExistsIFace(const char *ifaceId)
  {
    typedef    typename TList::Head CurType;
    const char *CurIFaceId = CurType::GetUUID();
    const char *IFaceId = ifaceId;
    for ( ; *CurIFaceId && *IFaceId ; ++CurIFaceId, ++IFaceId)
    {
      if (*CurIFaceId != *IFaceId)
      return ExistsIFace<typename TList::Tail>(ifaceId);
    }
    return (*CurIFaceId || *IFaceId) ? ExistsIFace<typename TList::Tail>(ifaceId) : true;
  }

  template <>
  inline bool ExistsIFace<NullType>(const char *)
  {
    return false;
  }

  template <typename TList>
  class InheritedFromIFacesList
    : public TList::Head
    , public InheritedFromIFacesList<typename TList::Tail>
  {
  public:
    void CastTo(const char *ifaceId, void **iface)
    {
      typedef typename TList::Head CurBase;
      const char *CurId = CurBase::GetUUID();
      const char *DestId = ifaceId;
      while (*DestId && *CurId && *DestId++ == *CurId++);
      if (*DestId || *CurId)
      {
        InheritedFromIFacesList<typename TList::Tail>::CastTo(ifaceId, iface);
        return;
      }
      *iface = static_cast<CurBase*>(this);
    }
  };

  template <>
  class InheritedFromIFacesList<NullType>
  {
  public:
    void CastTo(const char *, void **iface)
    {
      *iface = 0;
    }
  };

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
      SyncObject<TSynObj> Locker(Syn);
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
    ~ModuleCounter();
  private:
    template <typename, typename, template <typename, typename> class, typename>
    friend class CoClassBase;
    static ModuleCounter& GetInstance();
    ModuleCounter();
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
    : public InheritedFromIFacesList<TIFacesList>
    , public InheritedFromIFacesList<TYPE_LIST_1(IFaces::IBase)>
    , virtual public TCreateStrategy<TCoClass, TSynObj>
    , private NoCopyable
  {
  public:
    typedef TypeList<IFaces::IBase, TIFacesList> ExportIFacesList;
    typedef InheritedFromIFacesList<TIFacesList> InheritedInterfaces;
    typedef InheritedFromIFacesList<TYPE_LIST_1(IFaces::IBase)> InheritedBaseInterface;
    CoClassBase()
      : Counter(0)
    {
    }
    virtual unsigned long AddRef()
    {
      SyncObject<TSynObj> Locker(GetSynObj());
      ModuleCounter::GetInstance().Inc();
      return ++Counter;
    }
    virtual unsigned long Release()
    {
      SyncObject<TSynObj> Locker(GetSynObj());
      ModuleCounter::GetInstance().Dec();
      unsigned long NewCounter = --Counter;
      if (!NewCounter)
        delete this;
      return NewCounter;
    }
    virtual bool QueryInterface(const char *ifaceId, void **iface, IFaces::IErrorInfo *errInfo = 0)
    {
      SyncObject<TSynObj> Locker(GetSynObj());
      if (ExistsIFace<ExportIFacesList>(ifaceId))
      {
        TCoClass *CoClassPtr = dynamic_cast<TCoClass *>(this);
        if (!CoClassPtr)
          return false;
        CoClassPtr->InheritedInterfaces::CastTo(ifaceId, iface);
        if (*iface)
        {
          AddRef();
          return true;
        }
        else
        {
          CoClassPtr->InheritedBaseInterface::CastTo(ifaceId, iface);
          if (*iface)
          {
            AddRef();
            return true;
          }
        }
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

  template <typename TCoClassList>
  struct ObjectCreator
  {
    static RefObjPtr<IFaces::IBase>
    CreateObject(const char *classId)
    {
      if (!TypeListLength<TCoClassList>::Len)
        return RefObjPtr<IFaces::IBase>(0);
      const char *ClassId = classId;
      typedef typename TCoClassList::Head CurCoClass;
      const char *CurClassId = CurCoClass::GetUUID();
      while (*ClassId && *CurClassId && *ClassId++ == *CurClassId++);
      if (*ClassId || *CurClassId)
        return ObjectCreator<typename TCoClassList::Tail>::CreateObject(classId);
      RefObjPtr<IFaces::IBase> Ret;
      CurCoClass::CreateObject()->CurCoClass::QueryInterface(IFaces::IBase::GetUUID(), reinterpret_cast<void**>(&Ret));
      return Ret;
    }
  };

  template <>
  struct ObjectCreator<NullType>
  {
    static RefObjPtr<IFaces::IBase>
    CreateObject(const char *)
    {
      return RefObjPtr<IFaces::IBase>(0);
    }
  };

}

#endif	// !__IFACESTOOLS_H__
