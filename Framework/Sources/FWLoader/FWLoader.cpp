#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"

int main()
{
  try
  {
    Common::SharedPtr<System::DllHolder>
      Dll(new System::DllHolder("/home/dmitry/cross-fw/Framework/Bin/Debug//Registry.so"));
    Common::ModuleHolder Module(Dll);
    {
      Common::RefObjPtr<IFaces::IBase> Obj(Module.CreateObject("cf7456c3-70c7-4a97-b8e4-f910cd2f823b"));
      Common::RefObjPtr<IFaces::IRegistryCtrl> Reg;
      Obj.QueryInterface(&Reg);
      if (Reg.Get())
        Reg->Load("Hello");
      Common::RefObjPtr<IFaces::IRegistry> Reg1;
      Obj->QueryInterface(IFaces::IRegistry::GetUUID(), (void**)&Reg1);
      if (Reg1.Get())
        Reg1->CreatePathKey("111");
      std::cout << Module.GetModuleCounter() << std::endl;
    }
    std::cout << Module.GetModuleCounter() << std::endl;
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
