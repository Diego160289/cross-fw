#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"

#include "IVariantImpl.h"
#include "IEnumImpl.h"
#include "Mutex.h"

#include <sstream>
#include "RefObjQIPtr.h"
#include "RefObjPtr.h"
#include "ComponentWrappers.h"

void PrintEnum(Common::RefObjPtr<IFaces::IEnum> e, int n)
{
  e->First();
  for (Common::RefObjPtr<IFaces::IBase> i ; e->Next(i.GetPPtr()) == IFaces::retOk ; i.Release())
  {
    Common::RefObjPtr<IFaces::INamedVariable> v;
    if (i.QueryInterface(v.GetPPtr()) != IFaces::retOk)
      throw std::runtime_error("query err");
    Common::RefObjPtr<IFaces::IVariant> v1;
    if (v->Get(v1.GetPPtr()) !=IFaces::retOk)
      throw std::runtime_error("query err1");
    if (v1->GetType() == IFaces::IVariant::vtIBase)
    {
      Common::RefObjPtr<IFaces::IBase> e1;
      if (v1->GetValue((void**)e1.GetPPtr()) != IFaces::retOk)
        throw std::runtime_error("query err2");
      for (int k = 0 ; k < n ; ++k) std::cout << " ";
      std::cout << "Key " << v->GetName() << std::endl;
      Common::RefObjPtr<IFaces::IEnum> e2;
      if (e1.QueryInterface(e2.GetPPtr()) != IFaces::retOk)
        throw std::runtime_error("query err2");
      PrintEnum(e2, n + 2);
    }
    else
    {
      for (int k = 0 ; k < n ; ++k) std::cout << " ";
      std::cout << "Key " << v->GetName() << " Value "
        << (v1->IsEmpty() ? "" : (const char*)IFacesImpl::IVariantHelper(v1))
        << std::endl;
    }
  }
}

void TestRegistry()
{
  try
  {
    Common::SharedPtr<System::DllHolder>
		Dll(new System::DllHolder("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\Registry.dll"));
    //Dll(new System::DllHolder("/home/dmitry/cross-fw/Framework/Bin/Debug/Registry.so"));
    Common::ModuleHolder Module(Dll);
    {
      Common::RefObjPtr<IFaces::IBase> Obj(Module.CreateObject("cf7456c3-70c7-4a97-b8e4-f910cd2f823b"));
      Common::RefObjPtr<IFaces::IRegistryCtrl> Reg;
      Obj.QueryInterface(Reg.GetPPtr());
      if (Reg.Get())
      {
        //Reg->Create("./_1.xml");
        if (Reg->Load("C:\\Projects\\cross-fw\\Eclipse\\WinFW\\_1.xml") != IFaces::retOk)
        //if (Reg->Load("/home/dmitry/cross-fw/Eclipse/NixFW/_1.xml") != IFaces::retOk)
          std::cerr << "Can't load reg" << std::endl;
        else
          std::cout << Reg->GetLoadedRegistryVersion() << std::endl;
        //Reg->Remove("./_1.xml");
        Common::RefObjPtr<IFaces::IRegistry> Reg1;
        Reg->QueryInterface(IFaces::IRegistry::GetUUID(), (void**)Reg1.GetPPtr());
        if (Reg1.Get())
        {
          if (Reg1->CreateKey("111/333/555") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          if (Reg1->CreateKey("111/333/666") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          if (Reg1->CreateKey("111/444/888") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          if (Reg1->CreateKey("111/444/777") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          if (Reg1->CreateKey("111/222/999") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          if (Reg1->CreateKey("111/000") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          if (Reg1->CreateKey("111/222/555") != IFaces::retOk)
            std::cerr << "Can't create keys" << std::endl;
          else
          {
            IFacesImpl::IVariantHelper::IVariantPtr Var = IFacesImpl::IVariantImpl<>::CreateObject();
            IFacesImpl::IVariantHelper VH(Var);
            VH = "Hello !!!";
            if (Reg1->SetValue("111/222/555", Var.Get()) != IFaces::retOk)
              std::cerr << "Can't set value" << std::endl;
            else
              std::cout << "Ok" << std::endl;
            if (Reg1->SetValue("111/000", Var.Get()) != IFaces::retOk)
              std::cerr << "Can't set value" << std::endl;
            if (Reg->Save() != IFaces::retOk)
              std::cerr << "Can't save" << std::endl;
            Common::RefObjPtr<IFaces::IVariant> Var1;
            if (Reg1->GetValue("111/222/555", Var1.GetPPtr()) != IFaces::retOk)
              std::cerr << "Can't set value" << std::endl;
            else
              std::cout << (const char*)IFacesImpl::IVariantHelper(Var1) << std::endl;
            Common::RefObjPtr<IFaces::IEnum> En;
            if (Reg1->EnumKey("111", En.GetPPtr()) != IFaces::retOk)
              std::cerr << "Can't enum value" << std::endl;
            else
              PrintEnum(En, 0);
          }
        }
      }
      std::cout << Module.GetModuleCounter() << std::endl;
    }
    std::cout << Module.GetModuleCounter() << std::endl;
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void TestClassFactory()
{
  try
  {
    Common::SharedPtr<System::DllHolder>
		Dll(new System::DllHolder("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\ClassFactory.dll"));
    //Dll(new System::DllHolder("/home/dmitry/cross-fw/Framework/Bin/Debug/ClassFactory.so"));
    Common::ModuleHolder Module(Dll);
    {
      Common::RefObjPtr<IFaces::IBase> Obj(Module.CreateObject("0eedde75-ce15-4eba-9026-3d5f94488c26"));
      if (Obj.Get())
      {
        Common::RefObjQIPtr<IFaces::IClassFactory> Factory(Obj);
        if (Factory.Get())
        {
          std::cout << "Ok" << std::endl;
        }
        std::cout << Module.GetModuleCounter() << std::endl;
      }
      else
        std::cerr << "Can't create object" << std::endl;
    }
    std::cout << Module.GetModuleCounter() << std::endl;
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
}

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
      if (RegCtrl->Create("TestReg.xml") != IFaces::retOk)
      {
        std::cerr << "Can't create reg file" << std::endl;
        return;
      }
    }
    else
    {
      if (RegCtrl->Load("TestReg.xml") != IFaces::retOk)
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

    if (RegCtrl->Save() != IFaces::retOk)
      std::cerr << "Can't save reg" << std::endl;
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
    Common::Wrappers::RegistryComponent::ComponentInfoPtr FactoryInfo =
      Common::Wrappers::RegistryComponent(Common::RefObjQIPtr<IFaces::IRegistry>(RegistryCtrl)).GetComponentInfo(classFactoryId);
    if (FactoryInfo->GetType() != Common::Wrappers::RegistryComponent::ComponentInfo::ctInProc)
      throw FWLoaderException("Can't load not inproc class factory");
    std::string FactoryModuleName = FactoryInfo->GetLocation() + "/" + FactoryInfo->GetModuleName();
    ModuleHolderPtr NewfactoryModule(new Common::ModuleHolder(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(FactoryModuleName.c_str()))));
    Common::RefObjQIPtr<IFaces::IClassFactoryCtrl> NewFactoryCtrl(NewfactoryModule->CreateObject(classFactoryId));
    if (!NewFactoryCtrl.Get())
      throw FWLoaderException("Can't get class factory");
    ClassFactorModule.Swap(NewfactoryModule);
    FactoryCtrl = NewFactoryCtrl;
  }
private:
  Common::ModuleHolder RegistryModule;
  typedef Common::SharedPtr<Common::ModuleHolder> ModuleHolderPtr;
  ModuleHolderPtr ClassFactorModule;
  Common::RefObjPtr<IFaces::IClassFactoryCtrl> FactoryCtrl;
};

// TODO: 
//    1. убрать неск методов из IRegistryCtrl
//    2. Save для реестра в отдельный поток

int main()
{
  //TestRegistry();
  //TestRegistryModule("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug", "Registry.dll", true);
  //TestRegistryModule("C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug", "ClassFactory.dll");
  //TestClassFactory();
  try
  {
    FWLoader Loader(
      "C:\\Projects\\cross-fw\\VCPP\\Framework\\Bin\\Debug\\Registry.dll",
      "cf7456c3-70c7-4a97-b8e4-f910cd2f823b", "TestReg.xml", "0eedde75-ce15-4eba-9026-3d5f94488c26"
      );
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
