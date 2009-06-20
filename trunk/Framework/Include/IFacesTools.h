#ifndef __IFACESTOOLS_H__
#define __IFACESTOOLS_H__

#include "IFaces.h"
#include "TypeList.h"
#include "RefObjPtr.h"
#include "SyncObj.h"
#include "MutexStub.h"

#define COCLASS_UUID(id_) \
  static const char* GetCoClassId() \
  { \
    return #id_ ; \
  }

namespace Common
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
  inline bool ExistsIFace<NullType>(const char *)
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
    : virtual public InheritedFromList<TIFacesList>
    , virtual public TCreateStrategy<TCoClass, TSynObj>
    , private NoCopyable
  {
  public:
    typedef TypeList<IFaces::IBase, TIFacesList> ExportIFacesList;
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
      const char *CurClassId = CurCoClass::GetCoClassId();
      while (*ClassId && *CurClassId && *ClassId++ == *CurClassId++);
      return *ClassId || *CurClassId ?
        ObjectCreator<typename TCoClassList::Tail>::CreateObject(classId) :
        RefObjPtr<IFaces::IBase>(CurCoClass::CreateObject());
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
