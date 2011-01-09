//============================================================================
// Date        : 05.01.2011
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
  virtual IFaces::RetCode SetName(const char *name, const char *namePrefix = "", const char *namePostfix = ".log");
  virtual IFaces::RetCode GetName(IFaces::IVariant **name) const;
  virtual IFaces::RetCode SetFilterLevel(IFaces::Log::Level level);
  virtual IFaces::RetCode GetFilterLevel(IFaces::Log::Level *level) const;
  virtual IFaces::RetCode SetQueueLen(int length);
  virtual IFaces::RetCode GetQueueLen(int *length) const;

  virtual void BeforeDestroy();

private:
  typedef std::queue<std::string> LogItemsQueue;
  static const LogItemsQueue::size_type DEFAULT_QUEUE_MAX_LEN = 3;

  Common::SharedPtr<IFacesImpl::IStorageHelper> Storage;
  std::string Name;
  IFaces::Log::Level FilterLevel;
  LogItemsQueue LogItems;
  LogItemsQueue::size_type QueueMaxLen;

  bool IsValid() const;
  bool IsValidMessageType(IFaces::Log::MessageType type) const;
  IFacesImpl::IStorageHelper::IStreamPtr GetStream() const;
};

#endif  // !__LOGIMPL_H__
