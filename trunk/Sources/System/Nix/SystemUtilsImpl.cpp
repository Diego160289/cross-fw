//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "SystemUtilsImpl.h"

#include <vector>

#include <stdlib.h>

#include "Utf8/utf8.h"


namespace System
{

  std::wstring AStringToWStringImpl(const std::string &str, bool fromUTF8)
  {
    if (fromUTF8)
    {
      if (utf8::find_invalid(str.begin(), str.end()) != str.end())
        return L"";
      std::wstring RetStr;
      utf8::utf8to16(str.begin(), str.end(), std::back_inserter(RetStr));
      return RetStr;
    }
    size_t Len = str.length() + 1;
    std::vector<wchar_t> Buf(Len, 0);
    mbstowcs(&Buf[0], str.c_str(), Len - 1);
    return &Buf[0];
  }

  std::string WStringToAStringImpl(const std::wstring &str, bool toUTF8)
  {
    if (toUTF8)
    {
      std::string RetStr;
      utf8::utf16to8(str.begin(), str.end(), std::back_inserter(RetStr));
      return RetStr;
    }
    size_t Len = str.length() + 1;
    std::vector<char> Buf(Len, 0);
    wcstombs(&Buf[0], str.c_str(), Len - 1);
    return &Buf[0];
  }

}
