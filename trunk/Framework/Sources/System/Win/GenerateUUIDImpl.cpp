#include "GenerateUUIDImpl.h"
#include "DllHolder.h"

#include <stdexcept>

#include <windows.h>

namespace System
{

  std::string GenerateUUIDImpl()
  {
    typedef long (__stdcall *PFNUuidCreate)(GUID*);
    typedef long (__stdcall *PFNUuidToString)(GUID*, unsigned char**);
    typedef long (__stdcall *PFNRpcStringFree)(unsigned char**);
    PFNUuidCreate MyUuidCreate;
    PFNUuidToString MyUuidToString;
    PFNRpcStringFree MyRpcStringFree;
    DllHolder Dll("Rpcrt4.dll");
    MyUuidCreate = Dll.GetProc<PFNUuidCreate>("UuidCreate");
    MyUuidToString = Dll.GetProc<PFNUuidToString>("UuidToStringA");
    MyRpcStringFree = Dll.GetProc<PFNRpcStringFree>("RpcStringFreeA");
    GUID Guid;
    ::ZeroMemory(&Guid, sizeof(Guid));
    if (MyUuidCreate(&Guid) != ERROR_SUCCESS)
      throw std::runtime_error("Can't generate GUID");
    unsigned char* GuidString = NULL;
    if (MyUuidToString(&Guid, &GuidString) != ERROR_SUCCESS)
      throw std::runtime_error("Can't generate GUID");
    ::CharUpperA(reinterpret_cast<char*>(GuidString));
    std::string Ret = reinterpret_cast<const char*>(GuidString);
    MyRpcStringFree(&GuidString);
    return Ret;
  }

}
