#ifndef __VARIANT_H__
#define __VARIANT_H__

namespace Common
{
  class Variant
  {
  public:
    Variant() :
      Holder(0)
    {
    }
    template <typename T>
    Variant(const T &value)
      : Holder(new IHolderImpl<T> (value))
    {
    }
    Variant(const Variant &var)
      : Holder(var.Holder ? var.Holder->Clone() : 0)
    {
    }
    Variant& operator =(const Variant &var)
    {
      Variant Tmp(var);
      Swap(Tmp);
      return *this;
    }
    template <typename T>
    Variant& operator =(const T &value)
    {
      Variant Tmp(value);
      Swap(Tmp);
      return *this;
    }
    ~Variant()
    {
      delete Holder;
    }
    void Swap(Variant &var)
    {
      IHolder *CurHolder = Holder;
      Holder = var.Holder;
      var.Holder = CurHolder;
    }
    template <typename T>
    operator T() const
    {
      if (!Holder)
        return T();
      IHolderImpl<T> & CurHolder = dynamic_cast<IHolderImpl<T>&> (*Holder);
      return CurHolder;
    }
    template <typename T>
    bool IsEqualTypes() const
    {
      return !Holder ? true : !!dynamic_cast<IHolderImpl<T>*> (Holder);
    }
  private:
    struct IHolder
    {
      virtual ~IHolder()
      {
      }
      virtual IHolder* Clone() const = 0;
    };
    template <typename T>
    class IHolderImpl
      : public IHolder
    {
    public:
      IHolderImpl(const T &value)
        : Value(value)
      {
      }
      virtual IHolder* Clone() const
      {
        return new IHolderImpl<T> (Value);
      }
      operator T() const
      {
        return Value;
      }
    private:
      IHolderImpl(const IHolderImpl&);
      IHolderImpl& operator =(const IHolderImpl&);
      T Value;
    };
    IHolder *Holder;
  };
}

#endif	// !__VARIANT_H__
