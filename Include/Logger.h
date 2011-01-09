//============================================================================
// Date        : 05.01.2011
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "IFaces.h"


namespace IFaces
{

  namespace Log
  {

    enum MessageType
    {
      Error = 0,
      Warning = 32,
      Info = 64,

      MessageTypeMaxValue = 96
    };

    //Log detail level.
    typedef MessageType Level;

    struct ICtrl
      : public IBase
    {
      DECLARE_UUID(3165b9c9-2f62-44be-85a7-58aae2b2a6bc)

      //Required for use: SetStorage(), SetName().
      virtual RetCode SetStorage(IStorage *storage) = 0;
      virtual RetCode SetName(const char *name, const char *namePrefix = "", const char *namePostfix = "") = 0;
      virtual RetCode GetName(IVariant **name) const = 0;
      virtual RetCode SetFilterLevel(Level level) = 0;
      virtual RetCode GetFilterLevel(Level *level) const = 0;
      virtual RetCode SetQueueLen(int length) = 0;
      virtual RetCode GetQueueLen(int *length) const = 0;
    };

    struct ILog
      : public IBase
    {
      DECLARE_UUID(2cbd0d25-a8a1-4006-9430-4ecc337789b0)

      virtual RetCode Write(MessageType type, char *message) = 0;
      virtual RetCode Flush() = 0;
    };

  }

}

#endif  // !__LOGGER_H__
