//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "TCPClient.h"


IFaces::RetCode ITCPClientImpl::Connect(const char *host, unsigned short port)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode ITCPClientImpl::Disconnect()
{
  return IFaces::retNotImpl;
}

IFaces::RetCode ITCPClientImpl::IsConnected(bool *isConnected) const
{
  return IFaces::retNotImpl;
}

IFaces::RetCode ITCPClientImpl::SendData(const void *data, unsigned bytes)
{
  return IFaces::retNotImpl;
}

IFaces::RetCode ITCPClientImpl::Recv(IFaces::IRawDataBuffer **data, unsigned long timeout)
{
  return IFaces::retNotImpl;
}
