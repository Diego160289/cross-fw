#ifndef __IFACESTOOLS_H__
#define __IFACESTOOLS_H__

#include "IFaces.h"
#include "TypeList.h"
#include "RefObjPtr.h"
#include "SyncObj.h"
#include "Pointers.h"
#include "MutexStub.h"
#include "IsBaseOf.h"

#include <stdexcept>


namespace Common
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retNoInterface;


  template <typename TList>
  class InheritedFromIFacesList
    : public TList::Head
    , public InheritedFromIFacesList<typename TList::Tail>
  {
  };

  template <typename TList>
  class InheritedFromIFacesList<TYPE_LIST_1(TList)>
    : public TList
  {
  };


  template <typename T>
  class IBaseImpl;

  class ModuleCounter
  {
  public:
    static unsigned long GetModuleCounter();
    ~ModuleCounter();
  private:
    template <typename T> friend class IBaseImpl;
    static ModuleCounter& GetInstance();
    ModuleCounter();
    void Inc();
    void Dec();
    class CounterImpl;
    CounterImpl *Counter;
  };

  struct ISynObj
  {
    virtual ~ISynObj()
    {
    }
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    virtual SharedPtr<ISynObj> Clone() const = 0;
  };

  template <typename TSynObj>
  class ISynObjImpl
    : private NoCopyable
    , public ISynObj
  {
  public:
    virtual void Lock()
    {
      try
      {
        SynObj.Lock();
      }
      catch (std::exception &)
      {
      }
    }
    virtual void Unlock()
    {
      try
      {
        SynObj.Unlock();
      }
      catch (std::exception &)
      {
      }
    }
    virtual SharedPtr<ISynObj> Clone() const
    {
      SharedPtr<ISynObj> Ret(new ISynObjImpl<TSynObj>);
      return Ret;
    }
  private:
    TSynObj SynObj;
  };

  typedef SyncObject<ISynObj> ISyncObject;

  class ICoClassBase
    : private NoCopyable
  {
  protected:
    virtual ~ICoClassBase()
    {
    }

    virtual ISynObj& GetSynObj() const = 0;
    virtual RefObjPtr<IFaces::IBase> GetThisIBase() const = 0;
    virtual bool FinalizeCreate()
    {
      return true;
    }
    virtual void BeforeDestroy()
    {
    }
  };

  inline bool IsEqualUUID(const char *classId1, const char *classId2)
  {
      while (*classId1 && *classId2 && *classId1++ == *classId2++);
      return !(*classId1 || *classId2);
  }


  template <bool>
  struct BoolType
  {
  };

  template
  <
    typename T,
    typename TList
  >
  struct QueryIFaceFromInherited
  {
    static void* Query(const char *ifaceId, T *coClass,
                        bool expandCoClassList,
                        BoolType<false>)
    {
      typedef typename TList::Head CurType;
      if (IsEqualUUID(ifaceId, CurType::GetUUID()))
        return static_cast<CurType*>(coClass);
      if (expandCoClassList)
      {
        return QueryIFaceFromInherited<T, typename TList::Tail>::Query(
          ifaceId, coClass, true, BoolType<!!IsBaseOf<ICoClassBase, CurType>::IsBase>()
          );
      }
      return 0;
    }
    static void* Query(const char *ifaceId, T *coClass,
                        bool expandCoClassList,
                        BoolType<true>)
    {
      typedef typename TList::Head CurType;
      typedef typename CurType::TExportList CoClassBaseList;
      return QueryIFaceFromInherited<T, CoClassBaseList>::Query(
        ifaceId, coClass, false, BoolType<false>()
        );
    }
  };

  template
    <
      typename T
    >
  struct QueryIFaceFromInherited<T, NullType>
  {
    static void *Query(const char *, T *, bool, BoolType<false>)
    {
      return 0;
    }
    static void *Query(const char *, T *, bool, BoolType<true>)
    {
      return 0;
    }
  };

  class IBaseStub
    : public IFaces::IBase
  {
  };

  template
    <
      typename T
    >
  class IBaseImpl
    : public T
    , public IBaseStub
  {
  public:
    typedef T TCoClassType;

    virtual unsigned long AddRef()
    {
      ISyncObject Locker(GetSynObj());
      return InternalAddRef();
    }
    virtual unsigned long Release()
    {
      return InternalRelease();
    }
    virtual  RetCode QueryInterface(const char *ifaceId, void **iface)
    {
      ISyncObject Locker(GetSynObj());
      if (IsEqualUUID(ifaceId, IFaces::IBase::GetUUID()))
        *iface = static_cast<IBaseStub*>(this);
      else
      {
        if (!(*iface = QueryIFaceFromInherited<T, typename T::TExportList>::Query(
          ifaceId, static_cast<T*>(this), true, BoolType<false>()
          )))
          return retNoInterface;
      }
      InternalAddRef();
      return retOk;
    }

    virtual ISynObj& GetSynObj() const
    {
      return *SynObj.Get();
    }
    virtual RefObjPtr<IFaces::IBase> GetThisIBase() const
    {
      IFaces::IBase *ThisIBasePtr = static_cast<IFaces::IBase*>
        (
          const_cast<IBaseStub*>
          (
            static_cast<const IBaseStub*>(this)
          )
        );
      return RefObjPtr<IFaces::IBase>(ThisIBasePtr);
    }

    typedef IBaseImpl<T> TBaseImpl;
    typedef RefObjPtr<TBaseImpl> TBaseImplPtr;

    template <typename TSynObj>
    static TBaseImplPtr Create()
    {
      TBaseImplPtr Object(new TBaseImpl(ISynObjPtr(new ISynObjImpl<TSynObj>)));
      if (!Object->FinalizeCreate())
        return TBaseImplPtr();
      Object->IsSuccessfulCreated = true;
      return Object;
    }
    static TBaseImplPtr CreateWithSyn(const ISynObj &synObj)
    {
      TBaseImplPtr Object(new TBaseImpl(synObj.Clone()));
      if (!Object->FinalizeCreate())
        return TBaseImplPtr();
      Object->IsSuccessfulCreated = true;
      return Object;
    }

  private:
    typedef SharedPtr<ISynObj> ISynObjPtr;
    mutable ISynObjPtr SynObj;

    unsigned long Counter;
    bool IsSuccessfulCreated;

    IBaseImpl(ISynObjPtr synObj)
      : SynObj(synObj)
      , Counter(0)
      , IsSuccessfulCreated(false)
    {
    }
    unsigned long InternalAddRef()
    {
      ModuleCounter::GetInstance().Inc();
      return ++Counter;
    }
    unsigned long InternalRelease()
    {
      unsigned long NewCounter = 0;
      {
        ISyncObject Locker(GetSynObj());
        NewCounter = --Counter;
      }
      if (!NewCounter)
      {
        if (IsSuccessfulCreated)
          this->BeforeDestroy();
        delete this;
      }
      ModuleCounter::GetInstance().Dec();
      return NewCounter;
    }

  protected:
    virtual ~IBaseImpl()
    {
    }
  };

  template
  <
    typename TBaseClassesList
  >
  class CoClassBase
    : virtual public ICoClassBase
    , public InheritedFromIFacesList<TBaseClassesList>
  {
  public:
    typedef TBaseClassesList TExportList;
  };


  template
    <
      typename TCoClass,
      typename TSynObj
    >
  Common::RefObjPtr<Common::IBaseImpl<TCoClass> >
  CreateNewInstance()
  {
    SharedPtr<ISynObj> NewSyn(new ISynObjImpl<TSynObj>);
    Common::RefObjPtr<Common::IBaseImpl<TCoClass> > Ret =
      Common::IBaseImpl<TCoClass>::CreateWithSyn(*NewSyn.Get());
    return Ret;
  }

  template
    <
      typename TSynObj,
      typename TCoClassList
    >
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
        return ObjectCreator<TSynObj, typename TCoClassList::Tail>::CreateObject(classId);
      RefObjPtr<IFaces::IBase> Ret;
      typedef IBaseImpl<CurCoClass> CurCoClassImpl;
      RefObjPtr<CurCoClassImpl> NewObj =
        CreateNewInstance<CurCoClass, TSynObj>();
      if (NewObj.Get() &&
        NewObj->QueryInterface(IFaces::IBase::GetUUID(),
          reinterpret_cast<void**>(Ret.GetPPtr())) != retOk)
      {
        return RefObjPtr<IFaces::IBase>();
      }
      return Ret;
    }
  };

  template
    <
      typename TSynObj
    >
  struct ObjectCreator<TSynObj, NullType>
  {
    static RefObjPtr<IFaces::IBase>
    CreateObject(const char *)
    {
      return RefObjPtr<IFaces::IBase>(0);
    }
  };
}

#endif	// !__IFACESTOOLS_H__
