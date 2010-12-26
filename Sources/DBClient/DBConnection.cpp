//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBConnection.h"


IFaces::RetCode IConnectionImpl::Connect(const char *connectionString)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IConnectionImpl::CreateTransaction(IFaces::DBIFaces::ITransaction **transaction)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IConnectionImpl::CreateStatement(IFaces::DBIFaces::IStatement **statement)
{
  return IFaces::retNotImpl;
}
