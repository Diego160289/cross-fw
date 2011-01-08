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

    struct IStream
      : public IBase
    {
      DECLARE_UUID(ba1628d2-44d2-4471-a8c9-1d4ef7ab249e)

      //Required for use: SetName().
      virtual RetCode SetName(const char *name) = 0;
      virtual RetCode Create() = 0;
      virtual RetCode Open() = 0;
      virtual RetCode Write(MessageType type, char *message) = 0;
      virtual RetCode Flush() = 0;
    };

    struct IStorage
      : public IBase
    {
      DECLARE_UUID(6669b9e0-698f-466e-93dc-1cb6fa1ee2f4)

      virtual RetCode GetStream(IStream **stream, const char *name, const char *namePrefix = "", const char *namePostfix = "") = 0;
    };

    struct ICtrl
      : public IBase
    {
      DECLARE_UUID(3165b9c9-2f62-44be-85a7-58aae2b2a6bc)

      //Required for use: SetStorage(), SetName().
      virtual RetCode SetStorage(IStorage *storage) = 0;
      virtual RetCode SetName(const char *name) = 0;
      virtual RetCode GetName(char *name, int length) const = 0;
      virtual RetCode SetFilterLevel(Level level) = 0;
      virtual RetCode GetFilterLevel(Level *level) const = 0;
      virtual RetCode SetQueueLen(int length) = 0;
      virtual RetCode GetQueueLen(int *length) const = 0;
//      virtual RetCode SetTimeFormat(const char *format) = 0;
//      virtual RetCode GetTimeFormat(char *format, int length) = 0;
    };

    struct ILog
      : public IBase
    {
      DECLARE_UUID(2cbd0d25-a8a1-4006-9430-4ecc337789b0)

      virtual RetCode Write(MessageType type, char *message) = 0;
    };

  }

}

#endif  // !__LOGGER_H__
