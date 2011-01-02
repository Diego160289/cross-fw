//============================================================================
// Date        : 27.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__


#include "ComponentUUIDs.h"
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
  DECLARE_STR_UUID(UUID_ITCPSERVERIMPL)

  // ITCPServer
  virtual IFaces::RetCode Start(const char *host, unsigned short port,
    unsigned threadCount, unsigned maxConnectCount,
    IFaces::Network::IClientFactory *factory);
  virtual IFaces::RetCode Stop();

private:
  Common::SharedPtr<Socks::TCPServer> Server;
};

#endif  // !__TCPSERVER_H__
