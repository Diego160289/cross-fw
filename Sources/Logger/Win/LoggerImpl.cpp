//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#include "LoggerImpl.h"


IFaces::RetCode ILoggerImpl::Write(IFaces::LoggerIFaces::LogLevel level, wchar_t *message)
{
  return IFaces::retNotImpl;
}

bool ILoggerImpl::FinalizeCreate()
{
  return true;
}

void ILoggerImpl::BeforeDestroy()
{
}