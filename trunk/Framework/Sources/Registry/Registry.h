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
  virtual RetCode CALL_TYPE Create(const char *registryPath);
  virtual RetCode CALL_TYPE Remove(const char *registryPath);
  virtual RetCode CALL_TYPE Load(const char *registryPath);
  virtual bool CALL_TYPE IsLoaded() const;
  virtual RetCode CALL_TYPE Unload();
  virtual RetCode CALL_TYPE Save();
  virtual bool CALL_TYPE IsModified();
  virtual const char* CALL_TYPE GetCtrlVersion() const;
  virtual const char* CALL_TYPE GetLoadedRegistryVersion() const;

  // IRegistry
  virtual RetCode CALL_TYPE CreateKey(const char *pathKey);
  virtual RetCode CALL_TYPE RemoveKey(const char *pathKey);
  virtual RetCode CALL_TYPE GetValue(const char *pathKey, IFaces::IVariant **value);
  virtual RetCode CALL_TYPE SetValue(const char *pathKey, IFaces::IVariant *value);
  virtual RetCode CALL_TYPE EnumKey(const char *pathKey, IFaces::IEnum **enumKey);

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
