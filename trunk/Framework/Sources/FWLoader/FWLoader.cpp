#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"

#include "IVariantImpl.h"
#include "IEnumImpl.h"

#include <sstream>
#include "RefObjQIPtr.h"
#include "RefObjPtr.h"
#include "ComponentWrappers.h"

#include "PulsedTimer.h"

void TestRegistryModule(const char *location, const char *moduleName, bool isNew = false)
{
  try
  {
    Common::SharedPtr<System::DllHolder>
      Dll(new System::DllHolder("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\Registry.dll"));
      //Dll(new System::DllHolder("/home/dmitry/cross-fw/Framework/Bin/Debug/Registry.so"));
    Common::ModuleHolder Module(Dll);
    Common::RefObjQIPtr<IFaces::IRegistryCtrl> RegCtrl(Module.CreateObject("cf7456c3-70c7-4a97-b8e4-f910cd2f823b"));
    if (!RegCtrl.Get())
    {
      std::cerr << "Can'r load reg" << std::endl;
      return;
    }
    if (isNew)
    {
      if (RegCtrl->Create("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\TestReg.xml") != IFaces::retOk)
      {
        std::cerr << "Can't create reg file" << std::endl;
        return;
      }
    }
    else
    {
      if (RegCtrl->Load("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\TestReg.xml") != IFaces::retOk)
      {
        std::cerr << "Can't load reg file" << std::endl;
        return;
      }
    }
    Common::RefObjQIPtr<IFaces::IRegistry> Reg(RegCtrl);
    if (!Reg.Get())
    {
      std::cerr << "Can't get reg iface" << std::endl;
      return;
    }

    std::string Path = location;
    Path += "/";
    Path += moduleName;
    Common::ModuleHolder CmpMod(Common::ModuleHolder::DllHolderPtr(
      new System::DllHolder(Path.c_str()))
      );
    Common::Wrappers::RegistryComponent::ComponentInfo Info;
    Info.SetType(Common::Wrappers::RegistryComponent::ComponentInfo::ctInProc);
    Info.SetModuleGuid(CmpMod.GetModuleGuid());
    Info.SetModuleName(moduleName);
    Info.SetLocation(location);
    Info.SetDescription(CmpMod.GetModuleName());
    Common::ModuleHolder::StringList IDs = CmpMod.GetCoClassIds();
    for (Common::ModuleHolder::StringList::const_iterator i = IDs.begin() ; i != IDs.end() ; ++i)
      Info.AddClassId(i->c_str());
    Common::Wrappers::RegistryComponent(Reg).SetComponentInfo(Info);
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
}

DECLARE_RUNTIME_EXCEPTION(FWLoader)



class FWLoader
  : private Common::NoCopyable
{
public:
  FWLoader(const char *registryModuleName, const char *registryClassId, const char *registryName,
    const char *classFactoryId)
    : RegistryModule(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(registryModuleName)))
  {
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
    ModuleHolderPtr NewfactoryModule(new Common::ModuleHolder(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(FactoryModuleName.c_str()))));
    Common::RefObjQIPtr<IFaces::IClassFactoryCtrl> NewFactoryCtrl(NewfactoryModule->CreateObject(classFactoryId));
    if (!NewFactoryCtrl.Get())
      throw FWLoaderException("Can't get class factory control");
    Common::RefObjQIPtr<IFaces::IClassFactory> Factory(NewFactoryCtrl);
    if (!Factory.Get())
      throw FWLoaderException("Can't get class factory");
    if (NewFactoryCtrl->SetRegistry(Registry.Get()) != IFaces::retOk)
      throw FWLoaderException("Can't set registry into class factory");

    ClassFactorModule.Swap(NewfactoryModule);
    FactoryCtrl = NewFactoryCtrl;
  }
  ~FWLoader()
  {
    std::cout << "RegistryModule " << RegistryModule.GetModuleCounter() << std::endl;
    std::cout << "ClassFactorModule " << ClassFactorModule->GetModuleCounter() << std::endl;
    FactoryCtrl.Release();
    std::cout << "RegistryModule " << RegistryModule.GetModuleCounter() << std::endl;
    std::cout << "ClassFactorModule " << ClassFactorModule->GetModuleCounter() << std::endl;
  }
private:
  Common::ModuleHolder RegistryModule;
  typedef Common::SharedPtr<Common::ModuleHolder> ModuleHolderPtr;
  ModuleHolderPtr ClassFactorModule;
  Common::RefObjPtr<IFaces::IClassFactoryCtrl> FactoryCtrl;
};

// TODO:
//    1. ������ ���� ������� �� IRegistryCtrl
//    2. Save ��� ������� � ��������� �����


void Func()
{
  static int i = 0;
  std::cout << i ++ << std::endl;
}

int main()
{
  //TestRegistryModule("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug", "Registry.dll", true);
  //TestRegistryModule("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug", "ClassFactory.dll");
  //TestRegistryModule("/home/dmitry/cross-fw/Framework/Bin/Debug", "Registry.so", true);
  //TestRegistryModule("/home/dmitry/cross-fw/Framework/Bin/Debug", "ClassFactory.so");
  try
  {
    /*FWLoader Loader(
      "C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\Registry.dll",
      "cf7456c3-70c7-4a97-b8e4-f910cd2f823b",
      "C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\TestReg.xml",
      "0eedde75-ce15-4eba-9026-3d5f94488c26"
      );*/
    /*FWLoader Loader(
      "/home/dmitry/cross-fw/Framework/Bin/Debug/Registry.so",
      "cf7456c3-70c7-4a97-b8e4-f910cd2f823b",
      "/home/dmitry/cross-fw/Framework/Bin/Debug/TestReg.xml", "0eedde75-ce15-4eba-9026-3d5f94488c26"
      );*/

    System::PulsedTimer tt(Common::CreateFuncCakkback(&Func), 1000, 1000);
    ::system("PAUSE");
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
