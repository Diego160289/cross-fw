//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "ComponentWrappers.h"


namespace Common
{
  namespace Wrappers
  {

    ClassFactory::ClassFactory(IClassFactoryPtr factory)
      : Factory(factory)
    {
    }

    ClassFactory::IClassFactoryPtr ClassFactory::GetIClassFactoryPtr() const
    {
      return Factory;
    }

  }
}
