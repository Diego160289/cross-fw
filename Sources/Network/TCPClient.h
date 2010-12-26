//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __TCPCLIWNT_H__
#define __TCPCLIWNT_H__


#include "IFacesTools.h"
#include "NetworkIFaces.h"
#include "Socks.h"
#include "Pointers.h"


class ITCPClientImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::Network::ITCPClient)
      >
{
public:
  DECLARE_UUID(be565c33-f38b-47e5-8833-889b57fcd29e)

  // ITCPClient
  virtual IFaces::RetCode Connect(const char *host, unsigned short port);
  virtual IFaces::RetCode Disconnect();
  virtual IFaces::RetCode IsConnected(bool *isConnected) const;
  virtual IFaces::RetCode SendData(const void *data, unsigned bytes);
  virtual IFaces::RetCode Recv(IFaces::IRawDataBuffer **data, unsigned long timeout);

private:
  Common::SharedPtr<Socks::TCPClient> Client;
};

#endif  // !__TCPCLIWNT_H__
