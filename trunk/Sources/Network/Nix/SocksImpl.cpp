#include "SocksImpl.h"

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>


namespace Socks
{

  const SocketHandle InvalidSocket = -1;

  SocketInitializer::SocketInitializer()
  {
  }

  SocketInitializer::~SocketInitializer()
  {
    Done();
  }

  SocketInitializer& SocketInitializer::GetInst()
  {
    static SocketInitializer Inst;
    return Inst;
  }

  void SocketInitializer::Init()
  {
  }

  void SocketInitializer::Done()
  {
  }


  void CloseSocketImpl(SocketHandle sock)
  {
    if (sock == InvalidSocket)
      return;
    ::close(sock);
  }

  bool SetNonBlockingFalgImpl(SocketHandle sock, bool isNonBlocking)
  {
    if (sock == InvalidSocket)
      return false;
    if (isNonBlocking)
      throw std::runtime_error("Set block mode not implemented");
    return ::fcntl(sock, F_SETFL, O_NONBLOCK) != -1;
  }

  bool IsErrWouldBlock()
  {
    return errno == EWOULDBLOCK;
  }

  void FillAddr(const char *url, unsigned short port, sockaddr_in *addr)
  {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(url);
  }

}
