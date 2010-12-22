//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SYSTEMUTILSIMPL_H__
#define __SYSTEMUTILSIMPL_H__

#include <string>


namespace System
{

  std::wstring AStringToWStringImpl(const std::string &str, bool fromUTF8);
  std::string WStringToAStringImpl(const std::wstring &str, bool toUTF8);

}

#endif  // !__SYSTEMUTILSIMPL_H__
