#include <iostream>

#include "ModuleHolder.h"
#include "IFaces.h"

#include "IVariantImpl.h"

void PrintEnum(Common::RefObjPtr<IFaces::IEnum> e, int n)
{
  e->First();
  for (Common::RefObjPtr<IFaces::IBase> i ; e->Next(i.GetPPtr()) == IFaces::retOk ; i.Release())
  {
    Common::RefObjPtr<IFaces::INamedVariable> v;
    if (!i.QueryInterface(v.GetPPtr()))
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
      if (!e1.QueryInterface(e2.GetPPtr()))
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

int main()
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
  return 0;
}
