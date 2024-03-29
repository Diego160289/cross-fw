//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "IVariantImpl.h"


namespace IFacesImpl
{

  struct IVariantImpl::IVariantHolder
  {
    virtual ~IVariantHolder() {}
    virtual IFaces::IVariant::VariantType GetType() const = 0;
    virtual unsigned long GetSize() const = 0;
    virtual void GetData(void **data) const = 0;
    virtual const char* ToBase64() const = 0;
  };

  template <typename TVal, IFaces::IVariant::VariantType VarType>
  class IVariantHolderSimple
    : public IVariantImpl::IVariantHolder
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
    : public IVariantImpl::IVariantHolder
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
    : public IVariantImpl::IVariantHolder
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
    : public IVariantImpl::IVariantHolder
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
      if (Str.size())
      {
        union
        {
          TChar Val;
          char Buf[sizeof(TChar)];
        } Ch;
        for (typename TString::const_iterator i = Str.begin() ; i != Str.end() ; ++i)
        {
          Ch.Val = *i;
          for (unsigned j = 0 ; j < sizeof(TChar) ; ++j)
            TmpBuf.push_back(Ch.Buf[j]);
        }
      }
      Base64Buf.reset(new std::string(Common::BinToBase64(
        &TmpBuf[0], static_cast<unsigned>(TmpBuf.size() * sizeof(typename TString::size_type)))));
      return Base64Buf->c_str();
    }
  private:
    typedef std::vector<TChar> TString;
    mutable TString Str;
    mutable Common::StringPtr Base64Buf;
  };

  IVariantImpl::IVariantImpl()
  {
  }

  IVariantImpl::~IVariantImpl()
  {
  }

  RetCode IVariantImpl::SetValue(IFaces::IVariant::VariantType type, const void *value)
  {
    Common::ISyncObject Locker(GetSynObj());
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

  RetCode IVariantImpl::SetBinaryValue(const void *value, unsigned long bytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    Holder.Reset(new IVariantHolderBinary(value, bytes));
    return retOk;
  }

  RetCode IVariantImpl::GetValue(void **value) const
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!Holder.Get())
      return retFail;
    Holder->GetData(value);
    return retOk;
  }

  unsigned long IVariantImpl::GetValueSize() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return !Holder.Get() ? 0 : Holder->GetSize();
  }

  bool IVariantImpl::IsEmpty() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return !Holder.Get();
  }

  IFaces::IVariant::VariantType IVariantImpl::GetType() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Holder.Get() ? Holder->GetType() : IFaces::IVariant::vtUnknown;
  }

  RetCode IVariantImpl::Clear()
  {
    Common::ISyncObject Locker(GetSynObj());
    Holder.Release();
    return retOk;
  }

  const char* IVariantImpl::PackToBase64() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return !Holder.Get() ? 0 : Holder->ToBase64();
  }

  RetCode IVariantImpl::FromBase64Pack(const char *pkg)
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
        NewHolder.Reset(new IVariantHolderString<char, IFaces::IVariant::vtString>(ValData));
      }
      break;
    case IFaces::IVariant::vtWString:
      {
        typedef std::vector<std::wstring>::size_type SizeType;
        if (ValLen < sizeof(SizeType))
          return retFail;
        NewHolder.Reset(new IVariantHolderString<wchar_t, IFaces::IVariant::vtWString>(reinterpret_cast<const wchar_t*>(ValData)));
      }
      break;
    default :
      break;
    }
    if (!NewHolder.Get())
      return retFail;
    Common::ISyncObject Locker(GetSynObj());
    Holder = NewHolder;
    return retOk;
  }


  Common::RefObjPtr<IVariantImpl>
  CreateVariant(const Common::ISynObj &syn)
  {
    return Common::IBaseImpl<IVariantImpl>::CreateWithSyn(syn);
  }


  IVariantHelper::IVariantHelper(IVariantPtr var)
    : Var(var)
  {
  }

  IVariantHelper::~IVariantHelper()
  {
  }

  IVariantHelper& IVariantHelper::operator = (bool value)
  {
    Var->SetValue(IFaces::IVariant::vtBool, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::SetBinaryData(const void *data, unsigned long bytes)
  {
    Var->SetBinaryValue(data, bytes);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (IFaces::IBase *ptr)
  {
    Var->SetValue(IFaces::IVariant::vtIBase, ptr);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (char value)
  {
    Var->SetValue(IFaces::IVariant::vtChar, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (unsigned char value)
  {
    Var->SetValue(IFaces::IVariant::vtUChar, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (short value)
  {
    Var->SetValue(IFaces::IVariant::vtShort, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (unsigned short value)
  {
    Var->SetValue(IFaces::IVariant::vtUShort, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (int value)
  {
    Var->SetValue(IFaces::IVariant::vtInt, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (unsigned int value)
  {
    Var->SetValue(IFaces::IVariant::vtUInt, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (long value)
  {
    Var->SetValue(IFaces::IVariant::vtLong, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (unsigned long value)
  {
    Var->SetValue(IFaces::IVariant::vtULong, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (float value)
  {
    Var->SetValue(IFaces::IVariant::vtFloat, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (double value)
  {
    Var->SetValue(IFaces::IVariant::vtDouble, &value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (const char *value)
  {
    Var->SetValue(IFaces::IVariant::vtString, value);
    return *this;
  }

  IVariantHelper& IVariantHelper::operator = (const wchar_t *value)
  {
    Var->SetValue(IFaces::IVariant::vtWString, value);
    return *this;
  }

  IVariantHelper::operator const bool () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtBool)
      throw IVariantHelperException("Bad cast");
    bool *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const void* () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtBinary)
      throw IVariantHelperException("Bad cast");
    void *Value = 0;
    if (!Var->GetValueSize())
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(&Value) != retOk)
      throw IVariantHelperException("Can't get value");
    return Value;
  }

  IVariantHelper::operator Common::RefObjPtr<IFaces::IBase> () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtIBase)
      throw IVariantHelperException("Bad cast");
    Common::RefObjPtr<IFaces::IBase> Value;
    if (Var->GetValueSize() != sizeof(Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(Value.GetPPtr())) != retOk)
      throw IVariantHelperException("Can't get value");
    return Value;
  }

  IVariantHelper::operator const char () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtChar)
      throw IVariantHelperException("Bad cast");
    char *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const unsigned char () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtUChar)
      throw IVariantHelperException("Bad cast");
    unsigned char *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const short () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtShort)
      throw IVariantHelperException("Bad cast");
    short *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const unsigned short () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtUShort)
      throw IVariantHelperException("Bad cast");
    unsigned short *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const int () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtInt)
      throw IVariantHelperException("Bad cast");
    int *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const unsigned int () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtUInt)
      throw IVariantHelperException("Bad cast");
    unsigned *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const long () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtLong)
      throw IVariantHelperException("Bad cast");
    long *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const unsigned long () const

  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtULong)
      throw IVariantHelperException("Bad cast");
    unsigned long *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const float () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtFloat)
      throw IVariantHelperException("Bad cast");
    float *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const double () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtDouble)
      throw IVariantHelperException("Bad cast");
    double *Value = 0;
    if (Var->GetValueSize() != sizeof(*Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return *Value;
  }

  IVariantHelper::operator const char* () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtString)
      throw IVariantHelperException("Bad cast");
    char *Value = 0;
    if (!Var->GetValueSize())
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return Value;
  }

  IVariantHelper::operator const wchar_t* () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtWString)
      throw IVariantHelperException("Bad cast");
    wchar_t *Value = 0;
    if (!Var->GetValueSize())
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
      throw IVariantHelperException("Can't get value");
    return Value;
  }

  IFaces::IVariant::VariantType IVariantHelper::GetType() const
  {
    return Var->GetType();
  }

  bool IVariantHelper::IsEmpty() const
  {
    return Var->IsEmpty();
  }

  unsigned long IVariantHelper::GetDataSize() const
  {
    return Var->GetValueSize();
  }

  IVariantHelper::IVariantPtr IVariantHelper::GetVariant() const
  {
    return Var;
  }

}
