#ifndef __WFEXTENSIONS_H__
#define __WFEXTENSIONS_H__

#include "../../Framework/Include/IFaces.h"

namespace IFaces
{

  struct IViewFrame
    : public IBase
  {
    DECLARE_UUID(28505b5a-25fb-47e9-b736-c60aab73b949)
    virtual unsigned GetWindowCount() const = 0;
    virtual RetCode CreateWindow(unsigned *index) = 0;
    virtual RetCode GetCurWindowIndex(unsigned *index) const = 0;
    virtual void* GetCurWindowHandle() = 0;
    virtual RetCode SetCurWindow(unsigned index) = 0;
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

}

#endif // !__WFEXTENSIONS_H__