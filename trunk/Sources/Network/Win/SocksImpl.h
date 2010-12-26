//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SOCKSIMPL_H__
#define __SOCKSIMPL_H__


#include <winsock.h>

#include "Exceptions.h"
#include "NoCopyable.h"
#include "Mutex.h"


namespace Socks
{

  typedef SOCKET SocketHandle;

  extern const SocketHandle InvalidSocket;


  DECLARE_RUNTIME_EXCEPTION(SocketInitializer)

  class SocketInitializer
    : private Common::NoCopyable
  {
  public:
    static SocketInitializer& GetInst();
    ~SocketInitializer();
    void Init();
    void Done();
  private:
    SocketInitializer();
    System::Mutex Mtx;
    unsigned long Counter;
  };

  void CloseSocketImpl(SocketHandle sock);
  bool SetNonBlockingFalgImpl(SocketHandle sock, bool isNonBlocking);
  bool IsErrWouldBlock();
  void FillAddr(const char *url, unsigned short port, sockaddr_in *addr);

}

#endif  // !__SOCKSIMPL_H__
