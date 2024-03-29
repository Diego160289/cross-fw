//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "SystemUtils.h"
#include "SystemUtilsImpl.h"


namespace System
{

  std::wstring AStringToWString(const std::string &str, bool fromUTF8)
  {
    return AStringToWStringImpl(str, fromUTF8);
  }

  std::string WStringToAString(const std::wstring &str, bool toUTF8)
  {
    return WStringToAStringImpl(str, toUTF8);
  }

}
