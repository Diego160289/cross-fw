#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "Pointers.h"

#include "Xml/TinyXml/tinyxml.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;

class IRegistryImpl
  : public Common::CoClassBase
      <
        IRegistryImpl,
        TYPE_LIST_2(IFaces::IRegistryCtrl, IFaces::IRegistry),
        Common::MultiObject,
        System::Mutex
      >
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
  virtual RetCode Save();
  virtual bool IsModified();
  virtual const char* GetCtrlVersion() const;
  virtual const char* GetLoadedRegistryVersion() const;

  // IRegistry
  virtual RetCode CreateKey(const char *pathKey);
  virtual RetCode RemoveKey(const char *pathKey);
  virtual RetCode GetValue(const char *pathKey, IFaces::IVariant **value);
  virtual RetCode SetValue(const char *pathKey, IFaces::IVariant *value);
  virtual RetCode EnumKey(const char *pathKey, IFaces::IEnum **keys);

private:
  static const char RegistryVersion[];
  typedef Common::SharedPtr<TiXmlDocument> TiXmlDocumentPtr;
  TiXmlDocumentPtr Document;
  bool IsModifiedState;
  RetCode InternalLoad(const char *registryPath);
  RetCode InternalUnload();
  mutable std::string LoadedRegistryVersion;
};

#endif  // !__REGISTRY_H__
