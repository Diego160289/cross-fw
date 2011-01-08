//============================================================================
// Date        : 07.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __ISTREAMIMPL_H__
#define __ISTREAMIMPL_H__

#include "IFacesTools.h"
#include "Logger.h"
#include "IStorageHelper.h"
#include "IStreamHelper.h"

#include <queue>


// Private implementation.
#define UUID_ISTREAMIMPL "4a9be03b-dbb7-49a7-bf2f-9160debd0bde"

class IStreamImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::Log::IStream)
      >
{
public:
  DECLARE_STR_UUID(UUID_ISTREAMIMPL)

  IStreamImpl();
  ~IStreamImpl();

  // IStream
  virtual IFaces::RetCode SetName(const char *name);
  virtual IFaces::RetCode Create();
  virtual IFaces::RetCode Open();
  virtual IFaces::RetCode Write(IFaces::Log::MessageType type, char *message);
  virtual IFaces::RetCode Flush();

private:
  typedef std::queue<std::string> LogItemsQueue;
  static const LogItemsQueue::size_type DEFAULT_QUEUE_MAX_LEN = 3;

  std::string Name;
  Common::SharedPtr<IFacesImpl::IStreamHelper> FileStream;
  LogItemsQueue LogItems;
  LogItemsQueue::size_type QueueMaxLen;

  IFacesImpl::IStorageHelper CreateFileStorage();
  bool IsValid() const;
};

#endif  // !__ISTREAMIMPL_H__
