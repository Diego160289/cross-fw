#include "FlashCtrlHolder.h"


FlashHandle::FlashHandle(DllHolderPtr finbox)
  : File(INVALID_HANDLE_VALUE)
  , FileMapping(0)
  , FlashCtrlData(0)
  , FlashWnd(0)
{
  // TODO: с проперти
  HANDLE NewFile = CreateFileA("./flash.ocx", GENERIC_READ, FILE_SHARE_READ,
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
    FPC_GetClassName = finbox->GetProc<FPC_GetClassNameAPtr>("FPC_GetClassNameA");

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

void FlashHandle::Create(HWND parent)
{
  if (!(FlashWnd = CreateWindowA(FPC_GetClassName(Flash), 0, 
    WS_CHILD | WS_VISIBLE, 0,  0, 100, 100, parent, 0, GetModuleHandle(0), 0)))
  {
    throw FlashCtrlHolderException("Can't create flash control window");
  }
}

void FlashHandle::Resize(const RECT &r)
{
  if (!FlashWnd)
    return;
  MoveWindow(FlashWnd, r.left, r.top, r.right - r.left, r.bottom - r.top,
              IsWindowVisible(FlashWnd));
}


FlashCtrlHolder::FlashCtrlHolder()
{
  Handlers[WM_CREATE] = &FlashCtrlHolder::OnCreate;
  Handlers[WM_SIZE] = &FlashCtrlHolder::OnSize;
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

long FlashCtrlHolder::OnMessage(const IFaces::WindowMessage &msg)
{
  HandlerPool::iterator Iter = Handlers.find(msg.Msg);
  return Iter == Handlers.end() ? 0 : (this->*Iter->second)(msg);
}

long FlashCtrlHolder::OnCreate(const IFaces::WindowMessage &msg)
{
  try
  {
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
