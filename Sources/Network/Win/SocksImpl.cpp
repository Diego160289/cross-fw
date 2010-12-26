//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "SocksImpl.h"

#include "SyncObj.h"


namespace Socks
{

  const SocketHandle InvalidSocket = INVALID_SOCKET;

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
    Common::SyncObject<System::Mutex> Locker(Mtx);
    if (!Counter)
    {
      WSADATA WSData = { 0 };
      if(::WSAStartup(MAKEWORD(2, 2), &WSData))
        throw SocketInitializerException("Error init socket lib");
    }
    ++Counter;
  }

  void SocketInitializer::Done()
  {
    Common::SyncObject<System::Mutex> Locker(Mtx);
    if (!Counter)
      return;
    if (!--Counter)
      ::WSACleanup();
  }


  void CloseSocketImpl(SocketHandle sock)
  {
    if (sock == InvalidSocket)
      return;
    ::closesocket(sock);
  }

  bool SetNonBlockingFalgImpl(SocketHandle sock, bool isNonBlocking)
  {
    if (sock == InvalidSocket)
      return false;
    u_long On = isNonBlocking ? 1 : 0;
    return ::ioctlsocket(sock, FIONBIO, &On) != -1;
  }

  bool IsErrWouldBlock()
  {
    return ::WSAGetLastError() == WSAEWOULDBLOCK;
  }


  void FillAddr(const char *url, unsigned short port, sockaddr_in *addr)
  {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(url);
  }

}
