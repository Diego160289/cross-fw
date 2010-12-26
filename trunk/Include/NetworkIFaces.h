//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __NETWORKIFACES_H__
#define __NETWORKIFACES_H__

#include "IFaces.h"


namespace IFaces
{
  
  namespace Network
  {

    struct ITCPClient
      : public IBase
    {
      DECLARE_UUID(77b526e9-494e-4092-8fee-4b56705f0632)
      virtual RetCode Connect(const char *host, unsigned short port) = 0;
      virtual RetCode Disconnect() = 0;
      virtual RetCode IsConnected(bool *isConnected) const = 0;
      virtual RetCode SendData(const void *data, unsigned bytes) = 0;
      virtual RetCode Recv(IRawDataBuffer **data, unsigned long timeout) = 0;
    };

    struct ICtrlItem
      : public IBase
    {
      DECLARE_UUID(cddbcbd3-2b29-486b-b3ca-ff35c1d4094b)
      virtual void CloseMe() = 0;
      virtual bool SendData(const void *data, unsigned bytes) = 0;
    };

    struct IClient
      : public IBase
    {
      DECLARE_UUID(55ff3d63-2dbf-4147-9e64-b536ba7ef709)
      virtual bool OnData(const void *data, unsigned bytes) = 0;
    };

    struct IClientFactory
      : public IBase
    {
      DECLARE_UUID(e7c0ff28-1a51-4f6e-8c7c-2b6b49aaa37b)
      virtual RetCode CreateClient(ICtrlItem *ctrl, IClient **client) = 0;
    };

    struct ITCPServer
      : public IBase
    {
      DECLARE_UUID(80197547-9966-4303-8cc0-a80cdb26afc9)
      virtual RetCode Start(const char *host, unsigned short port,
        unsigned threadCount, unsigned maxConnectCount,
        IClientFactory *factory) = 0;
      virtual RetCode Stop() = 0;
    };

  }

}

#endif  // !__NETWORKIFACES_H__
