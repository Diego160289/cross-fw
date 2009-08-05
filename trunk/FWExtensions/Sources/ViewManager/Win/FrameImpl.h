#ifndef __FRAMEIMPL_H__
#define __FRAMEIMPL_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"
#include "../../../../Framework/Include/Thread.h"
#include "../../../../Framework/Include/ManualEvent.h"

#include <map>

#include "WFExtensions.h"

#include "SysDisplays.h"

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
  bool GetClientRect(LPRECT r);
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
  virtual bool ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam);
};

class ChildView
  : public WndRoot
{
public:
  void Create(LPCRECT r, WndRoot *parent);
  void Destroy();
private:
  static const char ChildViewClassName[];
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
  bool CreateWnd(unsigned *index);
  bool DestroyWnd(unsigned index);
  bool GetCurWndIndex(unsigned *index) const;
  void* GetCurWnd();
  bool SetCurWnd(unsigned index);


private:
  static const char FrameClassName[];

  typedef Common::SharedPtr<ChildView> ChildViewPtr;
  typedef std::map<unsigned/*index*/, ChildViewPtr/*view*/> WndPool;
  static unsigned WndIdCounter;
  WndPool Wins;
  WndPool::iterator CurWnd;

protected:
  virtual bool ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // !__FRAMEIMPL_H__
