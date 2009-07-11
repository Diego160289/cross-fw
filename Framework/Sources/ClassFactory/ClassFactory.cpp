#include "ClassFactory.h"

#include <iostream>

IClassFactoryImpl::IClassFactoryImpl()
{
}

RetCode IClassFactoryImpl::CreateObject(const char *classId, IFaces::IBase **obj)
{
  return retOk;
}

RetCode IClassFactoryImpl::SetRegistry(IFaces::IRegistry *reg)
{
  return retOk;
}

RetCode IClassFactoryImpl::SetParams(IFaces::IVarMap *params)
{
  return retOk;
}

bool IClassFactoryImpl::AfterCreate()
{
  std::cout << "Create" << std::endl;
  return true;
}

void IClassFactoryImpl::BeforeDestroy()
{
  std::cout << "Destroy" << std::endl;
}
