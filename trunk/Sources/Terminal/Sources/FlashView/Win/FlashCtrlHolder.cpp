#include "FlashCtrlHolder.h"

#include "../../../../Framework/Include/IStreamMemoryImpl.h"
#include "../../../../Framework/Include/IStreamHelper.h"
#include "../../../../Framework/Include/CommonUtils.h"
#include "../../../../Framework/Include/Typedefs.h"
#include "../../../../Framework/Include/Mutex.h"
#include "../../../../Framework/Include/RefObjQIPtr.h"
#include "../../../../Framework/Include/Xml/XmlTools.h"
#include "../../../../Framework/Include/IFunctionImpl.h"
#include "../../../../Framework/Include/SystemUtils.h"


const UINT FlashHandle::FPCM_FIRST = WM_USER + 0x1000;
const UINT FlashHandle::FPCN_FIRST = FlashHandle::FPCM_FIRST - 1;
const UINT FlashHandle::FPCM_PUTMOVIEFROMMEMORY = FlashHandle::FPCM_FIRST + 2;
const UINT FlashHandle::FPCN_FLASHCALL = FlashHandle::FPCN_FIRST - 5;
const UINT FlashHandle::FPCN_LOADEXTERNALRESOURCE = FlashHandle::FPCN_FIRST - 2;
const UINT FlashHandle::FPCS_NEED_ALL_KEYS = 0x00000002L;

FlashHandle::FlashHandle()
  : File(INVALID_HANDLE_VALUE)
  , FileMapping(0)
  , FlashCtrlData(0)
  , FlashWnd(0)
{
  // TODO: с проперти
  DllHolderPtr finbox(new System::DllHolder("./f_in_box.dll"));
  HANDLE NewFile = CreateFileA("./flash9f.ocx", GENERIC_READ, FILE_SHARE_READ,
    0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);  
  if (NewFile == INVALID_HANDLE_VALUE)
    throw FlashCtrlHolderException("Can't load flash control");
  DWORD FlashCtrlFileSize = GetFileSize(NewFile, 0);
  HANDLE NewFileMapping = CreateFileMappingA(NewFile, 0, PAGE_READONLY,
    0, FlashCtrlFileSize, 0);  
  if (!NewFileMapping)
  {
    CloseHandle(NewFile);
    throw FlashCtrlHolderException("Can't mapping flash control");
  }
  LPVOID NewFlashCtrlData = MapViewOfFile(NewFileMapping, FILE_MAP_READ, 0, 0, 0);
  if (!NewFlashCtrlData)
  {
    CloseHandle(NewFileMapping);
    CloseHandle(NewFile);
    throw FlashCtrlHolderException("Can't map view flash control");
  }
  try
  {
    FPC_LoadOCXCodeFromMemory = finbox->GetProc<FPC_LoadOCXCodeFromMemoryPtr>("FPC_LoadOCXCodeFromMemory");
    FPC_UnloadCode = finbox->GetProc<FPC_UnloadCodePtr>("FPC_UnloadCode");
    FPC_SetContext = finbox->GetProc<FPC_SetContextPtr>("FPC_SetContext");
    FPC_GetClassAtom = finbox->GetProc<FPC_GetClassAtomPtr>("FPC_GetClassAtomW");
    FPC_Play = finbox->GetProc<FPC_PlayPtr>("FPC_Play");
    FPCSetReturnValue = finbox->GetProc<FPCSetReturnValuePtr>("FPCSetReturnValueW");
    FPCCallFunction = finbox->GetProc<FPCCallFunctionPtr>("FPCCallFunctionW");
    FPC_IStream_Write = finbox->GetProc<FPC_IStream_WritePtr>("FPC_IStream_Write");

    if (!(Flash = FPC_LoadOCXCodeFromMemory(NewFlashCtrlData, FlashCtrlFileSize)))
      throw FlashCtrlHolderException("Can't init flash control");
  }
  catch (std::exception &)
  {
    UnmapViewOfFile(NewFlashCtrlData);
    CloseHandle(NewFileMapping);
    CloseHandle(NewFile);
    throw;
  }
  FInBox = finbox;
  FlashCtrlData = NewFlashCtrlData;
  FileMapping = NewFileMapping;
  File = NewFile;
}

FlashHandle::~FlashHandle()
{
  if (FlashWnd)
    SendMessage(FlashWnd, WM_CLOSE, 0, 0);
  FPC_UnloadCode(Flash);
  Sleep(1000);
  FInBox.Release();
  UnmapViewOfFile(FlashCtrlData);
  CloseHandle(FileMapping);
  CloseHandle(File);
}

void FlashHandle::Create(HWND parent)
{
  if (!(FlashWnd = CreateWindowW(reinterpret_cast<LPCWSTR>(FPC_GetClassAtom(Flash)), 0, 
    WS_CHILD | WS_VISIBLE | FPCS_NEED_ALL_KEYS, 0,  0, 100, 100, parent, 0, 0, 0)))
  {
    throw FlashCtrlHolderException("Can't create flash control window");
  }
  FPC_SetContext(FlashWnd, ""); 
}

void FlashHandle::Resize(const RECT &r)
{
  if (!FlashWnd)
    return;
  MoveWindow(FlashWnd, r.left, r.top, r.right - r.left, r.bottom - r.top,
              IsWindowVisible(FlashWnd));
}

void FlashHandle::PlayMovie(Common::RefObjPtr<IFaces::IStream> stream)
{
  struct
  {
    LPVOID lpData;
    DWORD dwSize;
  } Movie;
  Common::RefObjPtr<IFaces::IStream> Stream =
    IFacesImpl::OpenMemoryStream<System::MutexStub>();
  IFacesImpl::IStreamHelper(stream).CopyTo(Stream);
  Common::RefObjQIPtr<IFaces::IRawDataBuffer> Buf(Stream);
  Movie.lpData = Buf->GetData();
  Movie.dwSize = Buf->GetSize();
  SendMessage(FlashWnd, FPCM_PUTMOVIEFROMMEMORY, 0, reinterpret_cast<LPARAM>(&Movie));
  if (FAILED(FPC_Play(FlashWnd)))
    throw FlashCtrlHolderException("Can't play movie");
}

HWND FlashHandle::GetHWND()
{
  return FlashWnd;
}

void FlashHandle::FlashSetRetValue(const wchar_t *value)
{
  if (FAILED(FPCSetReturnValue(FlashWnd, value)))
    throw FlashCtrlHolderException("Can't set return value");
}

std::wstring FlashHandle::CallFlash(const wchar_t *request)
{
  std::vector<wchar_t> Response(4096, 0);
  DWORD MaxResponseLen = static_cast<DWORD>(Response.size());
  if (FAILED(FPCCallFunction(FlashWnd, request, &Response[0], &MaxResponseLen)))
    throw FlashCtrlHolderException("Can't call flash function");;
  return &Response[0];
}

void FlashHandle::PutResource(IStream *stream, Common::RefObjPtr<IFaces::IStream> resStream)
{
  Common::RefObjPtr<IFaces::IStream> MemStream =
    IFacesImpl::OpenMemoryStream<System::MutexStub>();
  IFacesImpl::IStreamHelper(resStream).CopyTo(MemStream);
  Common::RefObjQIPtr<IFaces::IRawDataBuffer> Buffer(MemStream);
  ULONG Written = 0;
  if (FAILED(FPC_IStream_Write(stream, Buffer->GetData(), Buffer->GetSize(), &Written)))
    throw FlashCtrlHolderException("Can't load resource");
}


const UINT FlashCtrlHolder::START_MSG = WM_USER + 100;
const UINT FlashCtrlHolder::PLAY_MOVIE_MSG = FlashCtrlHolder::START_MSG + 1;
const UINT FlashCtrlHolder::FLASH_REQUEST_MSG = FlashCtrlHolder::PLAY_MOVIE_MSG + 1;

FlashCtrlHolder::FlashCtrlHolder()
  : Wnd(0)
{
  Handlers[WM_CREATE] = &FlashCtrlHolder::OnCreate;
  Handlers[WM_DESTROY] = &FlashCtrlHolder::OnDestroy;
  Handlers[WM_SIZE] = &FlashCtrlHolder::OnSize;
  Handlers[WM_NOTIFY] = &FlashCtrlHolder::OnNotify;
  Handlers[WM_PAINT] = &FlashCtrlHolder::OnPaint;
  Handlers[PLAY_MOVIE_MSG] = &FlashCtrlHolder::OnPlayMovieMsg;
  Handlers[FLASH_REQUEST_MSG] = &FlashCtrlHolder::OnFlashRequestMsg;

  NotifyHandlers[FlashHandle::FPCN_LOADEXTERNALRESOURCE] = &FlashCtrlHolder::OnLoadExternalResource;
  NotifyHandlers[FlashHandle::FPCN_FLASHCALL] = &FlashCtrlHolder::OnFlashRequest;
}

FlashCtrlHolder::~FlashCtrlHolder()
{
}

void FlashCtrlHolder::Done()
{
  Flash.Release();
  if (Wnd)
    ::SendMessage(Wnd, WM_CLOSE, 0, 0);
  Wnd = 0;
  ViewCallback.Release();
}

void FlashCtrlHolder::SetViewCallback(Common::RefObjPtr<IFaces::IViewCallback> callback)
{
  ViewCallback = callback;
}

void FlashCtrlHolder::PlayMovie(const char *movieName)
{
  if (!Flash.Get())
    throw FlashCtrlHolderException("Can't play movie");
  SendMessage(Wnd, PLAY_MOVIE_MSG, reinterpret_cast<LPARAM>(movieName), 0);
}

long FlashCtrlHolder::OnMessage(const IFaces::WindowMessage &msg)
{
  HandlerPool::iterator Iter = Handlers.find(msg.Msg);
  return Iter == Handlers.end() ? 0 : (this->*Iter->second)(msg);
}

long FlashCtrlHolder::OnCreate(const IFaces::WindowMessage &msg)
{
  try
  {
    Wnd = msg.Wnd;
    Flash = new FlashHandle;
    Flash->Create(msg.Wnd);
  }
  catch (std::exception &)
  {
    return -1;
  }
  return 1;
}

long FlashCtrlHolder::OnDestroy(const IFaces::WindowMessage &msg)
{
  Flash.Release();
  Wnd = 0;
  return 0;
}

long FlashCtrlHolder::OnSize(const IFaces::WindowMessage &msg)
{
  if (!Flash.Get())
    return 0;
  RECT Rect = { 0 };
  if (!GetClientRect(msg.Wnd, &Rect))
    return 0;
  Flash->Resize(Rect);
  return 1;
}

long FlashCtrlHolder::OnNotify(const IFaces::WindowMessage &msg)
{
  LPNMHDR NMhdr = reinterpret_cast<LPNMHDR>(msg.LParam);
  if (Flash->GetHWND() == NMhdr->hwndFrom)
  {
    NotifyHandlerPool::iterator Iter = NotifyHandlers.find(NMhdr->code);
    if (Iter != NotifyHandlers.end())
      return (this->*Iter->second)(reinterpret_cast<void *>(msg.LParam));
  }
  return 0;
}

long FlashCtrlHolder::OnPaint(const IFaces::WindowMessage &msg)
{
  PAINTSTRUCT PS = { 0 };
  BeginPaint(msg.Wnd, &PS);
  EndPaint(msg.Wnd, &PS);
  return 1;
}

long FlashCtrlHolder::OnPlayMovieMsg(const IFaces::WindowMessage &msg)
{
  try
  {
    Common::RefObjPtr<IFaces::IViewCallback> Callback = ViewCallback;
    if (!Callback.Get())
      return 0;
    Common::RefObjPtr<IFaces::IStream> ResStream;
    if (Callback->QueryExternalResource(reinterpret_cast<const char *>(msg.WParam), ResStream.GetPPtr()))
      return 0;
    Flash->PlayMovie(ResStream);
  }
  catch (std::exception &)
  {
  }
  return 1;
}

void FlashCtrlHolder::CallFunction(IFaces::IFunction *func)
{
  if (!Flash.Get())
    return;
  using namespace Common::XmlTools;
  NodePtr FuncNode = IFacesImpl::NodeFromFunction(Common::RefObjPtr<IFaces::IFunction>(func));
  std::string Xml = NodeToUTF8(*FuncNode.Get(), false, false);
  Flash->CallFlash(System::AStringToWString(Xml.c_str(), true).c_str());
}

long FlashCtrlHolder::OnFlashRequestMsg(const IFaces::WindowMessage &msg)
{
  try
  {
    while (!FlashRequetsQueue.empty())
    {
      std::wstring Request = L"<?xml version='1.0' encoding='utf-8'?>";
      Request += FlashRequetsQueue.front();
      FlashRequetsQueue.pop();

      Common::XmlTools::NodePtr Node = Common::XmlTools::XmlToNode(System::WStringToAString(Request, true));
      Common::RefObjPtr<IFaces::IFunction> Function =
        IFacesImpl::FunctionFromNode<System::Mutex>(*Node.Get());

      std::wstring FunctionName = System::AStringToWString(Function->GetFunctionName(), false);

      Common::RefObjPtr<IFaces::IViewCallback> Callback = ViewCallback;
      if (!Callback.Get())
        return 0;
      Callback->Execute(Function.Get());
    }
  }
  catch (std::exception &)
  {
    return 0;
  }
  return 1;
}

long FlashCtrlHolder::OnLoadExternalResource(void *prm)
{
  try
  {
    Common::RefObjPtr<IFaces::IViewCallback> Callback = ViewCallback;
    if (!Callback.Get())
      return 0;
    struct SFPCLoadExternalResource
    {    
      NMHDR hdr;
      LPCSTR RelativePath;
      LPSTREAM Stream;
    };
    SFPCLoadExternalResource *Res = reinterpret_cast<SFPCLoadExternalResource*>(prm);
    std::string ResName = Res->RelativePath;
    while (ResName[0] == L' ')
      ResName = ResName.substr(1);
    while (ResName[ResName.length() - 1] == L' ' ||
           ResName[ResName.length() - 1] == L'\r' ||
           ResName[ResName.length() - 1] == L'\n')
    {
      ResName = ResName.substr(0, ResName.length() - 1);
    }
    Common::Replace<char>(&ResName, "\\", "/");
    Common::RefObjPtr<IFaces::IStream> ResStream;
    if (Callback->QueryExternalResource(ResName.c_str(), ResStream.GetPPtr()))
      return 0;
    Flash->PutResource(Res->Stream, ResStream);
  }
  catch (std::exception &)
  {
    return 0;
  }
  return 1;
}

long FlashCtrlHolder::OnFlashRequest(void *prm)
{
  try
  {
    struct SFPCFlashCallInfoStruct
    {
      NMHDR hdr;
      LPCWSTR request;
    };
    FlashRequetsQueue.push(reinterpret_cast<SFPCFlashCallInfoStruct*>(prm)->request);
    PostMessage(Wnd, FLASH_REQUEST_MSG, 0, 0);
    Flash->FlashSetRetValue(L"<string>true</string>");
  }
  catch (std::exception &)
  {
    return 0;
  }
  return 1;
}
