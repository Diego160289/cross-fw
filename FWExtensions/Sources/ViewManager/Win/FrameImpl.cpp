#include "FrameImpl.h"

#include "../../../../Framework/Include/Mutex.h"
#include "../../../../Framework/Include/SyncObj.h"

#include <map>


const char FrameImpl::FrameClassName[] = "ViewManagerWnd";

class FrameImpl::WndClassHolder
  : private Common::NoCopyable
{
public:
  static WndClassHolder& Instance()
  {
    static Common::SharedPtr<WndClassHolder> Inst(new WndClassHolder);
    return *Inst.Get();
  }
  ~WndClassHolder()
  {
    UnregisterClassA(FrameClassName, GetModuleHandle(0));
  }
  void RegWin(HWND hwnd, FrameImpl *wnd)
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
private:
  System::Mutex WndMapMtx;
  typedef std::map<HWND, FrameImpl*> WndPool;
  WndPool Wins;

  WndClassHolder()
  {
    WNDCLASSEXA Wnd = {0};
    Wnd.cbSize = sizeof(WNDCLASSEX);
    Wnd.style = CS_HREDRAW | CS_VREDRAW;
    Wnd.lpfnWndProc = &FrameImpl::FrameProc;
    Wnd.cbClsExtra = 0;
    Wnd.cbWndExtra = 0;
    Wnd.hInstance = GetModuleHandle(0);
    Wnd.hIcon = 0;
    Wnd.hCursor = LoadCursor(0, IDC_ARROW);
    Wnd.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(/*NULL_BRUSH*/WHITE_BRUSH));
    Wnd.lpszMenuName = 0;
    Wnd.lpszClassName = FrameClassName;
    Wnd.hIconSm = 0;
    if (!RegisterClassExA(&Wnd))
      throw FrameImplException("Can't reg frame");
  }
};

FrameImpl::FrameImpl(Common::SharedPtr<SysDisplays::SysDisplay> display)
  : Display(display)
  , Wnd(0)
  , IsCreated(false)
{
  if (!display.Get())
    throw FrameImplException("Empty display pointer");
  WndEvent.Reset();
  Common::SharedPtr<System::Thread> NewWndThread(new System::Thread(Common::CreateMemberCallback(*this, &FrameImpl::WndThreadFunc)));
  while (!WndEvent.Wait());
  if (!IsCreated)
    throw FrameImplException("Can't create frame");
  WndThread = NewWndThread;
}

void FrameImpl::WndThreadFunc()
{
  try
  {
    WndClassHolder::Instance();
    LPCRECT Rect = Display->GetRect();
    if (!(Wnd = CreateWindowExA(0, FrameClassName, "", WS_POPUP | WS_THICKFRAME,
                    Rect->left, Rect->top, Rect->right - Rect->left, Rect->bottom - Rect->top,
                    0, 0, GetModuleHandle(0), this)))
    {
      throw FrameImplException("Can't create frame");
    }
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
  while (GetMessageA(&Msg, Wnd, 0, 0))
  {
    TranslateMessage(&Msg);
    DispatchMessageA(&Msg);
  }
}

FrameImpl::~FrameImpl()
{
  ::DestroyWindow(Wnd);
}

LRESULT CALLBACK FrameImpl::FrameProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_PAINT)
  {
    int k = 0;
  }
  return ::DefWindowProcA(wnd, msg, wParam, lParam);
}

void FrameImpl::Show(bool isVisible)
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
