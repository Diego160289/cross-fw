#ifndef __WINDOWMESSAGE_H__
#define __WINDOWMESSAGE_H__

#include <windows.h>


namespace IFaces
{
  struct WindowMessage
  {
    UINT Msg;
    WPARAM WParam;
    LPARAM LParam;
  };
}

#endif // !__WINDOWMESSAGE_H__
