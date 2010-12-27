//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "TCPClient.h"
#include "IRawDataBufferImpl.h"


IFaces::RetCode ITCPClientImpl::Connect(const char *host, unsigned short port)
{
  if (!host)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    if (Client.Get())
      return IFaces::retFalse;
    Common::SharedPtr<Socks::TCPClient> NewClient;
    NewClient->Connect(host, port);
    Client = NewClient;
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode ITCPClientImpl::Disconnect()
{
  Common::ISyncObject Locker(GetSynObj());
  if (!Client.Get())
      return IFaces::retFail;
  try
  {
    Client->Disconnect();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode ITCPClientImpl::IsConnected(bool *isConnected) const
{
  if (!isConnected)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Client.Get())
      return IFaces::retFail;
  try
  {
    *isConnected = Client.Get() && Client->IsConnected();
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode ITCPClientImpl::SendData(const void *data, unsigned bytes)
{
  if (!data || !bytes)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Client.Get())
      return IFaces::retFail;
  try
  {
    Client->SendData(data, bytes);
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode ITCPClientImpl::Recv(IFaces::IRawDataBuffer **data, unsigned long timeout)
{
  if (!data || *data)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (!Client.Get())
      return IFaces::retFail;
  try
  {
    Socks::TCPClient::BufPtr Buf = Client->Recv(timeout);
    if (!Buf.get() || !Buf->size())
      return IFaces::retFalse;
    Common::RefObjPtr<IFacesImpl::IRawDataBufferImpl> Buffer =
      Common::IBaseImpl<IFacesImpl::IRawDataBufferImpl>::CreateWithSyn(GetSynObj());
    Buffer->SetData(&Buf->front(), static_cast<unsigned long>(Buf->size()));
    return Buffer.QueryInterface(data);
  }
  catch (std::exception &)
  {
  }
  return IFaces::retFail;
}
