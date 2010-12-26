//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBTransaction.h"


bool ITransactionImpl::FinalizeCreate()
{
  return true;
}

void ITransactionImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  Transaction.Release();
  Connection.Release();
}

IFaces::RetCode ITransactionImpl::Commit()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Transaction.Get())
    return IFaces::retFail;
  try
  {
    Transaction->Commit();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode ITransactionImpl::Rollback()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Transaction.Get())
    return IFaces::retFail;
  try
  {
    Transaction->Rollback();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

void ITransactionImpl::Init(Common::SharedPtr<DB::Connection> connection)
{
  Common::ISyncObject Locker(GetSynObj());
  Transaction.Reset(new DB::Transaction(*connection.Get()));
  Connection = connection;
}
