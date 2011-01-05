//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "IFaces.h"


namespace IFaces
{

  namespace LoggerIFaces
  {

    enum LogLevel
    {
      Error = 0,
      Warning = 32,
      Info = 64
    };

    typedef LogLevel LogMessageType;

    struct ILoggerFileCtrl
      : public IBase
    {
      DECLARE_UUID(6669b9e0-698f-466e-93dc-1cb6fa1ee2f4)
    };

    struct ILoggerCtrl
      : public IBase
    {
      DECLARE_UUID(3165b9c9-2f62-44be-85a7-58aae2b2a6bc)
    };

    struct ILogger
      : public IBase
    {
      DECLARE_UUID(2cbd0d25-a8a1-4006-9430-4ecc337789b0)
      virtual RetCode Write(LogLevel level, wchar_t *message) = 0;
    };

  }

}

#endif  // !__LOGGER_H__
