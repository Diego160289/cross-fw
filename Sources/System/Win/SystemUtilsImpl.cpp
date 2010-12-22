//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "SystemUtilsImpl.h"

#include <vector>

#include <windows.h>


namespace System
{

  std::wstring AStringToWStringImpl(const std::string &str, bool fromUTF8)
  {
    size_t Len = str.length() + 1;
    Len = ::MultiByteToWideChar(fromUTF8 ? CP_UTF8 : CP_ACP, 0, str.c_str(),
      static_cast<int>(str.length()), 0, 0);
    std::vector<wchar_t> Buffer(Len + 1, 0);
    Len = ::MultiByteToWideChar(fromUTF8 ? CP_UTF8 : CP_ACP, 0, str.c_str(),
      static_cast<int>(str.length()), &Buffer.front(), static_cast<int>(Len));
    return &Buffer.front();
  }

  std::string WStringToAStringImpl(const std::wstring &str, bool toUTF8)
  {
    size_t Len = str.length() + 1;
    Len = ::WideCharToMultiByte(toUTF8 ? CP_UTF8 : CP_ACP, 0, str.c_str(), -1, 0, 0, NULL, NULL);
    std::vector<char> Buffer(Len, 0);
    ::WideCharToMultiByte(toUTF8 ? CP_UTF8 : CP_ACP, 0, str.c_str(), -1, &Buffer.front(),
      static_cast<int>(Len), NULL, NULL);
    return &Buffer.front();
  }

}
