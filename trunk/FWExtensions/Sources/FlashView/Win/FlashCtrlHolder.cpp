#include "FlashCtrlHolder.h"


FlashHandle::FlashHandle(DllHolderPtr finbox)
  : File(INVALID_HANDLE_VALUE)
  , FileMapping(0)
  , FlashCtrlData(0)
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
    Flash = finbox->GetProc<HFPC (WINAPI *)(LPVOID, DWORD)>("FPC_LoadOCXCodeFromMemory")(NewFlashCtrlData, FlashCtrlFileSize);
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
  try
  {
    FInBox->GetProc<BOOL (WINAPI *)(HFPC)>("FPC_UnloadCode")(Flash);
  }
  catch (std::exception &)
  {
  }
  FInBox.Release();
  UnmapViewOfFile(FlashCtrlData);
  CloseHandle(FileMapping);
  CloseHandle(File);
}

FlashCtrlHolder::FlashCtrlHolder()
  : Flash(new FlashHandle(FlashHandle::DllHolderPtr(new System::DllHolder("./f_in_box.dll")))) // TODO: в пропертя
{
}

FlashCtrlHolder::~FlashCtrlHolder()
{
}
