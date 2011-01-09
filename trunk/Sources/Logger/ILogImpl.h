//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __LOGIMPL_H__
#define __LOGIMPL_H__

#include "ComponentUUIDs.h"
#include "IFacesTools.h"
#include "IStorageHelper.h"

#include "Logger.h"

#include <queue>


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
  virtual ~ILogImpl();

  // ILog
  virtual IFaces::RetCode Write(IFaces::Log::MessageType type, char *message);
  virtual IFaces::RetCode Flush();

  // ICtrl
  virtual IFaces::RetCode SetStorage(IFaces::IStorage *storage);
  virtual IFaces::RetCode SetName(const char *name);
  virtual IFaces::RetCode GetName(char *name, int length) const;
  virtual IFaces::RetCode SetFilterLevel(IFaces::Log::Level level);
  virtual IFaces::RetCode GetFilterLevel(IFaces::Log::Level *level) const;
  virtual IFaces::RetCode SetQueueLen(int length);
  virtual IFaces::RetCode GetQueueLen(int *length) const;
//  virtual IFaces::RetCode SetTimeFormat(const char *format);
//  virtual IFaces::RetCode GetTimeFormat(char *format, int length) const;

private:
  typedef std::queue<std::string> LogItemsQueue;
  static const LogItemsQueue::size_type DEFAULT_QUEUE_MAX_LEN = 3;
  static const char *NAME_PREFIX;
  static const char *NAME_POSTFIX;

  Common::SharedPtr<IFacesImpl::IStorageHelper> Storage;
  std::string Name;
  IFaces::Log::Level FilterLevel;
  LogItemsQueue LogItems;
  LogItemsQueue::size_type QueueMaxLen;

  bool IsValidMessageType(IFaces::Log::MessageType type);
  IFacesImpl::IStorageHelper::IStreamPtr GetStream();
};

#endif  // !__LOGIMPL_H__
