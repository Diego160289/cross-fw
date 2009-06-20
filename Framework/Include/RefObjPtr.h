#ifndef __REFOBJPTR_H__
#define __REFOBJPTR_H__

namespace Common
{
  template <typename T>
  class RefObjPtr
  {
  public:
    explicit RefObjPtr(T *ptr = 0)
      : Ptr(ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename TOther>
    explicit RefObjPtr(TOther *ptr)
      : Ptr(ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    RefObjPtr(const RefObjPtr &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    template <typename TOther>
    RefObjPtr(const RefObjPtr<TOther> &ptr)
      : Ptr(ptr.Ptr)
    {
      if (Ptr)
        Ptr->AddRef();
    }
    RefObjPtr& operator = (T *ptr)
    {
      RefObjPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    RefObjPtr& operator = (TOther *ptr)
    {
      RefObjPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    RefObjPtr& operator = (const RefObjPtr &ptr)
    {
      RefObjPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    RefObjPtr& operator = (const RefObjPtr<TOther> &ptr)
    {
      RefObjPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    ~RefObjPtr()
    {
      Release();
    }
    void Release()
    {
      if (Ptr)
        Ptr->Release();
      Ptr = 0;
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
    T** operator & ()
    {
      return &Ptr;
    }
    T* Get() const
    {
      return Ptr;
    }
    void Swap(RefObjPtr &ptr)
    {
      T *CurPtr = Ptr;
      Ptr = ptr.Ptr;
      ptr.Ptr = CurPtr;
    }
  private:
    template <typename > friend class RefObjPtr;
    T *Ptr;
  };
}

#endif	// !__REFOBJPTR_H__
