//============================================================================
// Date        : 27.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "TCPServer.h"


namespace
{

  class ICtrlItemImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::Network::ICtrlItem)
        >
  {
  public:
    DECLARE_UUID(eb463abc-d362-47b0-941d-a3470a75e1ea)

    ICtrlItemImpl()
      : Ctrl(0)
    {
    }

    //ICtrlItem
    virtual IFaces::RetCode CloseMe()
    {
      Common::ISyncObject Locker(GetSynObj());
      if (!Ctrl)
        return IFaces::retFail;
      Ctrl->CloseMe();
      return IFaces::retOk;
    }
    virtual IFaces::RetCode SendData(const void *data, unsigned bytes)
    {
      if (!data || !bytes)
        return IFaces::retBadParam;
      Common::ISyncObject Locker(GetSynObj());
      if (!Ctrl)
        return IFaces::retFail;
      try
      {
        Ctrl->SendData(data, bytes);
      }
      catch (std::exception &)
      {
        return IFaces::retFail;
      }
      return IFaces::retOk;
    }

    void SetCtrl(Socks::ICtrlItem *ctrl)
    {
      Common::ISyncObject Locker(GetSynObj());
      Ctrl = ctrl;
    }

  private:
    Socks::ICtrlItem *Ctrl;
  };

  class IClientImpl
    : private Common::NoCopyable
    , public Socks::IClient
  {
  public:
    IClientImpl(Socks::ICtrlItem &ctrl,
                Common::RefObjPtr<ICtrlItemImpl> ctrlItem,
                Common::RefObjPtr<IFaces::Network::IClient> client)
      : CtrlItem(ctrlItem)
      , Client(client)
    {
      CtrlItem->SetCtrl(&ctrl);
    }
    ~IClientImpl()
    {
      CtrlItem->SetCtrl(0);
    }
    virtual bool OnData(const void *data, unsigned bytes)
    {
      return Client->OnData(data, bytes) == IFaces::retOk;
    }

  private:
    Common::RefObjPtr<ICtrlItemImpl> CtrlItem;
    Common::RefObjPtr<IFaces::Network::IClient> Client;
  };

  class ClientFactory
    : private Common::NoCopyable
    , public Socks::IClientFactory
  {
  public:
    ClientFactory(Common::ISynObj &synObj, IFaces::Network::IClientFactory *factory)
      : SynObj(synObj)
      , Factory(factory)
    {
    }
    virtual Socks::IClientPtr CreateClient(Socks::ICtrlItem &ctrl)
    {
      Common::RefObjPtr<ICtrlItemImpl> CtrlItem = Common::IBaseImpl<ICtrlItemImpl>::CreateWithSyn(SynObj);
      Common::RefObjPtr<IFaces::Network::IClient> Client;
      if (Factory->CreateClient(CtrlItem.Get(), Client.GetPPtr()) != IFaces::retOk)
        return Socks::IClientPtr(0);
      Socks::IClientPtr Item(new IClientImpl(ctrl, CtrlItem, Client));
      return Item;
    }

  private:
    Common::ISynObj &SynObj;
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
      Socks::IClientFactoryPtr(new ClientFactory(GetSynObj(), factory))));
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
  if (!Server.Get())
    return IFaces::retFail;
  Server.Release();
  return IFaces::retOk;
}
