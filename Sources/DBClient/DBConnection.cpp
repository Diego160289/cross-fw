//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBConnection.h"
#include "DBTransaction.h"
#include "DBStatement.h"


bool IConnectionImpl::FinalizeCreate()
{
  return true;
}

void IConnectionImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  Connection.Release();
}

IFaces::RetCode IConnectionImpl::Connect(const char *connectionString,
                                         const char *userName,
                                         const char *password)
{
  if (!connectionString)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (Connection.Get())
    return IFaces::retFalse;
  try
  {
    Connection.Reset(new DB::Connection(connectionString, userName ? userName : "", password ? password : ""));
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode IConnectionImpl::Disconnect()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Connection.Get())
    return IFaces::retFalse;
  Connection.Release();
  return IFaces::retOk;
}

IFaces::RetCode IConnectionImpl::CreateTransaction(IFaces::DBIFaces::ITransaction **transaction)
{
  if (!transaction || *transaction)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Connection.Get())
    return IFaces::retFail;
  try
  {
    Common::RefObjPtr<ITransactionImpl> NewTr = Common::IBaseImpl<ITransactionImpl>::CreateWithSyn(GetSynObj());
    NewTr->Init(Connection);
    return NewTr.QueryInterface(transaction);
  }
  catch (std::exception &)
  {
  }
  return IFaces::retFail;
}

IFaces::RetCode IConnectionImpl::CreateStatement(IFaces::DBIFaces::IStatement **statement)
{
  if (!statement || *statement)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Connection.Get())
    return IFaces::retFail;
  try
  {
    Common::RefObjPtr<IStatementImpl> NewStmt = Common::IBaseImpl<IStatementImpl>::CreateWithSyn(GetSynObj());
    NewStmt->Init(Connection);
    return NewStmt.QueryInterface(statement);
  }
  catch (std::exception &)
  {
  }
  return IFaces::retFail;
}
