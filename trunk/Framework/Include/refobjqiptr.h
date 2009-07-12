#ifndef __REFOBJQIPTR_H__
#define __REFOBJQIPTR_H__

#include "RefObjPtr.h"

namespace Common
{
  template <typename T>
  class RefObjQIPtr
  {
  public:
    explicit RefObjQIPtr(T *ptr = 0)
      : Ptr(ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename TOther>
    explicit RefObjQIPtr(TOther *ptr)
      : Ptr(0)
    {
      if (ptr)
        ptr->QueryInterface(T::GetUUID(), reinterpret_cast<void**>(&Ptr));
    }
    RefObjQIPtr(const RefObjQIPtr &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename TOther>
    RefObjQIPtr(const RefObjQIPtr<TOther> &ptr)
      : Ptr(0)
    {
      if (ptr.Get())
        ptr.QueryInterface(&Ptr);
    }
    RefObjQIPtr(const RefObjPtr<T> &ptr)
      : Ptr()
    {
      if (ptr.Get())
        ptr.QueryInterface(&Ptr);
    }
    template <typename TOther>
    RefObjQIPtr(const RefObjPtr<TOther> &ptr)
      : Ptr(0)
    {
      if (ptr.Get())
        ptr->QueryInterface(T::GetUUID(), reinterpret_cast<void**>(&Ptr));
    }
    RefObjQIPtr& operator = (T *ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    RefObjQIPtr& operator = (TOther *ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    RefObjQIPtr& operator = (const RefObjQIPtr &ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    RefObjQIPtr& operator = (const RefObjQIPtr<TOther> &ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    RefObjQIPtr& operator = (const RefObjPtr<T> &ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    RefObjQIPtr& operator = (const RefObjPtr<TOther> &ptr)
    {
      RefObjQIPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    ~RefObjQIPtr()
    {
      Release();
    }
    void Release()
    {
      if (Ptr)
        Ptr->Release();
      Ptr = 0;
    }
    template <typename Q>
    IFaces::RetCode QueryInterface(Q **ptr)
    {
      return !Ptr ? IFaces::retFail :
        Ptr->QueryInterface(Q::GetUUID(), reinterpret_cast<void**>(ptr));
    }
    T* Detach()
    {
      T *Ret = Ptr;
      Ptr = 0;
      return Ret;
    }
    T* operator -> () const
    {
      return Ptr;
    }
    T** GetPPtr()
    {
      return &Ptr;
    }
    T* Get() const
    {
      return Ptr;
    }
    void Swap(RefObjQIPtr &ptr)
    {
      T *CurPtr = Ptr;
      Ptr = ptr.Ptr;
      ptr.Ptr = CurPtr;
    }
  private:
    template <typename > friend class RefObjQIPtr;
    T *Ptr;
  };
}

#endif  // !__REFOBJQIPTR_H__
