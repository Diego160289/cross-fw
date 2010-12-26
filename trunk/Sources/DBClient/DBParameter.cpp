//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBParameter.h"
#include "SystemUtils.h"

#include <sstream>
#include <iomanip>


IParameterImpl::IParameterImpl()
  : Index(0)
{
}

bool IParameterImpl::FinalizeCreate()
{
  return true;
}

void IParameterImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  Statement.Release();
  Connection.Release();
}

void IParameterImpl::SetParam(long value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Statement->GetParameter(Index).SetParam(value);
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(unsigned long value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Statement->GetParameter(Index).SetParam(value);
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(double value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Statement->GetParameter(Index).SetParam(value);
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(const char *value)
{
  if (!value)
    return;
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Statement->GetParameter(Index).SetParam(value);
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(const wchar_t *value)
{
  if (!value)
    return;
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Statement->GetParameter(Index).SetParam(System::WStringToAString(value).c_str());
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(const IFaces::DBIFaces::DateTime &value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    std::tm Tm = { 0 };
    Tm.tm_year = value.Dt.Year - 1900;
    Tm.tm_mon = value.Dt.Month;
    Tm.tm_mday = value.Dt.Day;
    Tm.tm_hour = value.Tm.Hour;
    Tm.tm_min = value.Tm.Min;
    Tm.tm_sec = value.Tm.Sec;
    Statement->GetParameter(Index).SetParam(Tm);
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(const IFaces::DBIFaces::Date &value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    std::stringstream Io;
    Io
      << std::setw(4) << std::setfill('0') << value.Year << "-"
      << std::setw(2) << std::setfill('0') << value.Month << "-"
      << std::setw(2) << std::setfill('0') << value.Day;
    Statement->GetParameter(Index).SetParam(Io.str().c_str());
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(const IFaces::DBIFaces::Time &value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    std::stringstream Io;
    Io
      << std::setw(2) << std::setfill('0') << value.Hour << ":"
      << std::setw(2) << std::setfill('0') << value.Min << "-"
      << std::setw(2) << std::setfill('0') << value.Sec;
    Statement->GetParameter(Index).SetParam(Io.str().c_str());
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::SetParam(bool value)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Statement->GetParameter(Index).SetParam(value);
  }
  catch (std::exception &)
  {
  }
}

void IParameterImpl::Init(Common::SharedPtr<DB::Connection> connection,
                      Common::SharedPtr<DB::Statement> statement,
                      unsigned index)
{
  Common::ISyncObject Locker(GetSynObj());
  statement->GetParameter(index);
  Statement = statement;
  Connection = connection;
  Index = index;
}
