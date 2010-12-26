//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBParameter.h"


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
}

void IParameterImpl::SetParam(unsigned long value)
{
}

void IParameterImpl::SetParam(double value)
{
}

void IParameterImpl::SetParam(const char *value)
{
}

void IParameterImpl::SetParam(const wchar_t *value)
{
}

void IParameterImpl::SetParam(const IFaces::DBIFaces::DateTime &value)
{
}

void IParameterImpl::SetParam(const IFaces::DBIFaces::Date &value)
{
}

void IParameterImpl::SetParam(const IFaces::DBIFaces::Time &value)
{
}

void IParameterImpl::SetParam(bool value)
{
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
