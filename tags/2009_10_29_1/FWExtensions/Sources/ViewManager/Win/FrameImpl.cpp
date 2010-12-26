#include "FrameImpl.h"

#include "../../../../Framework/Include/Mutex.h"
#include "../../../../Framework/Include/SyncObj.h"
#include "../../../../Framework/Include/Typedefs.h"

#include "WindowMessage.h"


class WndRoot::WndClassHolder
  : private Common::NoCopyable
{
public:
  static WndClassHolder& Instance()
  {
    static Common::SharedPtr<WndClassHolder> Inst(new WndClassHolder);
    return *Inst.Get();
  }
  void RegWndClass(const char *className)
  {
    for (Common::StringVector::const_iterator i = WndClasses.begin() ; i != WndClasses.end() ; ++i)
    {
      if (*i == className)
        return;
    }
    WNDCLASSEXA Wnd = {0};
    Wnd.cbSize = sizeof(WNDCLASSEX);
    Wnd.style = CS_HREDRAW | CS_VREDRAW;
    Wnd.lpfnWndProc = &WndRoot::FrameProc;
    Wnd.cbClsExtra = 0;
    Wnd.cbWndExtra = 0;
    Wnd.hInstance = GetModuleHandle(0);
    Wnd.hIcon = 0;
    Wnd.hCursor = LoadCursor(0, IDC_ARROW);
    Wnd.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(/*NULL_BRUSH*/WHITE_BRUSH));
    Wnd.lpszMenuName = 0;
    Wnd.lpszClassName = className;
    Wnd.hIconSm = 0;
    if (!RegisterClassExA(&Wnd))
      throw FrameImplException("Can't reg frame");
    WndClasses.push_back(className);
  }
  ~WndClassHolder()
  {
    HINSTANCE Inst = GetModuleHandle(0);
    for (Common::StringVector::const_iterator i = WndClasses.begin() ; i != WndClasses.end() ; ++i)
      UnregisterClassA(i->c_str(), Inst);
  }
  void RegWin(HWND hwnd, WndRoot *wnd)
  {
    Common::SyncObject<System::Mutex> Locker(WndMapMtx);
    Wins[hwnd] = wnd;
  }
  void UnRegWin(HWND hwnd)
  {
    Common::SyncObject<System::Mutex> Locker(WndMapMtx);
    WndPool::iterator Iter = Wins.find(hwnd);
    if (Iter != Wins.end())
      Wins.erase(Iter);
  }
  long ProcessMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    Common::SyncObject<System::Mutex> Locker(WndMapMtx);
    WndPool::iterator Iter = Wins.find(hwnd);
    return Iter != Wins.end() ? Iter->second->ProcessMsg(msg, wParam, lParam) : 0;
  }
private:
  System::Mutex WndMapMtx;
  typedef std::map<HWND, WndRoot*> WndPool;
  WndPool Wins;
  Common::StringVector WndClasses;
  WndClassHolder()
  {
  }
};

WndRoot::WndRoot()
  : Wnd(0)
  , ExStyle(0)
  , Style(0)
  , Parent(0)
  , Menu(0)
  , IsCreated(false)
{
  ZeroMemory(&Rect, sizeof(Rect));
}

WndRoot::~WndRoot()
{
}

void WndRoot::Create(DWORD exStyle, LPCSTR className, LPCSTR windowName,
                     DWORD style, int x, int y, int width, int height,
                     HWND parent, HMENU menu)
{
    if (!CreateWindowExA(exStyle, className, windowName, style,
          x, y, width, height, parent, menu, GetModuleHandle(0), this))
    {
      throw FrameImplException("Can't create frame");
    }
}

void WndRoot::CreateAndRun(DWORD exStyle, LPCSTR className, LPCSTR windowName,
                           DWORD style, int x, int y, int width, int height,
                           HWND parent, HMENU menu)
{
  ExStyle = exStyle;
  ClassName = className;
  WinName = windowName;
  Style = style;
  Rect.left = x;
  Rect.top = y;
  Rect.right = x + width;
  Rect.bottom = y + height;
  Parent = parent;
  Menu = menu;
  WndEvent.Reset();
  Common::SharedPtr<System::Thread> NewWndThread(new System::Thread(Common::CreateMemberCallback(*this, &FrameImpl::WndThreadFunc)));
  while (!WndEvent.Wait());
  if (!IsCreated)
    throw FrameImplException("Can't create frame");
  WndThread = NewWndThread;
}

void WndRoot::Show(bool isVisible)
{
  if (isVisible)
  {
    if (IsWindowVisible(Wnd))
      return;
    ShowWindow(Wnd, SW_SHOW);
    UpdateWindow(Wnd);
    BringWindowToTop(Wnd);
  }
  else
  {
    if (!IsWindowVisible(Wnd))
      return;
    ShowWindow(Wnd, SW_HIDE);
  }
}

bool WndRoot::GetClientRect(LPRECT r)
{
  return !Wnd || !::IsWindow(Wnd) ? false : !!::GetClientRect(Wnd, r);
}

HWND WndRoot::GetHWND() const
{
  return Wnd;
}

bool WndRoot::MoveWindow(int x, int y, int width, int height)
{
  return !Wnd || !::IsWindow(Wnd) ? false :
    !!::MoveWindow(Wnd, x, y, width, height, IsWindowVisible(Wnd));
}

LRESULT CALLBACK WndRoot::FrameProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_CREATE)
  {
    LPCREATESTRUCTA CS = reinterpret_cast<LPCREATESTRUCTA>(lParam);
    WndRoot *Impl = reinterpret_cast<FrameImpl*>(CS->lpCreateParams);
    WndClassHolder::Instance().RegWin(wnd, Impl);
    Impl->Wnd = wnd;
  }
  return WndRoot::WndClassHolder::Instance().ProcessMsg(wnd, msg, wParam, lParam) ?
    1 : ::DefWindowProcA(wnd, msg, wParam, lParam);
}

void WndRoot::WndThreadFunc()
{
  try
  {
    Create(0, ClassName.c_str(), WinName.c_str(), Style,
      Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top,
      Parent, Menu);
    IsCreated = true;
    WndEvent.Set();
  }
  catch (std::exception &)
  {
    IsCreated = false;
  }
  if (!IsCreated)
    return;
  MSG Msg = { 0 };
  while (GetMessageA(&Msg, 0, 0, 0))
  {
    TranslateMessage(&Msg);
    DispatchMessageA(&Msg);
  }
}

long WndRoot::ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_DESTROY)
  {
    WndClassHolder::Instance().UnRegWin(Wnd);
    PostQuitMessage(0);
    return 1;
  }
  return 0;
}


const char ChildView::ChildViewClassName[] = "ViewManaqerChildViewWnd";

ChildView::ChildView(IFaces::IWndMessageHandler *handler)
  : Handler(handler)
{
}

void ChildView::Create(LPCRECT r, WndRoot *parent)
{
  WndClassHolder::Instance().RegWndClass(ChildViewClassName);
  CreateAndRun(0, ChildViewClassName, "", WS_CHILD,
    r->left, r->top, r->right - r->left, r->bottom - r->top,
    parent->GetHWND(), 0);
}

void ChildView::Destroy()
{
  SendMessage(GetHWND(), WM_CLOSE, 0, 0);
  Handler.Release();
}

long ChildView::ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
  long Res = 0;
  if (Handler.Get())
  {
    IFaces::WindowMessage Msg;
    Msg.Wnd = GetHWND();
    Msg.Msg = msg;
    Msg.WParam = wParam;
    Msg.LParam = lParam;
    Res = Handler->OnMessage(Msg);
  }
  Res = Res | WndRoot::ProcessMsg(msg, wParam, lParam);
  return Res;
}


const char FrameImpl::FrameClassName[] = "ViewManagerWnd";
unsigned FrameImpl::WndIdCounter = 0;

FrameImpl::FrameImpl()
  : CurWnd(Wins.end())
{
}

FrameImpl::~FrameImpl()
{
}

void FrameImpl::Create(Common::SharedPtr<SysDisplays::SysDisplay> display)
{
  if (!display.Get())
    throw FrameImplException("Empty display pointer");
  WndClassHolder::Instance().RegWndClass(FrameClassName);
  LPCRECT Rect = display->GetRect();
  CreateAndRun(0, FrameClassName, "", WS_POPUP,
    Rect->left, Rect->top, Rect->right - Rect->left, Rect->bottom - Rect->top, 0, 0);
}

void FrameImpl::Destroy()
{
  for (WndPool::iterator i = Wins.begin() ; i != Wins.end() ; ++i)
    i->second->Destroy();
  Wins.clear();
  SendMessage(GetHWND(), WM_CLOSE, 0, 0);
}

unsigned FrameImpl::GetWndCount() const
{
  return static_cast<unsigned>(0);
}

bool FrameImpl::CreateWnd(unsigned *index, IFaces::IWndMessageHandler *handler)
{
  RECT Rect = { 0 };
  if (!GetClientRect(&Rect))
    return false;
  try
  {
    ChildViewPtr NewWnd(new ChildView(handler));
    NewWnd->Create(&Rect, this);
    Wins[*index = WndIdCounter++] = NewWnd;
    CurWnd = Wins.find(*index);
    return true;
  }
  catch (std::exception &)
  {
  }
  return false;
}

bool FrameImpl::DestroyWnd(unsigned index)
{
  WndPool::iterator Iter = Wins.find(index);
  if (Iter == Wins.end())
    return false;
  if (Iter == CurWnd)
  {
    CurWnd->second->Show(false);
    CurWnd = Wins.end();
  }
  Iter->second->Destroy();
  Wins.erase(Iter);
  return false;
}

bool FrameImpl::GetCurWnd(unsigned *index) const
{
  if (CurWnd == Wins.end())
    return false;
  *index = CurWnd->first;
  return true;
}

bool FrameImpl::SetCurWnd(unsigned index)
{
  WndPool::iterator Iter = Wins.find(index);
  if (Iter == Wins.end())
    return false;
  CurWnd->second->Show(false);
  CurWnd = Iter;
  CurWnd->second->Show(true);
  return true;
}

long FrameImpl::ProcessMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_SIZE)
  {
    RECT Rect = { 0 };
    if (!GetClientRect(&Rect))
      return false;
    for (WndPool::iterator i = Wins.begin() ; i != Wins.end() ; ++i)
    {
      i->second->MoveWindow(Rect.left, Rect.top,
        Rect.left + LOWORD(lParam), Rect.top + HIWORD(lParam));
    }
    return 1;
  }
  return WndRoot::ProcessMsg(msg, wParam, lParam);
}