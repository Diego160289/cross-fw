#ifndef __IFACESTOOLS_H__
#define __IFACESTOOLS_H__

#include "IFaces.h"
#include "TypeList.h"
#include "RefObjPtr.h"
#include "SyncObj.h"
#include "MutexStub.h"


namespace Common
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retNoInterface;

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

  template <template <typename> class TBase, typename TList>
  class InheritedBase
    : public TBase<TList>
  {
  public:
    virtual ~InheritedBase()
    {
    }
  };

  template <template <typename> class TBase>
  class InheritedBase<TBase, NullType>
    : virtual public TBase<NullType>
  {
  public:
    virtual ~InheritedBase()
    {
    }
  };

  template <typename TList>
  class InheritedFromIFacesList
    : public TList::Head
    , public InheritedBase<InheritedFromIFacesList, typename TList::Tail>
  {
  public:
    virtual ~InheritedFromIFacesList()
    {
    }
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
    virtual ~InheritedFromIFacesList()
    {
    }
    void CastTo(const char *, void **iface)
    {
      *iface = 0;
    }
  };

  template <typename TList>
  class InheritedFromTList
    : public TList::Head
    , public InheritedBase<InheritedFromTList, typename TList::Tail>
  {
  public:
    virtual ~InheritedFromTList()
    {
    }
  };

  template <>
  class InheritedFromTList<NullType>
  {
  public:
    virtual ~InheritedFromTList()
    {
    }
  };

  template <typename T, typename TSynObj>
  class MultiObject
  {
  public:
    typedef T TCoClassType;
    typedef RefObjPtr<TCoClassType> TCoClassTypePtr;
    static RefObjPtr<T> CreateObject()
    {
      TCoClassTypePtr Ret(new T);
      return (Ret->SuccessfulCreated() = Ret->FinalizeCreate()) ?
        Ret : RefObjPtr<T>();
    }
  protected:
    static void FinalizeDestroy()
    {
    }
  };

  template <typename T, typename TSynObj>
  class SingleObject
  {
  public:
    typedef T TCoClassType;
    typedef RefObjPtr<TCoClassType> TCoClassTypePtr;
    static TCoClassTypePtr CreateObject()
    {
      SyncObject<TSynObj> Locker(InternalGetSyn());
      TCoClassTypePtr Ret = InternalGetInstance();
      if (!Ret.Get())
      {
        Ret = new T;
        if (!(Ret->SuccessfulCreated() = Ret->FinalizeCreate()))
          Ret.Release();
      }
      return Ret;
    }
  private:
    static TSynObj& InternalGetSyn()
    {
      static TSynObj Syn;
      return Syn;
    }
    static TCoClassTypePtr InternalGetInstance()
    {
      static TCoClassTypePtr Instance;
      return Instance;
    }
  protected:
    static void FinalizeDestroy()
    {
      InternalGetInstance().Release();
    }
  };

  template <typename>
  class CoClassRoot;

  class ModuleCounter
  {
  public:
    static unsigned long GetModuleCounter();
    ~ModuleCounter();
  private:
    template <typename>
    friend class CoClassRoot;
    static ModuleCounter& GetInstance();
    ModuleCounter();
    void Inc();
    void Dec();
    class CounterImpl;
    CounterImpl *Counter;
  };

  template
  <
    typename TSynObj = System::MutexStub
  >
  class CoClassRoot
    : private NoCopyable
    , public InheritedFromIFacesList<TYPE_LIST_1(IFaces::IBase)>
  {
  public:
    CoClassRoot()
      : Counter(0)
      , IsSuccessfulCreated(false)
    {
    }
    virtual ~CoClassRoot()
    {
    }
  private:
    mutable TSynObj SynObj;
    unsigned long Counter;
    bool IsSuccessfulCreated;
  protected:
    unsigned long IncCounter()
    {
      ModuleCounter::GetInstance().Inc();
      return ++Counter;
    }
    unsigned long DecCounter()
    {
      ModuleCounter::GetInstance().Dec();
      return --Counter;
    }
    unsigned long InternalAddRef()
    {
      return IncCounter();
    }
    unsigned long InternalRelease()
    {
      unsigned long NewCounter = 0;
      {
        SyncObject<TSynObj> Locker(GetSynObj());
        NewCounter = DecCounter();
      }
      if (!NewCounter)
      {
        if (SuccessfulCreated())
          BeforeDestroy();
        delete this;
      }
      return NewCounter;
    }
    bool& SuccessfulCreated()
    {
      return IsSuccessfulCreated;
    }
    TSynObj& GetSynObj() const
    {
      return SynObj;
    }
    virtual bool FinalizeCreate()
    {
      return true;
    }
    virtual void BeforeDestroy()
    {
    }
  };

  template
  <
    typename TCoClass,
    typename TIFacesList,
    template <typename, typename> class TCreateStrategy = MultiObject,
    typename TSynObj = System::MutexStub,
    typename TIFaceImplList = NullType
  >
  class CoClassBase
    : public InheritedFromIFacesList<TIFacesList>
    , public InheritedFromTList<TIFaceImplList>
    , virtual public CoClassRoot<TSynObj>
    , virtual public TCreateStrategy<TCoClass, TSynObj>
  {
  public:
    typedef TypeList<IFaces::IBase, TIFacesList> ExportIFacesList;
    typedef InheritedFromIFacesList<TIFacesList> InheritedInterfaces;
    typedef InheritedFromIFacesList<TYPE_LIST_1(IFaces::IBase)> InheritedBaseInterface;
    typedef TSynObj TThisSynObj;
    typedef TCreateStrategy<TCoClass, TSynObj> TTithCreateStrategy;
    CoClassBase()
    {
    }
    virtual ~CoClassBase()
    {
    }
    virtual unsigned long AddRef()
    {
      SyncObject<TSynObj> Locker(this->GetSynObj());
      return InternalAddRef();
    }
    virtual unsigned long Release()
    {
      unsigned long NewCounter = InternalRelease();
      if (!NewCounter)
        TTithCreateStrategy::FinalizeDestroy();
      return NewCounter;
    }
    virtual  RetCode QueryInterface(const char *ifaceId, void **iface)
    {
      SyncObject<TSynObj> Locker(this->GetSynObj());
      if (ExistsIFace<ExportIFacesList>(ifaceId))
      {
        TCoClass *CoClassPtr = dynamic_cast<TCoClass *>(this);
        if (!CoClassPtr)
          return retFail;
        CoClassPtr->InheritedInterfaces::CastTo(ifaceId, iface);
        if (*iface)
        {
          InternalAddRef();
          return retOk;
        }
        else
        {
          CoClassPtr->InheritedBaseInterface::CastTo(ifaceId, iface);
          if (*iface)
          {
            InternalAddRef();
            return retOk;
          }
        }
      }
      else
        if (ExistsIFace<TIFaceImplList>(ifaceId))
        {
          return retFail;
        }
      return retNoInterface;
    }
  private:
    friend class TCreateStrategy<TCoClass, TSynObj>;
  protected:
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
      RefObjPtr<CurCoClass> NewObj = CurCoClass::TTithCreateStrategy::CreateObject();
      if (NewObj.Get() &&
        NewObj->CurCoClass::QueryInterface(IFaces::IBase::GetUUID(),
          reinterpret_cast<void**>(&Ret)) != retOk)
      {
        return RefObjPtr<IFaces::IBase>();
      }
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
