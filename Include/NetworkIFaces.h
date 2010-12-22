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
      virtual RetCode Connect(const char *url, unsigned short port) = 0;
      virtual RetCode Disconnect() = 0;
      virtual RetCode IsConnected(bool *connected) const = 0;
      virtual void SetRecvCallback(IDataCallback *receiver) = 0;
      virtual RetCode SendData(const void *data, unsigned long bytes) = 0;
      virtual void SetWaitTimeout(unsigned long timeout) = 0;
      virtual void GetWaitTimeout(unsigned long *timeout) const = 0;
    };

  }

}

#endif  // !__NETWORKIFACES_H__
