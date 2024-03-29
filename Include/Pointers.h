//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __POINTERS_H__
#define __POINTERS_H__


namespace Common
{
  template <typename T>
  class SharedPtr
  {
  public:
    explicit SharedPtr(T *ptr = 0)
      : Ptr(ptr)
      , Counter(ptr ? new unsigned long(1) : 0)
    {
    }
    template <typename TOther>
    explicit SharedPtr(TOther *ptr)
      : Ptr(ptr)
      , Counter(ptr ? new unsigned long(1) : 0)
    {
    }
    SharedPtr(const SharedPtr &ptr)
      : Ptr(ptr.Ptr)
      , Counter(ptr.Counter)
    {
      if (Counter)
        (*Counter)++;
    }
    template <typename TOther>
    SharedPtr(const SharedPtr<TOther> &ptr)
      : Ptr(ptr.Ptr)
      , Counter(ptr.Counter)
    {
      if (Counter)
        (*Counter)++;
    }
    SharedPtr& operator = (T *ptr)
    {
      SharedPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    SharedPtr& operator = (TOther *ptr)
    {
      SharedPtr<T> Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    SharedPtr& operator = (const SharedPtr &ptr)
    {
      SharedPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    SharedPtr& operator = (const SharedPtr<TOther> &ptr)
    {
      SharedPtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    ~SharedPtr()
    {
      Release();
    }
    void Release()
    {
      if (Counter && !--(*Counter))
      {
        delete Counter;
        delete Ptr;
      }
      Counter = 0;
      Ptr = 0;
    }
    T* operator -> () const
    {
      return Ptr;
    }
    void Swap(SharedPtr &ptr)
    {
      T *CurPtr = Ptr;
      Ptr = ptr.Ptr;
      ptr.Ptr = CurPtr;
      unsigned long *CurCounter = Counter;
      Counter = ptr.Counter;
      ptr.Counter = CurCounter;
    }
    T* Get() const
    {
      return Ptr;
    }
    template <typename TOther>
    void Reset(TOther *ptr)
    {
      SharedPtr NewPtr(ptr);
      Swap(NewPtr);
    }
  private:
    T *Ptr;
    unsigned long *Counter;
    template <typename> friend class SharedPtr;
  };

  template <typename T>
  class AutoPtr
  {
  public:
    explicit AutoPtr(T *ptr = 0)
      : Ptr(ptr)
    {
    }
    template <typename TOther>
    explicit AutoPtr(TOther *ptr)
      : Ptr(ptr)
    {
    }
    AutoPtr(AutoPtr &ptr)
      : Ptr(ptr.Ptr)
    {
      ptr.Ptr = 0;
    }
    template <typename TOther>
    AutoPtr(AutoPtr<TOther> &ptr)
      : Ptr(ptr.Ptr)
    {
      ptr.Ptr = 0;
    }
    AutoPtr& operator = (T *ptr)
    {
      Release();
      Ptr = ptr;
      return *this;
    }
    template <typename TOther>
    AutoPtr& operator = (TOther *ptr)
    {
      Release();
      Ptr = static_cast<T*>(ptr);
      return *this;
    }
    AutoPtr& operator = (AutoPtr &ptr)
    {
      Release();
      Ptr = ptr.Detach();
      return *this;
    }
    template <typename TOther>
    AutoPtr& operator = (AutoPtr<TOther> &ptr)
    {
      Release();
      Ptr = static_cast<T*>(ptr.Detach());
      return *this;
    }
    ~AutoPtr()
    {
      Release();
    }
    void Release()
    {
      delete Ptr;
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
    T* Get() const
    {
      return Ptr;
    }
    template <typename TOther>
    void Reset(TOther *ptr)
    {
      Release();
      Ptr = ptr;
    }
  private:
    T *Ptr;
    template <typename> friend class AutoPtr;
  };

  template <typename T>
  class ValuePtr
  {
  public:
    explicit ValuePtr(T *ptr = 0)
      : Holder(ptr ? new IHolderImpl<T> (ptr) : 0)
    {
    }
    template <typename TOther>
    explicit ValuePtr(TOther *ptr)
      : Holder(ptr ? IHolderImpl<TOther> (ptr) : 0)
    {
    }
    ValuePtr(const ValuePtr &ptr)
      : Holder(ptr.Holder ? new IHolderImpl<T> (ptr.Holder->CloneValue()) : 0)
    {
    }
    template <typename TOther>
    ValuePtr(const ValuePtr<TOther> &ptr)
      : Holder(ptr.Holder ? new IHolderImpl<T> (ptr.Holder->CloneValue()) : 0)
    {
    }
    ValuePtr& operator = (const ValuePtr &ptr)
    {
      ValuePtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    template <typename TOther>
    ValuePtr& operator = (const ValuePtr<TOther> &ptr)
    {
      ValuePtr Tmp(ptr);
      Swap(Tmp);
      return *this;
    }
    ~ValuePtr()
    {
      Release();
    }
    void Release()
    {
      delete Holder;
      Holder = 0;
    }
    void Swap(ValuePtr &ptr)
    {
      IHolder *CurHolder = Holder;
      Holder = ptr.Holder;
      ptr.Holder = CurHolder;
    }
    template <typename TOther>
    void Reset(TOther *ptr)
    {
      ValuePtr<TOther> NewPtr(ptr);
      Swap(NewPtr);
    }
    T* Get() const
    {
      IHolderImpl<T> *CurHolder = static_cast<IHolderImpl<T>*> (Holder);
      return !CurHolder ? 0 : CurHolder->Get();
    }
    T* operator -> () const
    {
      return Get();
    }
  private:
    template <typename> friend class ValuePtr;
    struct IHolder
    {
      virtual ~IHolder()
      {
      }
      virtual T* CloneValue() const = 0;
    };
    template <typename X>
    class IHolderImpl
      : public IHolder
    {
    public:
      IHolderImpl(X *ptr)
        : Ptr(ptr)
      {
      }
      ~IHolderImpl()
      {
        delete Ptr;
      }
      virtual X* CloneValue() const
      {
        return new X(*Ptr);
      }
      T* Get() const
      {
        return Ptr;
      }
    private:
      X* Ptr;
      IHolderImpl(const IHolderImpl&);
      IHolderImpl& operator =(const IHolderImpl&);
    };
    IHolder *Holder;
  };
}

#endif	// !__POINTERS_H__
