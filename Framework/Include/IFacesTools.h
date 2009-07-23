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
  class InheritedFromIFacesList
    : public TList::Head
    , public InheritedFromIFacesList<typename TList::Tail>
  {
  };

  template <>
  class InheritedFromIFacesList<NullType>
  {
  };

  struct ISynObj
  {
    virtual ~ISynObj()
    {
    }
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
  };


  typedef SyncObject<ISynObj> ISyncObject;

  class ICoClassBase
    : private NoCopyable
  {
  protected:
    virtual ISynObj& GetSynObj() const = 0;
  };

  template
    <
      typename T
    >
  class IBaseImpl
    : public T
    , virtual public ICoClassBase
  {
  public:
    virtual unsigned long AddRef()
    {
      return 0;
    }
    virtual unsigned long Release()
    {
      return 0;
    }
    virtual  RetCode QueryInterface(const char *ifaceId, void **iface)
    {
      return retNoInterface;
    }    

    virtual ISynObj& GetSynObj() const
    {
      return *((ISynObj*)0);
    }


    typedef IBaseImpl<T> TBaseImpl;
    typedef RefObjPtr<TBaseImpl> TBaseImplPtr;

    template <typename TSynObj>
    static TBaseImplPtr Create()
    {
      return TBaseImplPtr(new TBaseImpl);
    }

    static TBaseImplPtr Create(const ISynObj &synObj)
    {
      return TBaseImplPtr(new TBaseImpl);
    }

  private:
    // mutable TSynObj SynObj;
  };

  class ModuleCounter
  {
  public:
    static unsigned long GetModuleCounter();
    ~ModuleCounter();
  private:
    static ModuleCounter& GetInstance();
    ModuleCounter();
    void Inc();
    void Dec();
    class CounterImpl;
    CounterImpl *Counter;
  };


  template
  <
    typename TIFacesList
  >
  class CoClassBase
    : virtual public ICoClassBase
    , public InheritedFromIFacesList<TIFacesList>
  {
  public:
  };

}

#endif	// !__IFACESTOOLS_H__
