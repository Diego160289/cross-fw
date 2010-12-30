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
        : Server(server)
      {
        if (!Server.Get())
          throw TCPServerException("Empty server pointer");
        if (Server->Start(host.c_str(), port, threadCount, maxConnectCount, factory.Get()) != IFaces::retOk)
          throw TCPServerException("Can't start server");
      }

      TCPServer::~TCPServer()
      {
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

    }
  }
}
