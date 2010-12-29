//============================================================================
// Date        : 27.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "TCPServer.h"


namespace
{
  class ClientItem
    : private Common::NoCopyable
    , public Socks::ICtrlItem
  {
  public:
    ClientItem(Socks::ICtrlItem &ctrl)
      : Ctrl(ctrl)
    {
    }
    virtual void CloseMe()
    {
    }
    virtual bool SendData(const void *data, unsigned bytes)
    {
      return false;
    }

  private:
    Socks::ICtrlItem &Ctrl;
  };

  class ClientFactory
    : private Common::NoCopyable
    , public Socks::IClientFactory
  {
  public:
    ClientFactory(IFaces::Network::IClientFactory *factory)
      : Factory(factory)
    {
    }
    virtual Socks::IClientPtr CreateClient(Socks::ICtrlItem &ctrl)
    {
      //Factory->CreateClient();
      return Socks::IClientPtr(0);
    }

  private:
    Common::RefObjPtr<IFaces::Network::IClientFactory> Factory;
  };
}

IFaces::RetCode ITCPServerImpl::Start(const char *host,
                                      unsigned short port,
                                      unsigned threadCount,
                                      unsigned maxConnectCount,
                                      IFaces::Network::IClientFactory *factory)
{
  if (!host || !factory)
    return IFaces::retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  if (Server.Get())
    return IFaces::retFalse;
  try
  {
    Server.Reset(new Socks::TCPServer(host, port, threadCount, maxConnectCount,
      Socks::IClientFactoryPtr(new ClientFactory(factory))));
  }
  catch (std::exception &)
  {
    return IFaces::retFail;
  }
  return IFaces::retOk;
}

IFaces::RetCode ITCPServerImpl::Stop()
{
  Common::ISyncObject Locker(GetSynObj());
  return IFaces::retNotImpl;
}
