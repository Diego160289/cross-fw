#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"

#include "RefObjQIPtr.h"
#include "RefObjPtr.h"
#include "ComponentWrappers.h"

DECLARE_RUNTIME_EXCEPTION(FWLoader)

class FWLoader
  : private Common::NoCopyable
{
public:
  FWLoader(const char *registryModuleName, const char *registryClassId, const char *registryName,
    const char *classFactoryId, const char *serviceManagerId, const char *startServiceId)
  {
    Common::ModuleHolder RegistryModule(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(registryModuleName)));
    Common::RefObjQIPtr<IFaces::IRegistryCtrl> RegistryCtrl(RegistryModule.CreateObject(registryClassId));
    {
      Common::Wrappers::RegistryCtrl Ctrl(RegistryCtrl);
      Ctrl.Load(registryName);
    }
    Common::RefObjQIPtr<IFaces::IRegistry> Registry(RegistryCtrl);
    Common::Wrappers::RegistryComponent::ComponentInfoPtr FactoryInfo =
      Common::Wrappers::RegistryComponent(Registry).GetComponentInfo(classFactoryId);
    if (FactoryInfo->GetType() != Common::Wrappers::RegistryComponent::ComponentInfo::ctInProc)
      throw FWLoaderException("Can't load not inproc class factory");
    std::string FactoryModuleName = FactoryInfo->GetLocation() + "/" + FactoryInfo->GetModuleName();
    Common::ModuleHolder FactoryModule(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(FactoryModuleName.c_str())));
    Common::RefObjQIPtr<IFaces::IClassFactoryCtrl> FactoryCtrl(FactoryModule.CreateObject(classFactoryId));
    if (!FactoryCtrl.Get())
      throw FWLoaderException("Can't get class factory control");
    Common::RefObjQIPtr<IFaces::IClassFactory> Factory(FactoryCtrl);
    if (!Factory.Get())
      throw FWLoaderException("Can't get class factory");
    if (FactoryCtrl->SetRegistry(Registry.Get()) != IFaces::retOk)
      throw FWLoaderException("Can't set registry into class factory");
    Common::RefObjPtr<IFaces::IBase> SrvMgrObj;
    if (Factory->CreateObject(serviceManagerId, SrvMgrObj.GetPPtr()) != IFaces::retOk)
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
    if (SrvMgrCtrl->Run(startServiceId) != IFaces::retOk)
      throw FWLoaderException("Can't start service");
  }
  ~FWLoader()
  {
  }
private:
};


int main()
{
  try
  {
    FWLoader Loader(
      "C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\Registry.dll",
      "cf7456c3-70c7-4a97-b8e4-f910cd2f823b",
      "C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\TestReg.xml",
      "0eedde75-ce15-4eba-9026-3d5f94488c26",
      "74a12748-ee4a-4828-a502-6d2c05df637d",
      ""
      );
    /*FWLoader Loader(
      "./Registry.so",
      "cf7456c3-70c7-4a97-b8e4-f910cd2f823b",
      "./TestReg.xml", "0eedde75-ce15-4eba-9026-3d5f94488c26"
      );*/
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
