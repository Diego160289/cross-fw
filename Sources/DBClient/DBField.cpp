//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBField.h"

#include "CommonUtils.h"
#include "SystemUtils.h"
#include "IVariantImpl.h"


IFieldImpl::IFieldImpl()
{
}

bool IFieldImpl::FinalizeCreate()
{
  return true;
}

void IFieldImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  Field.Release();
  Statement.Release();
  Connection.Release();
}

IFaces::RetCode IFieldImpl::GetField(long *value) const
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    *value = Field->GetAsLong();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetField(unsigned long *value) const
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    *value = Field->GetAsULong();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetField(double *value) const
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    *value = Field->GetAsDouble();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetStrField(IFaces::IVariant **value, bool asUnicode) const
{
  if (!value || *value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    std::string Value = Field->GetAsString();
    Common::RefObjPtr<IFaces::IVariant> NewVar = IFacesImpl::CreateVariant(GetSynObj());
    if (asUnicode)
      (IFacesImpl::IVariantHelper(NewVar)) = System::AStringToWString(Value).c_str();
    else
      (IFacesImpl::IVariantHelper(NewVar)) = Value.c_str();
    return NewVar.QueryInterface(value);
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetField(IFaces::DBIFaces::DateTime *value) const
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    std::tm Tm = Field->GetAsTimestamp();
    value->Dt.Year = Tm.tm_year + 1900;
    value->Dt.Month = Tm.tm_mon;
    value->Dt.Day = Tm.tm_mday;
    value->Tm.Hour = Tm.tm_hour;
    value->Tm.Hour = Tm.tm_min;
    value->Tm.Hour = Tm.tm_sec;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetField(IFaces::DBIFaces::Date *value) const
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    Common::StringVectorPtr Items = Common::SplitString(Field->GetAsString(), ':');
    if (!Items.get() || Items->size() != 3)
      return IFaces::retFail;
    value->Year = Common::FromString<unsigned short>((*Items.get())[0]);
    value->Month = Common::FromString<unsigned short>((*Items.get())[1]);
    value->Day = Common::FromString<unsigned short>((*Items.get())[2]);
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetField(IFaces::DBIFaces::Time *value) const
  
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    DB::TimeStruct Tm = Field->GetAsTime();
    value->Hour = Tm.Hour;
    value->Min = Tm.Min;
    value->Sec = Tm.Sec;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IFieldImpl::GetField(bool *value) const
  
{
  if (!value)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Field.Get())
    return IFaces::retFail;
  try
  {
    *value = Field->GetAsBool();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

void IFieldImpl::Init(Common::SharedPtr<DB::Connection> connection,
                      Common::SharedPtr<DB::Statement> statement,
                      unsigned index)
{
  Common::ISyncObject Locker(GetSynObj());
  Field = statement->GetField(index);
  Statement = statement;
  Connection = connection;
}
