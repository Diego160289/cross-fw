//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBTransaction.h"


IFaces::RetCode ITransactionImpl::Commit()
{
  return IFaces::retNotImpl;
}

IFaces::RetCode ITransactionImpl::Rollback()
{
  return IFaces::retNotImpl;
}
