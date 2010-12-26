#ifndef __SOCKSIMPL_H__
#define __SOCKSIMPL_H__

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "Exceptions.h"
#include "NoCopyable.h"


namespace Socks
{

  typedef int SocketHandle;

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
  };

  void CloseSocketImpl(SocketHandle sock);
  bool SetNonBlockingFalgImpl(SocketHandle sock, bool isNonBlocking);
  bool IsErrWouldBlock();
  void FillAddr(const char *url, unsigned short port, sockaddr_in *addr);

}

#endif  // !__SOCKSIMPL_H__
