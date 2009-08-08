#ifndef __FLASHCTRLHOLDER_H__
#define __FLASHCTRLHOLDER_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/DllHolder.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"

#include "WFExtensions.h"
#include "WindowMessage.h"

#include <map>

#include <windows.h>

class FlashHandle
  : private Common::NoCopyable
{
public:
  typedef Common::SharedPtr<System::DllHolder> DllHolderPtr;
  FlashHandle(DllHolderPtr finbox);
  ~FlashHandle();
  void Create(HWND parent);
  void Resize(const RECT &r);
private:
  HANDLE File;
  HANDLE FileMapping;
  LPVOID FlashCtrlData;
  DllHolderPtr FInBox;
  HWND FlashWnd;
  typedef struct HFPC_
  {
    void* p;
  } *HFPC;
  typedef HFPC (WINAPI *FPC_LoadOCXCodeFromMemoryPtr)(LPVOID, DWORD);
  FPC_LoadOCXCodeFromMemoryPtr FPC_LoadOCXCodeFromMemory;
  typedef BOOL (WINAPI *FPC_UnloadCodePtr)(HFPC);
  FPC_UnloadCodePtr FPC_UnloadCode;
  typedef void (WINAPI *FPC_SetContextPtr)(HWND, LPCSTR);
  FPC_SetContextPtr FPC_SetContext;
  typedef LPCSTR (WINAPI *FPC_GetClassNameAPtr)(HFPC);
  FPC_GetClassNameAPtr FPC_GetClassName;
  HFPC Flash;
};


DECLARE_RUNTIME_EXCEPTION(FlashCtrlHolder)

class FlashCtrlHolder
  : private Common::NoCopyable
{
public:
  FlashCtrlHolder();
  ~FlashCtrlHolder();
  void Create();
  void Destroy();
  long OnMessage(const IFaces::WindowMessage &msg);
private:
  Common::SharedPtr<FlashHandle> Flash;
  typedef long (FlashCtrlHolder::*MsgHandler)(const IFaces::WindowMessage &);
  typedef std::map<UINT, MsgHandler> HandlerPool;
  HandlerPool Handlers;
  long OnCreate(const IFaces::WindowMessage &msg);
  long OnSize(const IFaces::WindowMessage &msg);
};

#endif // !__FLASHCTRLHOLDER_H__
