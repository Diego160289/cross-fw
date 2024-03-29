//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SYSTEMUTILS_H__
#define __SYSTEMUTILS_H__

#include <string>


namespace System
{

  std::wstring AStringToWString(const std::string &str, bool fromUTF8 = false);
  std::string WStringToAString(const std::wstring &str, bool toUTF8 = false);

}

#endif  // !__SYSTEMUTILS_H__
