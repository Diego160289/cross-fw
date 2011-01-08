//============================================================================
// Date        : 07.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#include "IStorageImpl.h"
#include "IStreamImpl.h"


IStorageImpl::IStorageImpl()
  : Stream(0)
{
}

IStorageImpl::~IStorageImpl()
{
}

IFaces::RetCode IStorageImpl::GetStream(IFaces::Log::IStream **stream,
                                        const char *name,
                                        const char *namePrefix /*= ""*/,
                                        const char *namePostfix /*= ".log"*/)
{
  //TODO: use for multiple streams.
  if (!stream)
    return IFaces::retBadParam;

  Common::ISyncObject Locker(GetSynObj());
  if (!Stream.Get())
  {
    Stream = Common::IBaseImpl<IStreamImpl>::CreateWithSyn(GetSynObj());
    if
    (
      !Stream.Get() ||
      Stream->SetName((std::string(namePrefix) + std::string(name) +
        std::string(namePostfix)).c_str()) != IFaces::retOk ||
      (
        //First try to open.
        Stream->Open() != IFaces::retOk &&
        //If open does`t possible then create.
        Stream->Create() != IFaces::retOk
      )
    )
    {
      return IFaces::retFail;
    }
  }

  return Stream.QueryInterface(stream);
}

void IStorageImpl::BeforeDestroy()
{
  //TODO: Make Stream autonomus (currently Stream destructor does`t call).
  if (Stream.Get())
    Stream->Flush();
}