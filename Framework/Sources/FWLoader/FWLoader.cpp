#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"

int main()
{
  try
  {
    Common::SharedPtr<System::DllHolder>
      Dll(new System::DllHolder("./Registry.dll"));
    Common::ModuleHolder Module(Dll);
    {
      Common::RefObjPtr<IFaces::IRegistry> Reg(Module.CreateObject("cf7456c3-70c7-4a97-b8e4-f910cd2f823b"));
      if (Reg.Get())
        Reg->Open("Hello");
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
