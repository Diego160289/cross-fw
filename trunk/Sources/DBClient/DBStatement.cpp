//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBStatement.h"


IFaces::RetCode IStatementImpl::Prepare(const char *str)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::ExecuteDirect(const char *str)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::Execute(const char *str)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::Fetch()
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::GetParameter(IFaces::DBIFaces::IParameter **prm)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::GetParameter(IFaces::DBIFaces::IParameter **prm) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::GetField(unsigned index, IFaces::DBIFaces::IField **fld) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode IStatementImpl::GetFieldsCount(unsigned *count) const
{
  return IFaces::retNotImpl;
}
