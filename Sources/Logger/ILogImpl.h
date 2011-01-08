//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __LOGIMPL_H__
#define __LOGIMPL_H__

#include "ComponentUUIDs.h"
#include "IFacesTools.h"
#include "Logger.h"


class ILogImpl
  : public Common::CoClassBase
      <
        Common::TypeListAdapter
        <
          IFaces::Log::ILog,
          IFaces::Log::ICtrl
        >
      >
{
public:
  DECLARE_STR_UUID(UUID_ILOGIMPL)

  static const IFaces::Log::Level Nothing;
  static const IFaces::Log::Level All;

  ILogImpl();
  ~ILogImpl();

  // ILog
  virtual IFaces::RetCode Write(IFaces::Log::MessageType type, char *message);

  // ICtrl
  virtual IFaces::RetCode SetStorage(IFaces::Log::IStorage *storage);
  virtual IFaces::RetCode SetName(const char *name);
  virtual IFaces::RetCode GetName(char *name, int length) const;
  virtual IFaces::RetCode SetFilterLevel(IFaces::Log::Level level);
  virtual IFaces::RetCode GetFilterLevel(IFaces::Log::Level *level) const;
  virtual IFaces::RetCode SetQueueLen(int length);
  virtual IFaces::RetCode GetQueueLen(int *length) const;
//  virtual IFaces::RetCode SetTimeFormat(const char *format);
//  virtual IFaces::RetCode GetTimeFormat(char *format, int length) const;

private:
  static const char *NAME_PREFIX;
  static const char *NAME_POSTFIX;

  IFaces::Log::IStorage *Storage;
  std::string Name;
  IFaces::Log::Level FilterLevel;
  int QueueLen;

  bool IsValidMessageType(IFaces::Log::MessageType type);
};

#endif  // !__LOGIMPL_H__
