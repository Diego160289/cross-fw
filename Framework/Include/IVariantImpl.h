#ifndef __IVARIANTIMPL_H__
#define __IVARIANTIMPL_H__

#include "IFaces.h"
#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"
#include "RefObjPtr.h"
#include "Pointers.h"
#include "Typedefs.h"
#include "CommonUtils.h"

#include <string.h>

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
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

    IVariantImpl()
    {
    }
    virtual ~IVariantImpl()
    {
    }

    // IVariant
    virtual RetCode CALL_TYPE SetValue(IFaces::IVariant::VariantType type, const void *value)
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
        Holder.Reset(new IVariantHolderIBase((IFaces::IBase*)(value)));
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
        Holder.Reset(new IVariantHolderString<char, IFaces::IVariant::vtString>(reinterpret_cast<const char*>(value)));
        break;
      case IFaces::IVariant::vtWString :
        Holder.Reset(new IVariantHolderString<wchar_t, IFaces::IVariant::vtWString>(reinterpret_cast<const wchar_t*>(value)));
        break;
      default :
        return retFail;
      }
      return retOk;
    }
    virtual RetCode CALL_TYPE SetBinaryValue(const void *value, unsigned long bytes)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Holder.Reset(new IVariantHolderBinary(value, bytes));
      return retOk;
    }
    virtual RetCode CALL_TYPE GetValue(void **value) const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      if (!Holder.Get())
        return retFail;
      Holder->GetData(value);
      return retOk;
    }
    virtual unsigned long CALL_TYPE GetValueSize() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return !Holder.Get() ? 0 : Holder->GetSize();
    }
    virtual bool CALL_TYPE IsEmpty() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return !Holder.Get();
    }
    virtual IFaces::IVariant::VariantType CALL_TYPE GetType() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return Holder.Get() ? Holder->GetType() : IFaces::IVariant::vtUnknown;
    }
    virtual RetCode CALL_TYPE Clear()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Holder.Release();
      return retOk;
    }
    virtual const char* CALL_TYPE PackToBase64() const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      return !Holder.Get() ? 0 : Holder->ToBase64();
    }
    virtual RetCode CALL_TYPE FromBase64Pack(const char *pkg)
    {
      Common::CharVectorPtr Data = Common::Base64ToBin(pkg);
      IFaces::IVariant::VariantType Type = IFaces::IVariant::vtUnknown;
      size_t TypeSize = sizeof(Type);
      if (Data->size() < TypeSize)
        return retFail;
      memcpy(&Type, &Data->front(), TypeSize);
      const char *ValData = &Data->front() + TypeSize;
      if (!ValData)
        return retFail;
      size_t ValLen = Data->size() - TypeSize;
      IVariantHolderPtr NewHolder;
      switch (Type)
      {
      case IFaces::IVariant::vtBool :
        {
          if (ValLen != sizeof(bool))
            return retFail;
          bool Val = false;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<bool, IFaces::IVariant::vtBool>(Val));
        }
        break;
      case IFaces::IVariant::vtChar :
        {
          if (ValLen != sizeof(char))
            return retFail;
          char Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<char, IFaces::IVariant::vtChar>(Val));
        }
        break;
      case IFaces::IVariant::vtUChar :
        {
          if (ValLen != sizeof(unsigned char))
            return retFail;
          unsigned char Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<unsigned char, IFaces::IVariant::vtUChar>(Val));
        }
        break;
      case IFaces::IVariant::vtShort :
        {
          if (ValLen != sizeof(short))
            return retFail;
          short Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<short, IFaces::IVariant::vtShort>(Val));
        }
        break;
      case IFaces::IVariant::vtUShort :
        {
          if (ValLen != sizeof(unsigned short))
            return retFail;
          unsigned short Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<unsigned short, IFaces::IVariant::vtUShort>(Val));
        }
        break;
      case IFaces::IVariant::vtInt :
        {
          if (ValLen != sizeof(int))
            return retFail;
          int Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<int, IFaces::IVariant::vtInt>(Val));
        }
        break;
      case IFaces::IVariant::vtUInt :
        {
          if (ValLen != sizeof(unsigned))
            return retFail;
          unsigned Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<unsigned, IFaces::IVariant::vtUInt>(Val));
        }
        break;
      case IFaces::IVariant::vtLong :
        {
          if (ValLen != sizeof(long))
            return retFail;
          long Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<long, IFaces::IVariant::vtLong>(Val));
        }
        break;
      case IFaces::IVariant::vtULong :
        {
          if (ValLen != sizeof(unsigned long))
            return retFail;
          unsigned long Val = 0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<unsigned long, IFaces::IVariant::vtULong>(Val));
        }
        break;
      case IFaces::IVariant::vtFloat :
        {
          if (ValLen != sizeof(float))
            return retFail;
          float Val = 0.0f;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<float, IFaces::IVariant::vtFloat>(Val));
        }
        break;
      case IFaces::IVariant::vtDouble :
        {
          if (ValLen != sizeof(double))
            return retFail;
          double Val = 0.0;
          memcpy(&Val, ValData, ValLen);
          NewHolder.Reset(new IVariantHolderSimple<double, IFaces::IVariant::vtDouble>(Val));
        }
        break;
      case IFaces::IVariant::vtBinary :
        {
          if (ValLen < sizeof(Common::CharVector::size_type))
            return retFail;
          Common::CharVector::size_type Bytes = 0;
          memcpy(&Bytes, ValData, sizeof(Common::CharVector::size_type));
          ValData += sizeof(Common::CharVector::size_type);
          NewHolder.Reset(new IVariantHolderBinary(ValData, static_cast<unsigned long>(Bytes)));
        }
        break;
      case IFaces::IVariant::vtString :
        {
          typedef std::vector<std::string>::size_type SizeType;
          if (ValLen < sizeof(SizeType))
            return retFail;
          SizeType Bytes = 0;
          memcpy(&Bytes, ValData, sizeof(SizeType));
          ValData += sizeof(SizeType);
          NewHolder.Reset(new IVariantHolderString<char, IFaces::IVariant::vtString>(Bytes ? ValData : 0));
        }
        break;
      case IFaces::IVariant::vtWString:
        {
          typedef std::vector<std::wstring>::size_type SizeType;
          if (ValLen < sizeof(SizeType))
            return retFail;
          SizeType Bytes = 0;
          memcpy(&Bytes, ValData, sizeof(SizeType));
          ValData += sizeof(SizeType);
          NewHolder.Reset(new IVariantHolderString<wchar_t, IFaces::IVariant::vtWString>(reinterpret_cast<const wchar_t*>(Bytes ? ValData : 0)));
        }
        break;
      default :
        break;
      }
      if (!NewHolder.Get())
        return retFail;
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Holder = NewHolder;
      return retOk;
    }
  private:
    struct IVariantHolder
    {
      virtual ~IVariantHolder() {}
      virtual IFaces::IVariant::VariantType GetType() const = 0;
      virtual unsigned long GetSize() const = 0;
      virtual void GetData(void **data) const = 0;
      virtual const char* ToBase64() const = 0;
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
      virtual void GetData(void **data) const
      {
        *data = &Val;
      }
      virtual const char* ToBase64() const
      {
        std::vector<char> Buf;
        IFaces::IVariant::VariantType Vt = VarType;
        std::copy(
          &reinterpret_cast<const char*>(&Vt)[0],
          &reinterpret_cast<const char*>(&Vt)[sizeof(Vt)],
          std::back_inserter(Buf));
        std::copy(
          &reinterpret_cast<const char*>(&Val)[0],
          &reinterpret_cast<const char*>(&Val)[sizeof(Val)],
          std::back_inserter(Buf));
        Base64Buf.reset(new std::string(Common::BinToBase64(&Buf[0], static_cast<unsigned>(Buf.size()))));
        return Base64Buf->c_str();
      }
    private:
      mutable TVal Val;
      mutable Common::StringPtr Base64Buf;
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
      virtual void GetData(void **data) const
      {
        *data = Buf.empty() ? 0 : &Buf[0];
      }
      virtual const char* ToBase64() const
      {
        std::vector<char> TmpBuf;
        IFaces::IVariant::VariantType Vt = IFaces::IVariant::vtBinary;
        std::copy(
          &reinterpret_cast<const char*>(&Vt)[0],
          &reinterpret_cast<const char*>(&Vt)[sizeof(Vt)],
          std::back_inserter(TmpBuf));
        Common::CharVector::size_type Len = Buf.size();
        std::copy(
          &reinterpret_cast<const char*>(&Len)[0],
          &reinterpret_cast<const char*>(&Len)[sizeof(Len)],
          std::back_inserter(TmpBuf));
        if (Len)
          std::copy(Buf.begin(), Buf.end(), std::back_inserter(TmpBuf));
        Base64Buf.reset(new std::string(Common::BinToBase64(&TmpBuf[0], static_cast<unsigned>(TmpBuf.size()))));
        return Base64Buf->c_str();
      }
    private:
       mutable Common::CharVector Buf;
       mutable Common::StringPtr Base64Buf;
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
      virtual void GetData(void **data) const
      {
        Ptr->QueryInterface(IFaces::IBase::GetUUID(), data);
      }
      virtual const char* ToBase64() const
      {
        return 0;
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
        return static_cast<unsigned long>(sizeof(TChar) * Str.size());
      }
      virtual void GetData(void **data) const
      {
        *data = &Str[0];
      }
      virtual const char* ToBase64() const
      {
        std::vector<char> TmpBuf;
        IFaces::IVariant::VariantType Vt = VarType;
        std::copy(
          &reinterpret_cast<const char*>(&Vt)[0],
          &reinterpret_cast<const char*>(&Vt)[sizeof(Vt)],
          std::back_inserter(TmpBuf));
        typename TString::size_type StrSize = (Str.size()) * sizeof(typename TString::value_type);
        std::copy(
          &reinterpret_cast<const char*>(&StrSize)[0],
          &reinterpret_cast<const char*>(&StrSize)[sizeof(StrSize)],
          std::back_inserter(TmpBuf));
        if (StrSize)
        {
          std::copy(
            &reinterpret_cast<const char*>(&Str[0])[0],
            &reinterpret_cast<const char*>(&Str[0])[StrSize],
            std::back_inserter(TmpBuf));
        }
        Base64Buf.reset(new std::string(Common::BinToBase64(&TmpBuf[0], static_cast<unsigned>(TmpBuf.size()))));
        return Base64Buf->c_str();
      }
    private:
      typedef std::vector<TChar> TString;
      mutable TString Str;
      mutable Common::StringPtr Base64Buf;
    };
    typedef Common::SharedPtr<IVariantHolder> IVariantHolderPtr;
    IVariantHolderPtr Holder;
  };

  template <typename TSyn>
  typename IVariantImpl<Common::MultiObject, TSyn>::ThisTypePtr
  CreateVariant()
  {
    typename IVariantImpl<Common::MultiObject, TSyn>::ThisTypePtr Ret =
      IVariantImpl<Common::MultiObject, TSyn>::CreateObject();
    return Ret;
  }

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
