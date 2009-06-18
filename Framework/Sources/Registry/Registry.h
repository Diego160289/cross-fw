#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "IFacesTools.h"
#include "Mutex.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;

class IRegistryImpl
  : public IFacesTools::CoClassBase<IRegistryImpl, TYPE_LIST_1(IFaces::IRegistry), IFacesTools::MultiObject, System::Mutex>
{
public:
  COCLASS_UUID(cf7456c3-70c7-4a97-b8e4-f910cd2f823b)
  IRegistryImpl();
  virtual ~IRegistryImpl();

  virtual RetCode Open(const char *registryPath);
  virtual RetCode Close();
  virtual RetCode IsOpen();
};

#endif  // !__REGISTRY_H__
