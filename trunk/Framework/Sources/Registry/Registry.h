#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "IFacesTools.h"
#include "Mutex.h"

#include "Xml/tinyxml.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;

class IRegistryImpl
  : public Common::CoClassBase<IRegistryImpl, TYPE_LIST_2(IFaces::IRegistryCtrl, IFaces::IRegistry), Common::MultiObject, System::Mutex>
{
public:
  DECLARE_UUID(cf7456c3-70c7-4a97-b8e4-f910cd2f823b)
  IRegistryImpl();
  virtual ~IRegistryImpl();

  // IRegistryCtrl
  virtual RetCode Create(const char *registryPath);
  virtual RetCode Remove(const char *registryPath);
  virtual RetCode Load(const char *registryPath);
  virtual bool IsLoaded() const;
  virtual RetCode Unload();
  virtual RetCode Save(const char *registryPath);
  virtual RetCode IsModified();
  virtual RetCode Close();

  // IRegistry
  virtual RetCode CreatePathKey(const char *pathKey);
  virtual RetCode RemovePathKey(const char *pathKey);
  virtual RetCode GetValue(const char *pathKey, IFaces::IVariant **value);
  virtual RetCode SetValue(const char *pathKey, IFaces::IVariant *value);
  virtual RetCode EnumKey(const char *pathKey, IFaces::IEnum **enumKey);

private:
  static const char RegistryVersion[];
  TiXmlDocument Document;
  bool IsLoadedState;
  bool IsModifiedState;
};

#endif  // !__REGISTRY_H__
