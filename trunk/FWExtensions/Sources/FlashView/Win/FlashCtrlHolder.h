#ifndef __FLASHCTRLHOLDER_H__
#define __FLASHCTRLHOLDER_H__

#include <windows.h>

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/DllHolder.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"
#include "../../../../Framework/Include/IFaces.h"
#include "../../../../Framework/Include/RefObjPtr.h"


#include "WFExtensions.h"
#include "WindowMessage.h"

#include <map>
#include <queue>
//#include "c:/temp/ar1/ar1/flashpresenter/flash/include/f_in_box.h"

class FlashHandle
  : private Common::NoCopyable
{
public:
  static const UINT FPCM_FIRST;
  static const UINT FPCN_FIRST;
  static const UINT FPCM_PUTMOVIEFROMMEMORY;
  static const UINT FPCN_FLASHCALL;
  static const UINT FPCN_LOADEXTERNALRESOURCE;
  static const UINT FPCS_NEED_ALL_KEYS;
                
  FlashHandle();
  ~FlashHandle();
  void Create(HWND parent);
  void Resize(const RECT &r);
  void SetDataSource(Common::RefObjPtr<IFaces::IStorage> dataSource);
  void PlayMovie(const char *movieName);
  HWND GetHWND();
  void FlashSetRetValue(const wchar_t *value);
  std::wstring CallFlash(const wchar_t *request);
  void LoadResource(const wchar_t *name, IStream *stream);
private:
  typedef struct _HFPC
  {
    void* p;
  } *HFPC;
  HANDLE File;
  HANDLE FileMapping;
  LPVOID FlashCtrlData;
  typedef Common::SharedPtr<System::DllHolder> DllHolderPtr;
  DllHolderPtr FInBox;
  Common::RefObjPtr<IFaces::IStorage> DataSource;
  HFPC Flash;
  HWND FlashWnd;
  DWORD ResourceHandlerID;
  typedef HFPC (WINAPI *FPC_LoadOCXCodeFromMemoryPtr)(LPVOID, DWORD);
  FPC_LoadOCXCodeFromMemoryPtr FPC_LoadOCXCodeFromMemory;
  typedef BOOL (WINAPI *FPC_UnloadCodePtr)(HFPC);
  FPC_UnloadCodePtr FPC_UnloadCode;
  typedef void (WINAPI *FPC_SetContextPtr)(HWND, LPCSTR);
  FPC_SetContextPtr FPC_SetContext;
  typedef ATOM (WINAPI *FPC_GetClassAtomPtr)(HFPC);
  FPC_GetClassAtomPtr FPC_GetClassAtom;
  typedef HRESULT (WINAPI *FPC_PlayPtr)(HWND);
  FPC_PlayPtr FPC_Play;
  typedef HRESULT (WINAPI *PLOAD_EXTERNAL_RESOURCE_HANDLER)(LPCWSTR, IStream**, HFPC, LPARAM);
  typedef DWORD (WINAPI *FPC_AddOnLoadExternalResourceHandlerPtr)(HFPC, PLOAD_EXTERNAL_RESOURCE_HANDLER, LPARAM);
  FPC_AddOnLoadExternalResourceHandlerPtr FPC_AddOnLoadExternalResourceHandler;
  static HRESULT WINAPI OnLoadExternalResource(LPCWSTR url, IStream** stream, HFPC flash, LPARAM param);
  HRESULT OnLoadResource(LPCWSTR url, IStream** stream);
  typedef HRESULT (WINAPI *FPCSetReturnValuePtr)(HWND, LPCWSTR);
  FPCSetReturnValuePtr FPCSetReturnValue;
  typedef HRESULT (WINAPI *FPCCallFunctionPtr)(HWND, LPCWSTR, LPWSTR, DWORD *);
  FPCCallFunctionPtr FPCCallFunction;
  typedef HRESULT (WINAPI *FPC_IStream_WritePtr)(IStream *, const void *, ULONG, ULONG *);
  FPC_IStream_WritePtr FPC_IStream_Write;
  Common::RefObjPtr<IFaces::IRawDataBuffer> LoadResource(const wchar_t *name);
};


DECLARE_RUNTIME_EXCEPTION(FlashCtrlHolder)

class FlashCtrlHolder
  : private Common::NoCopyable
{
public:
  static const UINT START_MSG;
  static const UINT PLAY_MOVIE_MSG;
  static const UINT FLASH_REQUEST_MSG;
  FlashCtrlHolder();
  ~FlashCtrlHolder();
  void SetDataSource(Common::RefObjPtr<IFaces::IStorage> dataSource);
  void PlayMovie(const char *movieName);
  long OnMessage(const IFaces::WindowMessage &msg);
private:
  HWND Wnd;
  Common::SharedPtr<FlashHandle> Flash;
  typedef long (FlashCtrlHolder::*MsgHandler)(const IFaces::WindowMessage &);
  typedef std::map<UINT, MsgHandler> HandlerPool;
  HandlerPool Handlers;
  std::queue<std::wstring> FlashRequetsQueue;
  long OnCreate(const IFaces::WindowMessage &msg);
  long OnDestroy(const IFaces::WindowMessage &msg);
  long OnSize(const IFaces::WindowMessage &msg);
  long OnNotify(const IFaces::WindowMessage &msg);
  long OnPaint(const IFaces::WindowMessage &msg);
  long OnPlayMovieMsg(const IFaces::WindowMessage &msg);
  long OnFlashRequestMsg(const IFaces::WindowMessage &msg);
  typedef long (FlashCtrlHolder::*NotifyHandler)(void *);
  typedef std::map<UINT, NotifyHandler> NotifyHandlerPool;
  NotifyHandlerPool NotifyHandlers;
  long OnLoadExternalResource(void *prm);
  long OnFlashRequest(void *prm);
};

#endif // !__FLASHCTRLHOLDER_H__
