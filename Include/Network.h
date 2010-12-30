//============================================================================
// Date        : 30.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "NetworkIFaces.h"
#include "RefObjPtr.h"
#include "Exceptions.h"


namespace Common
{
  namespace Wrappers
  {
    namespace Network
    {

      typedef Common::RefObjPtr<IFaces::Network::ITCPServer> ITCPServerPtr;
      typedef Common::RefObjPtr<IFaces::Network::IClientFactory> IClientFactoryPtr;

      DECLARE_RUNTIME_EXCEPTION(TCPServer)

      class TCPServer
      {
      public:
        TCPServer(ITCPServerPtr server, const std::string &host, unsigned short port,
          unsigned threadCount, unsigned maxConnectCount, IClientFactoryPtr factory);
        virtual ~TCPServer();

      private:
        ITCPServerPtr Server;
      };

    }
  }
}

#endif  // !__NETWORK_H__
