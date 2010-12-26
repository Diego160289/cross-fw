//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBField.h"


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
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetField(unsigned long *value) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetField(double *value) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetStrField(IFaces::IVariant **value, bool asUnicode) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetField(IFaces::DBIFaces::DateTime *value) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetField(IFaces::DBIFaces::Date *value) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetField(IFaces::DBIFaces::Time *value) const
  
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IFieldImpl::GetField(bool *value) const
  
{
  return IFaces::retNotImpl;
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
