//============================================================================
// Date        : 27.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__


#include "IFacesTools.h"
#include "NetworkIFaces.h"
#include "Socks.h"
#include "Pointers.h"


class ITCPServerImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::Network::ITCPServer)
      >
{
public:
  DECLARE_UUID(17fde71c-1e91-4984-8cb9-2bd2f4ec8ac6)

  // ITCPServer
  virtual IFaces::RetCode Start(const char *host, unsigned short port,
    unsigned threadCount, unsigned maxConnectCount,
    IFaces::Network::IClientFactory *factory);
  virtual IFaces::RetCode Stop();

private:
  Common::SharedPtr<Socks::TCPServer> Server;
};

#endif  // !__TCPSERVER_H__
