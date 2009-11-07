#ifndef __FRAMEIMPL_H__
#define __FRAMEIMPL_H__

#include "../../../../Framework/Include/IFacesTools.h"
#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"
#include "../../../../Framework/Include/Thread.h"
#include "../../../../Framework/Include/ManualEvent.h"
#include "../../../../Framework/Include/IFaces.h"
#include "../../../../Framework/Include/RefObjPtr.h"

#include <map>

#include "Terminal.h"

#include "SysDisplays.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;


DECLARE_RUNTIME_EXCEPTION(FrameImpl)

class WndRoot
  : Common::NoCopyable
{
public:
  WndRoot();
  virtual ~WndRoot();
  void Create(DWORD exStyle, LPCSTR className, LPCSTR windowName, DWORD style,
    int x, int y, int width, int height, HWND parent, HMENU menu);
  void CreateAndRun(DWORD exStyle, LPCSTR className, LPCSTR windowName,
    DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu);
  void Show(bool isVisible);
  bool GetClientRect(LPRECT r) const;
  HWND GetHWND() const;
  bool MoveWindow(int x, int y, int width, int height);
private:
  friend class WndClassHolder;
  static LRESULT CALLBACK FrameProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
  HWND Wnd;
  DWORD ExStyle;
  std::string ClassName;
  std::string WinName;
  DWORD Style;
  RECT Rect;
  HWND Parent;
  HMENU Menu;
  volatile bool IsCreated;
  System::ManualEvent WndEvent;
  Common::SharedPtr<System::Thread> WndThread;
  void WndThreadFunc();
protected:
  class WndClassHolder;
  virtual long ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam);
};

class ChildView
  : public Common::CoClassBase
    <
      TYPE_LIST_1(IFaces::IView)
    >
  , public WndRoot
{
public:
  ChildView();
  void SetHandler(IFaces::IWndMessageHandler *handler);
  void Create(LPCRECT r, WndRoot *parent);
  void Destroy();

  // IView
  virtual RetCode GetHandle(System::WindowHandle *handle);
  virtual RetCode GetPos(int *x, int *y) const;
  virtual RetCode GetSize(int *width, int *height) const;

private:
  static const char ChildViewClassName[];
  Common::RefObjPtr<IFaces::IWndMessageHandler> Handler;
protected:
  virtual long ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam);
};

class FrameImpl
  : public WndRoot
{
public:
  FrameImpl();
  ~FrameImpl();

  void Create(Common::SharedPtr<SysDisplays::SysDisplay> display);
  void Destroy();

  unsigned GetWndCount() const;
  bool CreateWnd(unsigned *index, IFaces::IWndMessageHandler *handler);
  bool DestroyWnd(unsigned index);
  bool GetCurWnd(unsigned *index) const;
  bool SetCurWnd(unsigned index);
  bool GetView(unsigned wndIndex, IFaces::IView **view);

private:
  static const char FrameClassName[];

  typedef Common::RefObjPtr<ChildView> ChildViewPtr;
  typedef std::map<unsigned/*index*/, ChildViewPtr/*view*/> WndPool;
  static unsigned WndIdCounter;
  WndPool Wins;
  WndPool::iterator CurWnd;

protected:
  virtual long ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // !__FRAMEIMPL_H__
