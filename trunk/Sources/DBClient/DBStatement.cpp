//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBStatement.h"
#include "DBParameter.h"
#include "DBField.h"


bool IStatementImpl::FinalizeCreate()
{
  return true;
}

void IStatementImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  Statement.Release();
  Connection.Release();
}

IFaces::RetCode IStatementImpl::Prepare(const char *str)
{
  if (!str)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    Statement->Prepare(str);
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IStatementImpl::ExecuteDirect(const char *str)
{
  if (!str)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    Statement->ExecuteDirect(str);
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IStatementImpl::Execute()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    Statement->Execute();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IStatementImpl::Fetch()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    if (!Statement->Fetch())
      return IFaces::retFalse;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IStatementImpl::GetParameter(unsigned index, IFaces::DBIFaces::IParameter **prm)
{
  if (!prm || *prm)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    Common::RefObjPtr<IParameterImpl> NewPrm = Common::IBaseImpl<IParameterImpl>::CreateWithSyn(GetSynObj());
    NewPrm->Init(Connection, Statement, index);
    return NewPrm.QueryInterface(prm);
  }
  catch (std::exception &)
  {
  }
  return IFaces::retFail;
}

IFaces::RetCode IStatementImpl::GetField(unsigned index, IFaces::DBIFaces::IField **fld) const
{
  if (!fld || *fld)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    Common::RefObjPtr<IFieldImpl> NewFld = Common::IBaseImpl<IFieldImpl>::CreateWithSyn(GetSynObj());
    NewFld->Init(Connection, Statement, index);
    return NewFld.QueryInterface(fld);
  }
  catch (std::exception &)
  {
  }
  return IFaces::retFail;
}

IFaces::RetCode IStatementImpl::GetFieldsCount(unsigned *count) const
{
  if (!count)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Statement.Get())
    return IFaces::retFail;
  try
  {
    *count = Statement->GetFieldsCount();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

void IStatementImpl::Init(Common::SharedPtr<DB::Connection> connection)
{
  Common::ISyncObject Locker(GetSynObj());
  Statement.Reset(new DB::Statement(*connection.Get()));
  Connection = connection;
}
