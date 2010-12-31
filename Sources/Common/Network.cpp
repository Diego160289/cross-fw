//============================================================================
// Date        : 30.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Network.h"


namespace Common
{
  namespace Wrappers
  {
    namespace Network
    {

      TCPServer::TCPServer(ITCPServerPtr server,
                          const std::string &host, unsigned short port,
                          unsigned threadCount, unsigned maxConnectCount,
                          IClientFactoryPtr factory)
        : Srv(server)
      {
        if (!Srv.Get())
          throw TCPServerException("Empty server pointer");
        if (Srv->Start(host.c_str(), port, threadCount, maxConnectCount, factory.Get()) != IFaces::retOk)
          throw TCPServerException("Can't start server");
      }

      TCPServer::~TCPServer()
      {
      }

      Common::CharVectorPtr DefSendStrategy::PrepareData(const void *data, unsigned bytes)
      {
        Common::CharVectorPtr Buf(new Common::CharVector(bytes, 0));
        const char *Data = reinterpret_cast<const char *>(data);
        for (unsigned i = 0 ; i < bytes ; ++i)
          (*Buf.get())[i] = Data[i];
        return Buf;
      }

      DefRecvStrategy::~DefRecvStrategy()
      {
      }

      bool DefRecvStrategy::AssignData(const void *data, unsigned bytes, Common::ISynObj &synObj)
      {
        try
        {
          Common::RefObjPtr<IFaces::IVariant> Var = IFacesImpl::CreateVariant(synObj);
          IFacesImpl::IVariantHelper(Var).SetBinaryData(data, bytes);
          return OnData(Var);
        }
        catch (std::exception &)
        {
        }
        return false;
      }

      bool DefRecvStrategy::OnData(IVariantPtr val)
      {
        return false;
      }

      TCPClient::TCPClient(ITCPClientPtr client)
        : Client(client)
      {
        if (!Client.Get())
          throw TCPClientException("Empty client pointer");
      }

      void TCPClient::Connect(const std::string &host, unsigned short port)
      {
        if (Client->Connect(host.c_str(), port) != IFaces::retOk)
          throw TCPClientException("Error connect");
      }

      void TCPClient::Disconnect()
      {
        if (Client->Disconnect() != IFaces::retOk)
          throw TCPClientException("Error disconnect");
      }

      bool TCPClient::IsConnected() const
      {
        bool Connected = false;
        if (Client->IsConnected(&Connected) != IFaces::retOk)
          throw TCPClientException("Error get connection flag");
        return Connected;
      }

      void TCPClient::SendData(const void *data, unsigned bytes)
      {
        if (Client->SendData(data, bytes) != IFaces::retOk)
          throw TCPClientException("Error send data");
      }

      Common::RefObjPtr<IFaces::IRawDataBuffer> TCPClient::Recv(unsigned long timeout)
      {
        Common::RefObjPtr<IFaces::IRawDataBuffer> Buffer;
        if (Client->Recv(Buffer.GetPPtr(), timeout) != IFaces::retOk)
          throw TCPClientException("Error recv data");
        return Buffer;
      }

    }
  }
}
