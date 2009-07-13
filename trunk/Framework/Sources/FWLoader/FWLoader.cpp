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
    if (Reg->CreateKey("ComponentInformation") != IFaces::retOk)
    {
      std::cerr << "Can't create key ComponentInformation" << std::endl;
      return;
    }
    try
    {
      std::string ModuleFullName = location;
      ModuleFullName += "/";
      ModuleFullName += moduleName;
      Common::ModuleHolder CmpMod(Common::SharedPtr<System::DllHolder>(new System::DllHolder(ModuleFullName.c_str())));
      std::string ComponentKey = "ComponentInformation/" + CmpMod.GetModuleGuid();
      if (Reg->CreateKey(ComponentKey.c_str()) != IFaces::retOk)
        throw std::logic_error("Can't create component key");
      if (Reg->CreateKey((ComponentKey + "/Type").c_str()) != IFaces::retOk)
        throw std::logic_error("Can't create component type key");
      Common::RefObjPtr<IFaces::IVariant> Var = IFacesImpl::CreateVariant<System::Mutex>();
      IFacesImpl::IVariantHelper VH(Var);
      VH = "Inproc";
      if (Reg->SetValue((ComponentKey + "/Type").c_str(), Var.Get()) != IFaces::retOk)
        throw std::logic_error("can't set reg val");
      if (Reg->CreateKey((ComponentKey + "/Name").c_str()) != IFaces::retOk)
        throw std::logic_error("Can't create component name key");
      IFacesImpl::CreateVariant<System::Mutex>();
      VH = moduleName;
      if (Reg->SetValue((ComponentKey + "/Name").c_str(), Var.Get()) != IFaces::retOk)
        throw std::logic_error("can't set reg val");
      if (Reg->CreateKey((ComponentKey + "/Location").c_str()) != IFaces::retOk)
        throw std::logic_error("Can't create component Location key");
      VH = location;
      if (Reg->SetValue((ComponentKey + "/Location").c_str(), Var.Get()) != IFaces::retOk)
        throw std::logic_error("can't set reg val");
      if (Reg->CreateKey((ComponentKey + "/Description").c_str()) != IFaces::retOk)
        throw std::logic_error("Can't create component Description key");
      VH = CmpMod.GetModuleName().c_str();
      if (Reg->SetValue((ComponentKey + "/Description").c_str(), Var.Get()) != IFaces::retOk)
        throw std::logic_error("can't set reg val");
      if (Reg->CreateKey((ComponentKey + "/ClassIDs").c_str()) != IFaces::retOk)
        throw std::logic_error("Can't create component ClassIDs key");
      Common::ModuleHolder::StringList IDs = CmpMod.GetCoClassIds();
      unsigned Index = 0;
      for (Common::ModuleHolder::StringList::const_iterator i = IDs.begin() ; i != IDs.end() ; ++i)
      {
        std::stringstream Io;
        Io << Index++;
        if (Reg->CreateKey((ComponentKey + "/ClassIDs/ClassId_" + Io.str()).c_str()) != IFaces::retOk)
          throw std::logic_error("Can't create component ClassID key");
        VH = i->c_str();
        if (Reg->SetValue((ComponentKey + "/ClassIDs/ClassId_" + Io.str()).c_str(), Var.Get()) != IFaces::retOk)
          throw std::logic_error("can't set classid val");
      }
    }
    catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
      return;
    }
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
    , RegistryCtrl(RegistryModule.CreateObject(registryClassId))
  {
    {
      Common::Wrappers::RegistryCtrl Ctrl(RegistryCtrl);
      Ctrl.Load(registryName);
    }
    Common::RefObjQIPtr<IFaces::IRegistry> Reg(RegistryCtrl);
    Common::Wrappers::Registry Reg1(Reg);
    Reg1.EnumKey("ComponentInformation")->GetChildKeys();
  }
private:
  Common::ModuleHolder RegistryModule;
  Common::RefObjQIPtr<IFaces::IRegistryCtrl> RegistryCtrl;
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
