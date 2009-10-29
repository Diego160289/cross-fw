#ifndef __IMAINVIEWIFACES_H__
#define __IMAINVIEWIFACES_H__

#include "../../Framework/Include/IFaces.h"

namespace IFaces
{
  struct IMainViewCallback;

  struct IMainView
    : public IBase
  {
    DECLARE_UUID(485dae28-81e7-41e8-83e4-66618b4efb80)

    virtual void SetCallback(IMainViewCallback *callback) = 0;
    virtual RetCode ShowView() = 0;
  };

  struct IMainViewCallback
    : public IBase
  {
    DECLARE_UUID(97b825fb-42bb-4598-9472-c47fda66e5a9)
    
    virtual RetCode OnQueryExternalResource(const char *resName, IStream **stream) = 0;
  };

}

#endif  // !__IMAINVIEWIFACES_H__
