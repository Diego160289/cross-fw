//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================

#include "ILogImpl.h"


const IFaces::Log::Level ILogImpl::Nothing = static_cast<IFaces::Log::Level>(-1);
const IFaces::Log::Level ILogImpl::All = IFaces::Log::MessageTypeMaxValue;
const char *ILogImpl::NAME_PREFIX = "";
const char *ILogImpl::NAME_POSTFIX = ".log";

ILogImpl::ILogImpl()
  : Storage(0)
  , FilterLevel(All)
{
}

ILogImpl::~ILogImpl()
{
}

IFaces::RetCode ILogImpl::Write(IFaces::Log::MessageType type, char *message)
{
  if (!IsValidMessageType(type) || !message || !*message)
    return IFaces::retBadParam;

  if (Name.empty())
    return IFaces::retFail;

  //Skip meesage if level is below.
  if (type > FilterLevel)
    return IFaces::retOk;
  
  Common::ISyncObject Locker(GetSynObj());
  IFaces::Log::IStream *Stream = 0;
  if (
    !Storage ||
    Storage->GetStream(&Stream, Name.c_str(), NAME_PREFIX, NAME_POSTFIX) != IFaces::retOk ||
    !Stream
    )
  {
    return IFaces::retFail;
  }

  return Stream->Write(type, message);
}

IFaces::RetCode ILogImpl::SetStorage(IFaces::Log::IStorage *storage)
{
  if (!storage)
    return IFaces::retBadParam;

  Common::ISyncObject Locker(GetSynObj());
  Storage = storage;
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
