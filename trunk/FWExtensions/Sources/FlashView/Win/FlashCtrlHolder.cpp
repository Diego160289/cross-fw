#include "FlashCtrlHolder.h"

#include "../../../../Framework/Include/IStreamMemoryImpl.h"
#include "../../../../Framework/Include/IStorageHelper.h"
#include "../../../../Framework/Include/IStreamHelper.h"
#include "../../../../Framework/Include/CommonUtils.h"
#include "../../../../Framework/Include/Typedefs.h"
#include "../../../../Framework/Include/Mutex.h"

const UINT FlashHandle::FPCM_FIRST = WM_USER + 0x1000;
const UINT FlashHandle::FPCN_FIRST = FlashHandle::FPCM_FIRST - 1;
const UINT FlashHandle::FPCM_PUTMOVIEFROMMEMORY = FlashHandle::FPCM_FIRST + 2;
const UINT FlashHandle::FPCN_FLASHCALL = FlashHandle::FPCN_FIRST - 4;
const UINT FlashHandle::FPCS_NEED_ALL_KEYS = 0x00000002L;

FlashHandle::FlashHandle(DllHolderPtr finbox)
  : File(INVALID_HANDLE_VALUE)
  , FileMapping(0)
  , FlashCtrlData(0)
  , FlashWnd(0)
  , ResourceHandlerID(0)
{
  // TODO: с проперти
  HANDLE NewFile = CreateFileA("./flash10b.ocx", GENERIC_READ, FILE_SHARE_READ,
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
    FPC_GetClassAtom = finbox->GetProc<FPC_GetClassAtomPtr>("FPC_GetClassAtomA");
    FPC_Play = finbox->GetProc<FPC_PlayPtr>("FPC_Play");
    FPC_AddOnLoadExternalResourceHandler = finbox->GetProc<FPC_AddOnLoadExternalResourceHandlerPtr>("FPC_AddOnLoadExternalResourceHandlerA");
    FPCSetReturnValue = finbox->GetProc<FPCSetReturnValuePtr>("FPCSetReturnValueA");

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
  FInBox.Release();
  UnmapViewOfFile(FlashCtrlData);
  CloseHandle(FileMapping);
  CloseHandle(File);
}

HRESULT WINAPI FlashHandle::OnLoadExternalResource(LPCSTR url, IStream** stream, HFPC flash, LPARAM param)
{
  return reinterpret_cast<FlashHandle*>(param)->OnLoadResource(url, stream);
}

HRESULT FlashHandle::OnLoadResource(LPCSTR url, IStream** stream)
{
  return E_FAIL;
}

void FlashHandle::Create(HWND parent)
{
  if (!(FlashWnd = CreateWindowA(reinterpret_cast<LPCSTR>(FPC_GetClassAtom(Flash)), 0, 
    WS_CHILD | WS_VISIBLE | FPCS_NEED_ALL_KEYS, 0,  0, 100, 100, parent, 0, 0, 0)))
  {
    throw FlashCtrlHolderException("Can't create flash control window");
  }
  FPC_SetContext(FlashWnd, ""); 
  ResourceHandlerID = FPC_AddOnLoadExternalResourceHandler(Flash, &FlashHandle::OnLoadExternalResource, reinterpret_cast<LPARAM>(this));
}

void FlashHandle::Resize(const RECT &r)
{
  if (!FlashWnd)
    return;
  MoveWindow(FlashWnd, r.left, r.top, r.right - r.left, r.bottom - r.top,
              IsWindowVisible(FlashWnd));
}

void FlashHandle::SetDataSource(Common::RefObjPtr<IFaces::IStorage> dataSource)
{
  DataSource = dataSource;
}

Common::RefObjPtr<IFaces::IRawDataBuffer> FlashHandle::LoadResource(const char *name)
{
  if (!name)
    throw FlashCtrlHolderException("Empty resource name");
  std::string Name = name;
  Common::Replace<char>(&Name, "\\", "/");
  Common::StringVectorPtr StgNames = Common::SplitString(Name, '/');
  Common::RefObjPtr<IFaces::IStorage> Stg = DataSource;
  if (StgNames->size() > 1)
  {
    for (Common::StringVector::const_iterator i = StgNames->begin() ; i != StgNames->end() - 1 ; ++i)
    {
      if (!i->length())
        continue;
      Stg = IFacesImpl::IStorageHelper(Stg).OpenStorage(*i);
    }
  }
  if (!Stg.Get())
    throw FlashCtrlHolderException("Empty data source");
  return IFacesImpl::StreamToBuf<System::Mutex>(IFacesImpl::IStorageHelper(Stg).OpenStream(StgNames->back()));
}

void FlashHandle::PlayMovie(const char *movieName)
{
  struct
  {
    LPVOID lpData;
    DWORD dwSize;
  } Movie;
  Common::RefObjPtr<IFaces::IRawDataBuffer> Buf = LoadResource(movieName);
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

void FlashHandle::FlashSetRetValue(const char *value)
{
  if (FAILED(FPCSetReturnValue(FlashWnd, value)))
    throw FlashCtrlHolderException("Can't set return value");
}


const UINT FlashCtrlHolder::START_MSG = WM_USER + 100;
const UINT FlashCtrlHolder::PLAY_MOVIE_MSG = FlashCtrlHolder::START_MSG + 1;

FlashCtrlHolder::FlashCtrlHolder()
  : Wnd(0)
{
  Handlers[WM_CREATE] = &FlashCtrlHolder::OnCreate;
  Handlers[WM_SIZE] = &FlashCtrlHolder::OnSize;
  Handlers[WM_NOTIFY] = &FlashCtrlHolder::OnNotify;
  Handlers[WM_PAINT] = &FlashCtrlHolder::OnPaint;
  Handlers[PLAY_MOVIE_MSG] = &FlashCtrlHolder::OnPlayMovieMsg;
}

FlashCtrlHolder::~FlashCtrlHolder()
{
}

void FlashCtrlHolder::Create()
{
  Flash = new FlashHandle(FlashHandle::DllHolderPtr(new System::DllHolder("./f_in_box.dll"))); // TODO: в пропертя
}

void FlashCtrlHolder::Destroy()
{
  Flash.Release();
}

void FlashCtrlHolder::SetDataSource(Common::RefObjPtr<IFaces::IStorage> dataSource)
{
  if (!Flash.Get())
    throw FlashCtrlHolderException("Can't play movie");
  Flash->SetDataSource(dataSource);
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
    Flash->Create(msg.Wnd);
  }
  catch (std::exception &)
  {
    return -1;
  }
  return 1;
}

long FlashCtrlHolder::OnSize(const IFaces::WindowMessage &msg)
{
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
    try
    {
      if(NMhdr->code != FlashHandle::FPCN_FLASHCALL)
        return 0;
      struct SFPCFlashCallInfoStruct
      {
        NMHDR hdr;
        LPCSTR request;
      };
      SFPCFlashCallInfoStruct *Info = reinterpret_cast<SFPCFlashCallInfoStruct*>(msg.LParam);
      std::string Request = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
		  Request += Info->request;


      Flash->FlashSetRetValue("<string>true</string>");
    }
    catch (std::exception &)
    {
      return 0;
    }
    return 1;
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
    Flash->PlayMovie(reinterpret_cast<const char *>(msg.WParam));
  }
  catch (std::exception &)
  {
  }
  return 1;
}
