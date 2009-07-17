#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"
#include "RefObjQIPtr.h"
#include "RefObjPtr.h"
#include "ComponentWrappers.h"
#include "Xml/TinyXml/tinyxml.h"
#include "Typedefs.h"
#include "CommonUtils.h"


DECLARE_RUNTIME_EXCEPTION(FWLoader)

class FWLoader
  : private Common::NoCopyable
{
public:
  FWLoader(const Common::StringMap &params)
  {
    Common::ModuleHolder RegistryModule(Common::ModuleHolder::DllHolderPtr(
      new System::DllHolder(Common::GetValueSromStringMap("RegistryModuleName", params).c_str())));
    Common::RefObjQIPtr<IFaces::IRegistryCtrl> RegistryCtrl(
      RegistryModule.CreateObject(Common::GetValueSromStringMap("RegitryClassId", params).c_str()));
    {
      Common::Wrappers::RegistryCtrl Ctrl(RegistryCtrl);
      Ctrl.Load(Common::GetValueSromStringMap("RegistryName", params).c_str());
    }
    Common::RefObjQIPtr<IFaces::IRegistry> Registry(RegistryCtrl);
    Common::Wrappers::RegistryComponent::ComponentInfoPtr FactoryInfo =
      Common::Wrappers::RegistryComponent(Registry).GetComponentInfo(Common::GetValueSromStringMap("ClassFactoryId", params));
    if (FactoryInfo->GetType() != Common::Wrappers::RegistryComponent::ComponentInfo::ctInProc)
      throw FWLoaderException("Can't load not inproc class factory");
    std::string FactoryModuleName = FactoryInfo->GetLocation() + "/" + FactoryInfo->GetModuleName();
    Common::ModuleHolder FactoryModule(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(FactoryModuleName.c_str())));
    Common::RefObjQIPtr<IFaces::IClassFactoryCtrl> FactoryCtrl(FactoryModule.CreateObject(
      Common::GetValueSromStringMap("ClassFactoryId", params).c_str()));
    if (!FactoryCtrl.Get())
      throw FWLoaderException("Can't get class factory control");
    Common::RefObjQIPtr<IFaces::IClassFactory> Factory(FactoryCtrl);
    if (!Factory.Get())
      throw FWLoaderException("Can't get class factory");
    if (FactoryCtrl->SetRegistry(Registry.Get()) != IFaces::retOk)
      throw FWLoaderException("Can't set registry into class factory");
    Common::RefObjPtr<IFaces::IBase> SrvMgrObj;
    if (Factory->CreateObject(Common::GetValueSromStringMap("ServiceManagerId", params).c_str(), SrvMgrObj.GetPPtr()) != IFaces::retOk)
      throw FWLoaderException("Can't create Service manager object");
    Common::RefObjQIPtr<IFaces::IServiceManagerCtrl> SrvMgrCtrl(SrvMgrObj);
    if (!SrvMgrCtrl.Get())
      throw FWLoaderException("Can't get service manager control");
    Common::RefObjQIPtr<IFaces::IServiceManager> SrvMgr(SrvMgrObj);
    if (!SrvMgr.Get())
      throw FWLoaderException("Can't get service manager");
    if (SrvMgrCtrl->SetRegistry(Registry.Get()) != IFaces::retOk)
      throw FWLoaderException("Can't set registry into service manager control");
    if (SrvMgrCtrl->SetClassFactory(Factory.Get()) != IFaces::retOk)
      throw FWLoaderException("Can't set class factory into service manager control");
    if (SrvMgrCtrl->Run(Common::GetValueSromStringMap("StartServiceId", params).c_str()) != IFaces::retOk)
      throw FWLoaderException("Can't start service");
  }
  ~FWLoader()
  {
  }
private:
};

Common::StringMapPtr LoadConfig(const char *configFileName)
{
  TiXmlDocument Doc;
  if (!Doc.LoadFile(configFileName, TIXML_ENCODING_UTF8))
    throw std::runtime_error("Can't load config file");
  const TiXmlElement *Config = Doc.FirstChildElement("Config");
  if (!Config)
    throw std::logic_error("Bad config file");
  std::string Version;
  if (Config->QueryValueAttribute("Version", &Version) != TIXML_SUCCESS)
    throw std::logic_error("Can't get config version");
  if (Version != "1.0")
    throw std::logic_error("Unsupported version");
  Common::StringMapPtr RetMap(new Common::StringMap);
  for (const TiXmlNode *i = Config->FirstChild() ; i ; i = i->NextSibling())
  {
    std::string Name = i->ValueStr();
    if (Name.empty())
      throw std::logic_error("Bad config config file");
    const TiXmlNode *ValNode = i->FirstChild();
    if (!ValNode)
      throw std::logic_error("Bad config config file");
    std::string Value = ValNode->ValueStr();
    if (Value.empty())
      throw std::logic_error("Bad config config file");
    (*RetMap.get())[Name] = Value;
  }
  return RetMap;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr
      << "Bad params\n"
      << "FWLoader \"config file\"\n";
    return 1;
  }
  try
  {
    FWLoader(*LoadConfig(argv[1]).get());
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
