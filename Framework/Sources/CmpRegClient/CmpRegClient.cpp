#include <iostream>
#include "../Registry/Registry.h"
#include "ComponentWrappers.h"
#include "ModuleHolder.h"
#include "RefObjQIPtr.h"

void ShowHelp()
{
  std::cout <<
    "ComponentRegostry Client\n"
    "CmpRegClient key [ param ]\n"
    "  key: create remove reg unreg print\n"
    "    create: create new registry\n"
    "      CmpRegClient create \"full registry file name\"\n"
    "    remove: remove registry\n"
    "      CmpRegClient remove \"full registry file name\"\n"
    "    reg: register module\n"
    "      CmpRegClient reg \"full registry file name\" \"full module file name\"\n"
    "    unreg: unregister module\n"
    "      CmpRegClient unreg \"module id\"\n"
    "    print: show all the registered modules\n"
    "      CmpRegClient enum \"full registry file name\"\n";
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    ShowHelp();
    return 1;
  }
  try
  {
    Common::RefObjQIPtr<IFaces::IRegistry> Reg =
      Common::IBaseImpl<IRegistryImpl>::Create<System::Mutex>();
    Common::RefObjQIPtr<IFaces::IRegistryCtrl> RegCtrl(Reg);
    if (!strcmp(argv[1], "create"))
      Common::Wrappers::RegistryCtrl(RegCtrl).Create(argv[2]);
    else
    if (!strcmp(argv[1], "remove"))
      Common::Wrappers::RegistryCtrl(RegCtrl).Remove(argv[2]);
    else
    if (!strcmp(argv[1], "reg"))
    {
      Common::Wrappers::RegistryCtrl(RegCtrl).Load(argv[2]);
      if (argc < 4)
      {
        std::cerr << "Bad param" << std::endl;
        ShowHelp();
        return 1;
      }
      Common::SharedPtr<Common::ModuleHolder> CmpModule;
      try
      {
        CmpModule.Reset(new Common::ModuleHolder(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(argv[3]))));
      }
      catch (std::exception &)
      {
        std::cerr << "Error load \"" << argv[3] << "\"" << std::endl;
        return 1;
      }
      Common::Wrappers::RegistryComponent::ComponentInfo Info;
      Info.SetType(Common::Wrappers::RegistryComponent::ComponentInfo::ctInProc);
      Info.SetModuleGuid(CmpModule->GetModuleGuid());
      std::string ModuleName = argv[3];
      std::string::size_type Pos = ModuleName.rfind('\\');
      if (Pos == std::string::npos)
        Pos = ModuleName.rfind('/');
      if (Pos == std::string::npos)
      {
        Info.SetModuleName(ModuleName);
        Info.SetLocation("./");
      }
      else
      {
        Info.SetModuleName(ModuleName.substr(Pos + 1));
        Info.SetLocation(ModuleName.substr(0, Pos));
      }
      Info.SetDescription(CmpModule->GetModuleName());
      Common::StringVector ClassIDs = CmpModule->GetCoClassIds();
      for (Common::StringVector::const_iterator i = ClassIDs.begin() ; i != ClassIDs.end() ; ++i)
        Info.AddClassId(*i);
      Common::Wrappers::RegistryComponent(Reg).SetComponentInfo(Info);
    }
    else
    if (!strcmp(argv[1], "unreg"))
    {
      if (argc < 4)
      {
        std::cerr << "Bad param" << std::endl;
        ShowHelp();
        return 1;
      }
      Common::Wrappers::RegistryCtrl(RegCtrl).Load(argv[2]);
      Common::Wrappers::RegistryComponent(Reg).RemoveModule(argv[3]);
    }
    else
    if (!strcmp(argv[1], "print"))
    {
      Common::Wrappers::RegistryCtrl(RegCtrl).Load(argv[2]);
      Common::Wrappers::RegistryComponent::ComponentInfoPoolPtr Components =
        Common::Wrappers::RegistryComponent(Reg).GetAllComponentsInfo();
      for (Common::Wrappers::RegistryComponent::ComponentInfoPool::const_iterator i = Components->begin() ; i != Components->end() ; ++i)
      {
        std::cout
          << "Component: " << (*i)->GetDescription() << std::endl
          << "  Guid: " << (*i)->GetModuleGuid() << std::endl
          << "  Type: " << (*i)->GetTypeAsStr() << std::endl
          << "  ModuleName: " << (*i)->GetModuleName() << std::endl
          << "  Location: " << (*i)->GetLocation() << std::endl
          << "  CladdIDs:" << std::endl;
        const Common::StringVector &ClassIDs = (*i)->GetClassIDs();
        for (Common::StringVector::const_iterator j = ClassIDs.begin() ; j != ClassIDs.end() ; ++j)
          std::cout << "    ClassId: " << *j << std::endl;
        std::cout << std::endl;
      }
    }
    else
    {
      std::cerr << "unknown key" << std::endl;
      ShowHelp();
      return 1;
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
