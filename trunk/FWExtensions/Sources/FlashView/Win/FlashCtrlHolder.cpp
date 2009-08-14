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
const UINT FlashHandle::FPCN_FLASHCALL = FlashHandle::FPCN_FIRST - 5;
const UINT FlashHandle::FPCN_LOADEXTERNALRESOURCE = FlashHandle::FPCN_FIRST - 2;
const UINT FlashHandle::FPCS_NEED_ALL_KEYS = 0x00000002L;

FlashHandle::FlashHandle(DllHolderPtr finbox)
  : File(INVALID_HANDLE_VALUE)
  , FileMapping(0)
  , FlashCtrlData(0)
  , FlashWnd(0)
  , ResourceHandlerID(0)
{
  // TODO: с проперти
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
    FPC_AddOnLoadExternalResourceHandler = finbox->GetProc<FPC_AddOnLoadExternalResourceHandlerPtr>("FPC_AddOnLoadExternalResourceHandlerW");
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

HRESULT WINAPI FlashHandle::OnLoadExternalResource(LPCWSTR url, IStream** stream, HFPC flash, LPARAM param)
{
  return reinterpret_cast<FlashHandle*>(param)->OnLoadResource(url, stream);
}

HRESULT FlashHandle::OnLoadResource(LPCWSTR url, IStream** stream)
{
  return E_FAIL;
}

void FlashHandle::Create(HWND parent)
{
  if (FAILED(CoInitialize(0)))
    throw FlashCtrlHolderException("Can't COM init");
  if (!(FlashWnd = CreateWindowW(reinterpret_cast<LPCWSTR>(FPC_GetClassAtom(Flash)), 0, 
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

  std::wstring AStringToWString(const std::string &s, bool fromUTF8)
  {
    size_t Len = s.length() + 1;
    Len = ::MultiByteToWideChar(fromUTF8 ? CP_UTF8 : CP_ACP, 0, s.c_str(),
      static_cast<int>(s.length()), 0, 0);
    std::vector<wchar_t> Buffer(Len + 1, 0);
    Len = ::MultiByteToWideChar(fromUTF8 ? CP_UTF8 : CP_ACP, 0, s.c_str(),
      static_cast<int>(s.length()), &Buffer.front(), static_cast<int>(Len));
    return &Buffer.front();
  }

  std::string WStringToAString(const std::wstring &s, bool toUTF8)
  {
    size_t Len = s.length() + 1;
    Len = ::WideCharToMultiByte(toUTF8 ? CP_UTF8 : CP_ACP, 0, s.c_str(), -1, 0, 0, NULL, NULL);
    std::vector<char> Buffer(Len, 0);
    ::WideCharToMultiByte(toUTF8 ? CP_UTF8 : CP_ACP, 0, s.c_str(), -1, &Buffer.front(),
      static_cast<int>(Len), NULL, NULL);
    return &Buffer.front();
  }

Common::RefObjPtr<IFaces::IRawDataBuffer> FlashHandle::LoadResource(const wchar_t *name)
{
  if (!name)
    throw FlashCtrlHolderException("Empty resource name");
  std::string Name = WStringToAString(name, false);
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
  Common::RefObjPtr<IFaces::IRawDataBuffer> Buf = LoadResource(AStringToWString(movieName, false).c_str());
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

void FlashHandle::LoadResource(const wchar_t *name, IStream *stream)
{
  std::wstring Name = name;
  while (Name[0] == L' ')
    Name = Name.substr(1);
  while (Name[Name.length() - 1] == L' ' ||
         Name[Name.length() - 1] == L'\r' ||
         Name[Name.length() - 1] == L'\n')
  {
    Name = Name.substr(0, Name.length() - 1);
  }
  Common::RefObjPtr<IFaces::IRawDataBuffer> Buffer = LoadResource(Name.c_str());
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
  Handlers[WM_SIZE] = &FlashCtrlHolder::OnSize;
  Handlers[WM_NOTIFY] = &FlashCtrlHolder::OnNotify;
  Handlers[WM_PAINT] = &FlashCtrlHolder::OnPaint;
  Handlers[PLAY_MOVIE_MSG] = &FlashCtrlHolder::OnPlayMovieMsg;
  Handlers[FLASH_REQUEST_MSG] = &FlashCtrlHolder::OnFlashRequestMsg;
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

#include "../../../../Framework/Include/IStorageFileImpl.h"
#include "../../../../Framework/Include/IStreamFileImpl.h"
#include "../../../../Framework/Include/MutexStub.h"

long FlashCtrlHolder::OnNotify(const IFaces::WindowMessage &msg)
{
  LPNMHDR NMhdr = reinterpret_cast<LPNMHDR>(msg.LParam);
  if (Flash->GetHWND() == NMhdr->hwndFrom)
  {
    try
    {
      if(NMhdr->code == FlashHandle::FPCN_LOADEXTERNALRESOURCE)
      {
        struct SFPCLoadExternalResource
        {    
          NMHDR hdr;
          LPCSTR RelativePath;
          LPSTREAM Stream;
        };
        SFPCLoadExternalResource *Res = reinterpret_cast<SFPCLoadExternalResource*>(msg.LParam);
        Flash->LoadResource(AStringToWString(Res->RelativePath, false).c_str(), Res->Stream);
        return 1;
      }

      if(NMhdr->code != FlashHandle::FPCN_FLASHCALL)
        return 0;
      struct SFPCFlashCallInfoStruct
      {
        NMHDR hdr;
        LPCWSTR request;
      };
      FlashRequetsQueue.push(reinterpret_cast<SFPCFlashCallInfoStruct*>(msg.LParam)->request);
      PostMessage(Wnd, FLASH_REQUEST_MSG, 0, 0);
      Flash->FlashSetRetValue(L"<string>true</string>");
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

long FlashCtrlHolder::OnFlashRequestMsg(const IFaces::WindowMessage &msg)
{
  try
  {
    std::vector<std::wstring> Funcs;
    Funcs.push_back(L"OnGetAnimation");
    Funcs.push_back(L"OnGetBuildingsByProductId");
    Funcs.push_back(L"OnGetContent");
    Funcs.push_back(L"OnGetContentByGroupId");
    Funcs.push_back(L"OnGetGroups");
    Funcs.push_back(L"OnGetHallByNavigationId");
    Funcs.push_back(L"OnGetProductsDetails");
    Funcs.push_back(L"OnGetSchedule");
    Funcs.push_back(L"OnGetSegment");
    Funcs.push_back(L"OnSystemInitialize");

    while (!FlashRequetsQueue.empty())
    {
      std::wstring Request = L"<?xml version='1.0' encoding='utf-8'?>";
      Request += FlashRequetsQueue.front();
      FlashRequetsQueue.pop();
      for (std::vector<std::wstring>::const_iterator i = Funcs.begin() ; i != Funcs.end() ; ++i)
      {
        if (Request.find(*i) != std::wstring::npos)
        {
          std::wstring Path = (*i) + L".xml";
          Common::RefObjPtr<IFaces::IStream> Stream = IFacesImpl::OpenMemoryStream<System::MutexStub>();
          IFacesImpl::IStreamHelper(IFacesImpl::IStorageHelper(IFacesImpl::OpenFileStorage<System::MutexStub>("./FlashData/XmlFuncs", false)).OpenStream(WStringToAString(Path, false))).CopyTo(Stream);
          IFacesImpl::IStreamHelper Sh(Stream);
          Sh.SeekToEnd();
          Sh.Write("\0", 1);
          Sh.SeekToBegin();
          Flash->CallFlash(AStringToWString((const char *)Common::RefObjQIPtr<IFaces::IRawDataBuffer>(Stream)->GetData(), true).c_str());
          break;
        }
      }
    }
  }
  catch (std::exception &)
  {
    return 0;
  }
  return 1;
}
