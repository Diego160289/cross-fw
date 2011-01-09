//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================

#include "ILogImpl.h"
#include "IStreamHelper.h"

#include <ctime>
#include <iomanip>


const IFaces::Log::Level ILogImpl::Nothing = static_cast<IFaces::Log::Level>(-1);
const IFaces::Log::Level ILogImpl::All = IFaces::Log::MessageTypeMaxValue;
const char *ILogImpl::NAME_PREFIX = "";
const char *ILogImpl::NAME_POSTFIX = ".log";

ILogImpl::ILogImpl()
  : Storage(0)
  , FilterLevel(All)
  , QueueMaxLen(DEFAULT_QUEUE_MAX_LEN)
{
}

ILogImpl::~ILogImpl()
{
}

IFaces::RetCode ILogImpl::Write(IFaces::Log::MessageType type, char *message)
{
  if (!IsValidMessageType(type) || !message || !*message)
    return IFaces::retBadParam;

  //TODO: check storage

  if (Name.empty())
    return IFaces::retFail;

  //Skip meesage if level is below.
  if (type > FilterLevel)
    return IFaces::retOk;

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
  //return Stream->Write(type, message);
}

IFaces::RetCode ILogImpl::Flush()
{
  Common::ISyncObject Locker(GetSynObj());

  IFacesImpl::IStreamHelper::IStreamPtr Stream;
  if (!(Stream = GetStream()).Get())
    return IFaces::retFail;

  while (!LogItems.empty())
  {
    const LogItemsQueue::value_type &LogItem = LogItems.front();
    Stream->Write(LogItem.c_str(), static_cast<unsigned long>(sizeof(LogItem[0]) * LogItem.size()));
    LogItems.pop();
  }
  return IFaces::retOk;
}


IFaces::RetCode ILogImpl::SetStorage(IFaces::IStorage *storage)
{
  if (!storage)
    return IFaces::retBadParam;

  Common::ISyncObject Locker(GetSynObj());
  Storage = new IFacesImpl::IStorageHelper(IFacesImpl::IStorageHelper::IStoragePtr(storage));
  return IFaces::retOk;
}

//Change out stream name.
//SetName() is possible to change name ever Storage was already set. As realtime redirect.
IFaces::RetCode ILogImpl::SetName(const char *name)
{
  if (!name)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  Name = name;
  return IFaces::retOk;
}

IFaces::RetCode ILogImpl::GetName(char *name, int length) const
{
  if (!name || !*name || length < static_cast<int>(Name.size()))
    return IFaces::retBadParam;
  
  //Name.copy(name, length);
  return IFaces::retNotImpl;
}

IFaces::RetCode ILogImpl::SetFilterLevel(IFaces::Log::Level level)
{
  FilterLevel = level;
  return IFaces::retOk;
}

IFaces::RetCode ILogImpl::GetFilterLevel(IFaces::Log::Level *level) const
{
  if (!level)
    return IFaces::retBadParam;
  
  *level = FilterLevel;
  return IFaces::retOk;
}

IFaces::RetCode ILogImpl::SetQueueLen(int length)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode ILogImpl::GetQueueLen(int *length) const
{
  return IFaces::retNotImpl;
}

bool ILogImpl::IsValidMessageType(IFaces::Log::MessageType type)
{
  return type >= IFaces::Log::Error && type < IFaces::Log::MessageTypeMaxValue;
}

IFacesImpl::IStreamHelper::IStreamPtr ILogImpl::GetStream()
{
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    IFacesImpl::IStreamHelper::IStreamPtr Stream;
    try
    {
      //First try to open.
      Stream = Storage->OpenStream(Name.c_str());
    }
    catch (IFacesImpl::IStorageHelperException &)
    {
      //If open does`t possible then create.
      Stream = Storage->CreateStream(Name.c_str());
    }
    return Stream;
  }
  catch (std::exception &)
  {
    return IFacesImpl::IStreamHelper::IStreamPtr(0);
  }
}
