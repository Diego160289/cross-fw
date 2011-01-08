//============================================================================
// Date        : 07.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#include "IStreamImpl.h"
#include "IStorageFileImpl.h"

#include <ctime>
#include <iomanip>


IStreamImpl::IStreamImpl()
  : QueueMaxLen(DEFAULT_QUEUE_MAX_LEN)
{
}

IStreamImpl::~IStreamImpl()
{
}

IFaces::RetCode IStreamImpl::SetName(const char *name)
{
  if (IsValid())
    return IFaces::retFail;
  Name = name;
  return IFaces::retOk;
}

IFaces::RetCode IStreamImpl::Create()
{
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    if (IsValid())
      return IFaces::retFail;

    FileStream = new IFacesImpl::IStreamHelper(CreateFileStorage().CreateStream(Name));
    return IFaces::retOk;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
}

IFaces::RetCode IStreamImpl::Open()
{
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    if (IsValid())
      return IFaces::retFail;

    FileStream = new IFacesImpl::IStreamHelper(CreateFileStorage().OpenStream(Name));
    FileStream->SeekToEnd();
    return IFaces::retOk;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
}

IFaces::RetCode IStreamImpl::Write(IFaces::Log::MessageType type, char *message)
{
  if (!message || !*message)
    return IFaces::retBadParam;
  if (!IsValid())
    return IFaces::retFail;
  try
  {
    Common::ISyncObject Locker(GetSynObj());

    std::stringstream LogItem;
    //Time.
    std::time_t CurrentTime = std::time(0);
    if (CurrentTime > 0)
    {
      //TODO: warning
      std::tm *CurrentTimeHelper = std::localtime(&CurrentTime);
      if (CurrentTimeHelper)
      {
        LogItem
          << std::setw(4) << std::setfill('0') << CurrentTimeHelper->tm_year + 1900 << "-"
          << std::setw(2) << std::setfill('0') << CurrentTimeHelper->tm_mon + 1 << "-"
          << std::setw(2) << std::setfill('0') << CurrentTimeHelper->tm_mday << " "
          << std::setw(2) << std::setfill('0') << CurrentTimeHelper->tm_hour << ":"
          << std::setw(2) << std::setfill('0') << CurrentTimeHelper->tm_min << ":"
          << std::setw(2) << std::setfill('0') << CurrentTimeHelper->tm_sec << " ";
      }
    }
    //Type.
    LogItem << "(";
    switch (type)
    {
    case IFaces::Log::Error :
      LogItem << "Error";
      break;
    case IFaces::Log::Warning :
      LogItem << "Warning";
      break;
    case IFaces::Log::Info :
      LogItem << "Info";
      break;
    default :
      LogItem << "Unknown";
      break;
    }
    LogItem << ") ";
    //Thread ID.
    //TODO:
    LogItem << "0000: ";
    //Message.
    LogItem << message;
    //EOL symbols.
    LogItem << std::endl;

    LogItems.push(LogItem.str());
    if (LogItems.size() >= QueueMaxLen)
      return Flush();

    return IFaces::retOk;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
}

IFaces::RetCode IStreamImpl::Flush()
{
  Common::ISyncObject Locker(GetSynObj());
  while (!LogItems.empty())
  {
    const LogItemsQueue::value_type &LogItem = LogItems.front();
    FileStream->Write(LogItem.c_str(), static_cast<unsigned long>(sizeof(LogItem[0]) * LogItem.size()));
    LogItems.pop();
  }
  return IFaces::retOk;
}

IFacesImpl::IStorageHelper IStreamImpl::CreateFileStorage()
{
  return IFacesImpl::IStorageHelper(Common::IBaseImpl<IFacesImpl::IStorageFileImpl>::CreateWithSyn(GetSynObj()));
}

bool IStreamImpl::IsValid() const
{
  return !!FileStream.Get();
}
