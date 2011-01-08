//============================================================================
// Date        : 07.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __ISTORAGEIMPL_H__
#define __ISTORAGEIMPL_H__

#include "ComponentUUIDs.h"
#include "IFacesTools.h"
#include "Logger.h"


class IStreamImpl;

class IStorageImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::Log::IStorage)
      >
{
public:
  DECLARE_STR_UUID(UUID_ISTORAGEIMPL)

  IStorageImpl();
  ~IStorageImpl();

  // IStorage
  virtual IFaces::RetCode GetStream(IFaces::Log::IStream **stream,
    const char *name, const char *namePrefix = "", const char *namePostfix = ".log");

  virtual void BeforeDestroy();
private:
  Common::RefObjPtr<IStreamImpl> Stream;
};

#endif  // !__ISTORAGEIMPL_H__
