#ifndef __UI_H__
#define __UI_H__

#include "../NoCopyable.h"
#include "../Exceptions.h"

namespace OGLUI
{

  DECLARE_RUNTIME_EXCEPTION(ClientContext)

  class ClientContext
    : private Common::NoCopyable
  {
  public:
    ClientContext();
    ~ClientContext();
  private:
    class ClientContextImpl;
    ClientContextImpl *Impl;
  };

}

#endif  // !__UI_H__
