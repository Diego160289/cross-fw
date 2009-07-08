#ifndef __IVARIANTIMPL_H__
#define __IVARIANTIMPL_H__

#include "IFaces.h"
#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"
#include "RefObjPtr.h"
#include "Pointers.h"


#include <vector>

namespace IFacesImpl
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  template
  <
    template <typename, typename > class TCreateStrategy = Common::MultiObject,
    typename TSynObj = System::MutexStub
  >
  class IVariantImpl
    : public Common::CoClassBase
        <
          IVariantImpl<TCreateStrategy, TSynObj>,
          TYPE_LIST_1(IFaces::IVariant),
          TCreateStrategy, TSynObj
        >
  {
  public:
    DECLARE_UUID(0b9550da-6466-4fd4-9a43-40901551f727)

    typedef IVariantImpl<TCreateStrategy, TSynObj> ThisType;

    IVariantImpl()
    {
    }
    virtual ~IVariantImpl()
    {
    }

    // IVariant
    virtual RetCode SetValue(IFaces::IVariant::VariantType type, const void *value)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      switch (type)
      {
      case IFaces::IVariant::vtUnknown :
        Holder.Release();
        break;
      case IFaces::IVariant::vtBool :
        Holder.Reset(new IVariantHolderSimple<bool, IFaces::IVariant::vtBool>(*reinterpret_cast<const bool*>(value)));
        break;
      case IFaces::IVariant::vtIBase :
        Holder.Reset(new IVariantHolderIBase(*reinterpret_cast<IFaces::IBase*>(value)));
        break;
      case IFaces::IVariant::vtChar :
        Holder.Reset(new IVariantHolderSimple<char, IFaces::IVariant::vtChar>(*reinterpret_cast<const char*>(value)));
        break;
      case IFaces::IVariant::vtUChar :
        Holder.Reset(new IVariantHolderSimple<unsigned char, IFaces::IVariant::vtUChar>(*reinterpret_cast<const unsigned char*>(value)));
        break;
      case IFaces::IVariant::vtShort :
        Holder.Reset(new IVariantHolderSimple<short, IFaces::IVariant::vtShort>(*reinterpret_cast<const short*>(value)));
        break;
      case IFaces::IVariant::vtUShort :
        Holder.Reset(new IVariantHolderSimple<unsigned short, IFaces::IVariant::vtUShort>(*reinterpret_cast<const unsigned short*>(value)));
        break;
      case IFaces::IVariant::vtInt :
        Holder.Reset(new IVariantHolderSimple<int, IFaces::IVariant::vtInt>(*reinterpret_cast<const int*>(value)));
        break;
      case IFaces::IVariant::vtUInt :
        Holder.Reset(new IVariantHolderSimple<unsigned, IFaces::IVariant::vtUInt>(*reinterpret_cast<const unsigned*>(value)));
        break;
      case IFaces::IVariant::vtLong :
        Holder.Reset(new IVariantHolderSimple<long, IFaces::IVariant::vtLong>(*reinterpret_cast<const long*>(value)));
        break;
      case IFaces::IVariant::vtULong :
        Holder.Reset(new IVariantHolderSimple<unsigned long, IFaces::IVariant::vtULong>(*reinterpret_cast<const unsigned long*>(value)));
        break;
      case IFaces::IVariant::vtFloat :
        Holder.Reset(new IVariantHolderSimple<float, IFaces::IVariant::vtFloat>(*reinterpret_cast<const float*>(value)));
        break;
      case IFaces::IVariant::vtDouble :
        Holder.Reset(new IVariantHolderSimple<double, IFaces::IVariant::vtDouble>(*reinterpret_cast<const double*>(value)));
        break;
      case IFaces::IVariant::vtString :
        Holder.Reset(new IVariantHolderString<char, IFaces::IVariant::vtString>(*reinterpret_cast<const char*>(value)));
        break;
      case IFaces::IVariant::vtWString :
        Holder.Reset(new IVariantHolderString<wchar_t, IFaces::IVariant::vtWString>(*reinterpret_cast<const wchar_t*>(value)));
        break;
      default :
        return retFail;
      }
      return retOk;
    }
    virtual RetCode SetBinaryValue(const void *value, unsigned long bytes)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Holder.Reset(new IVariantHolderBinary(value, bytes));
      return retOk;
    }
    virtual RetCode GetValue(void **value) const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      if (!Holder.Get())
        return retFail;
      Holder->GetData(value);
      return retOk;
    }
    virtual unsigned long GetValueSize() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return !Holder.Get() ? 0 : Holder->GetSize();
    }
    virtual bool IsEmpty() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return !Holder.Get();
    }
    virtual IFaces::IVariant::VariantType GetType() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return Holder.Get() ? Holder->GetType() : IFaces::IVariant::vtUnknown;
    }
    virtual RetCode Clear()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Holder.Release();
      return retOk;
    }
  private:
    struct IVariantHolder
    {
      virtual ~IVariantHolder() {}
      virtual IFaces::IVariant::VariantType GetType() const = 0;
      virtual unsigned long GetSize() const = 0;
      virtual void GetData(void **data) const = 0;
    };
    template <typename TVal, IFaces::IVariant::VariantType VarType>
    class IVariantHolderSimple
      : public IVariantHolder
      , private Common::NoCopyable
    {
    public:
      IVariantHolderSimple(const TVal &val)
        : Val(val)
      {
      }
      virtual IFaces::IVariant::VariantType GetType() const
      {
        return VarType;
      }
      virtual unsigned long GetSize() const
      {
        return sizeof(TVal);
      }
      virtual void GetData(void **data)
      {
        *data = &Val;
      }
    private:
      TVal Val;
    };
    class IVariantHolderBinary
      : public IVariantHolder
      , private Common::NoCopyable
    {
    public:
      IVariantHolderBinary(const void *val, unsigned long bytes)
        : Buf(&reinterpret_cast<const char*>(val)[0],
              &reinterpret_cast<const char*>(val)[bytes])
      {
      }
      virtual IFaces::IVariant::VariantType GetType() const
      {
        return IFaces::IVariant::vtBinary;
      }
      virtual unsigned long GetSize() const
      {
        return static_cast<unsigned long>(Buf.size());
      }
      virtual void GetData(void **data)
      {
        *data = Buf.empty() ? 0 : &Buf[0];
      }
    private:
      typedef std::vector<char> BufType;
      BufType Buf;
    };
    class IVariantHolderIBase
      : public IVariantHolder
      , private Common::NoCopyable
    {
    public:
      IVariantHolderIBase(IFaces::IBase *ptr)
        : Ptr(ptr)
      {
      }
      virtual IFaces::IVariant::VariantType GetType() const
      {
        return IFaces::IVariant::vtIBase;
      }
      virtual unsigned long GetSize() const
      {
        return sizeof(IFaces::IBase*);
      }
      virtual void GetData(void **data)
      {
        return Ptr.QueryInterface(reinterpret_cast<IFaces::IBase**>(data));
      }
    private:
      typedef Common::RefObjPtr<IFaces::IBase> IBasePtr;
      IBasePtr Ptr;
    };
    template <typename TChar, IFaces::IVariant::VariantType VarType>
    class IVariantHolderString
      : public IVariantHolder
      , private Common::NoCopyable
    {
    public:
      IVariantHolderString(const TChar *str)
      {
        for ( ; *str ; ++str)
          Str.push_back(*str);
        Str.push_back(0);
      }
      virtual IFaces::IVariant::VariantType GetType() const
      {
        return VarType;
      }
      virtual unsigned long GetSize() const
      {
        return sizeof(TChar) * (Str.length() + 1);
      }
      virtual void GetData(void **data)
      {
        *data = &Str[0];
      }
    private:
      typedef std::vector<TChar> TString;
      TString Str;
    };
    typedef Common::SharedPtr<IVariantHolder> IVariantHolderPtr;
    IVariantHolderPtr Holder;
  };

  DECLARE_RUNTIME_EXCEPTION(IVariantHelper)

  class IVariantHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IVariant> IVariantPtr;

    IVariantHelper(IVariantPtr var);
    ~IVariantHelper();

    IVariantHelper& operator = (bool value);
    IVariantHelper& SetBinaryData(const void *data, unsigned long bytes);
    IVariantHelper& operator = (IFaces::IBase *ptr);
    IVariantHelper& operator = (char value);
    IVariantHelper& operator = (unsigned char value);
    IVariantHelper& operator = (short value);
    IVariantHelper& operator = (unsigned short value);
    IVariantHelper& operator = (int value);
    IVariantHelper& operator = (unsigned int value);
    IVariantHelper& operator = (long value);
    IVariantHelper& operator = (unsigned long value);
    IVariantHelper& operator = (float value);
    IVariantHelper& operator = (double value);
    IVariantHelper& operator = (const char *value);
    IVariantHelper& operator = (const wchar_t *value);

    operator const bool () const;
    operator const void* () const;
    operator IFaces::IBase* () const;
    operator const char () const;
    operator const unsigned char () const;
    operator const short () const;
    operator const unsigned short () const;
    operator const int () const;
    operator const unsigned int () const;
    operator const long () const;
    operator const unsigned long () const;
    operator const float () const;
    operator const double () const;
    operator const char* () const;
    operator const wchar_t* () const;

    IFaces::IVariant::VariantType GetType() const;
    bool IsEmpty() const;
    unsigned long GetDataSize() const;
  private:
    IVariantPtr Var;
  };
}

#endif  // !__IVARIANTIMPL_H__
