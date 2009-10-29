#ifndef __MAINDATASRCIFACES_H__
#define __MAINDATASRCIFACES_H__

#include "../../Framework/Include/IFaces.h"

namespace IFaces
{
  struct IMainDataSource
    : public IBase
  {
    DECLARE_UUID(68a0fce5-34cf-4a78-bec7-052c7168bdb0)

    virtual RetCode GetResource(const char *resName, IStream **stream) = 0;
  };
}

#endif  // !__MAINDATASRCIFACES_H__
