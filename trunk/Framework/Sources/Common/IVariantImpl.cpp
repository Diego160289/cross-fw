#include "IVariantImpl.h"

namespace IFacesImpl
{
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

  IVariantHelper::operator IFaces::IBase* () const
  {
    if (Var->IsEmpty())
      throw IVariantHelperException("Empty variant");
    if (Var->GetType() != IFaces::IVariant::vtIBase)
      throw IVariantHelperException("Bad cast");
    IFaces::IBase *Value = 0;
    if (Var->GetValueSize() != sizeof(Value))
      throw IVariantHelperException("Bad value");
    if (Var->GetValue(reinterpret_cast<void**>(&Value)) != retOk)
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

}
