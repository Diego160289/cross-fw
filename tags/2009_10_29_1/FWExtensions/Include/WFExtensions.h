#ifndef __WFEXTENSIONS_H__
#define __WFEXTENSIONS_H__

#include "../../Framework/Include/IFaces.h"

namespace IFaces
{

  struct WindowMessage;

  struct IWndMessageHandler
    : public IBase
  {
    DECLARE_UUID(144a971c-64da-4be1-ab10-64bda8c5486b)
    virtual long OnMessage(const WindowMessage &msg) = 0;
  };

  struct IViewFrame
    : public IBase
  {
    DECLARE_UUID(28505b5a-25fb-47e9-b736-c60aab73b949)
    virtual RetCode Show(bool isVisible) = 0;
    virtual unsigned GetWndCount() const = 0;
    virtual RetCode CreateWnd(unsigned *index, IWndMessageHandler *handler = 0) = 0;
    virtual RetCode DestroyWnd(unsigned index) = 0;
    virtual RetCode GetCurWnd(unsigned *index) const = 0;
    virtual RetCode SetCurWnd(unsigned index) = 0;
  };

  struct IDisplay
    : public IBase
  {
    DECLARE_UUID(16d683be-a12e-4e22-b5cb-ee5e20bedec8)
    virtual bool ExistsFrame() const = 0;
    virtual RetCode CreateFrame(IViewFrame **frame) = 0;
    virtual RetCode GetFrame(IViewFrame **frame) = 0;
  };

  struct IViewManager
    : public IBase
  {
    DECLARE_UUID(b4a4d70e-4349-4132-b003-e32bad629d2a)
    virtual unsigned GetDisplayCount() const = 0;
    virtual RetCode GetDisplay(unsigned index, IDisplay **display) = 0;
  };

  struct IViewCallback
    : public IBase
  {
    DECLARE_UUID(398d8f8c-3aac-4c5b-ac70-a1cbdce9c3c6)
    virtual RetCode QueryExternalResource(const char *resName, IStream **resStream) = 0;
    virtual void Execute(IFunction *func) = 0;
  };

  struct IFlashView
    : public IBase
  {
    DECLARE_UUID(97835a53-8f31-4eeb-9485-d7b62da5d146)
    virtual RetCode SetViewCallback(IViewCallback *callback) = 0;
    virtual RetCode PlayMovie(const char *movieName) = 0;
    virtual RetCode CallFunction(IFunction *func) = 0;
  };
}

#endif // !__WFEXTENSIONS_H_
