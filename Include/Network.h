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
#include "IFacesTools.h"
#include "IVariantImpl.h"
#include "Typedefs.h"
#include "Pointers.h"


namespace Common
{
  namespace Wrappers
  {
    namespace Network
    {

      typedef Common::RefObjPtr<IFaces::IVariant> IVariantPtr;

      typedef Common::RefObjPtr<IFaces::Network::ITCPServer> ITCPServerPtr;
      typedef Common::RefObjPtr<IFaces::Network::IClientFactory> IClientFactoryPtr;
      typedef Common::RefObjPtr<IFaces::Network::ICtrlItem> ICtrlItemPtr;
      
      typedef Common::RefObjPtr<IFaces::Network::ITCPClient> ITCPClientPtr;

      DECLARE_RUNTIME_EXCEPTION(TCPServer)

      class TCPServer
      {
      public:
        TCPServer(ITCPServerPtr server, const std::string &host, unsigned short port,
          unsigned threadCount, unsigned maxConnectCount, IClientFactoryPtr factory);
        virtual ~TCPServer();

      private:
        ITCPServerPtr Srv;
      };

      template <typename T>
      class IClientFactoryImpl
        : public Common::CoClassBase
            <
              TYPE_LIST_1(IFaces::Network::IClientFactory)
            >
      {
      public:
        DECLARE_UUID(b702c3f8-3910-4c78-abe5-2324ac10c3a9)

        // IClientFactory
        virtual IFaces::RetCode CreateClient(IFaces::Network::ICtrlItem *ctrl, IFaces::Network::IClient **client)
        {
          Common::RefObjPtr<T> Item = Common::IBaseImpl<T>::CreateWithSyn(GetSynObj());
          try
          {
            Item->SetCtrl(ctrl);
          }
          catch (std::exception &)
          {
            return IFaces::retFail;
          }
          return Item.QueryInterface(client);
        }
      };

      class DefSendStrategy
      {
      public:
        virtual Common::CharVectorPtr PrepareData(const void *data, unsigned bytes);
      };

      DECLARE_RUNTIME_EXCEPTION(CtrlItem)

      template <typename TSendStrategy = SendStrategy>
      class CtrlItem
        : public TSendStrategy
      {
      public:
        CtrlItem(ICtrlItemPtr ctrl)
          : Ctrl(ctrl)
        {
          if (!Ctrl.Get())
            throw CtrlItemException("Empty ctrl item pointer");
        }
        virtual ~CtrlItem()
        {
        }
        void CloseMe()
        {
          if (Ctrl->CloseMe() != IFaces::retOk)
            throw CtrlItemException("Error post message for close me");
        }
        void SendData(const void *data, unsigned bytes)
        {
          Common::CharVectorPtr Buf = this->PrepareData(data, bytes);
          if (Ctrl->SendData(Buf.get() ? &Buf->front() : 0, Buf.get() ? static_cast<unsigned>(Buf->size()) : 0) != IFaces::retOk)
            throw CtrlItemException("Error send data");
        }

      private:
        ICtrlItemPtr Ctrl;
      };

      class DefRecvStrategy
      {
      public:
        virtual ~DefRecvStrategy();
        bool AssignData(const void *data, unsigned bytes, Common::ISynObj &synObj);
        virtual bool OnData(IVariantPtr val);
      };

      template <typename TRecvStrategy = DefRecvStrategy>
      class IClientImpl
        : public Common::CoClassBase
            <
              TYPE_LIST_1(IFaces::Network::IClient)
            >
        , public TRecvStrategy
      {
      public:
        DECLARE_UUID(741c832a-2890-4069-aac1-34d16827d1f1)

        // IClient
        virtual bool OnData(const void *data, unsigned bytes)
        {
          try
          {
            return this->AssignData(data, bytes, GetSynObj());
          }
          catch (std::exception &)
          {
          }
          return false;
        }

        void SetCtrl(IFaces::Network::ICtrlItem *ctrl)
        {
          Common::ISyncObject Locker(GetSynObj());
          Ctrl = ctrl;
        }
        ICtrlItemPtr GetCtrl() const
        {
          return Ctrl;
        }

      private:
        ICtrlItemPtr Ctrl;
      };

      template
      <
        typename TRecvStrategy = DefRecvStrategy,
        typename TSendStrategy = DefSendStrategy
      >
      class Server
        : private Common::NoCopyable
      {
      public:
        Server(ITCPServerPtr srv, Common::ISynObj &synObj,
              const std::string &host, unsigned short port,
              unsigned threadCount, unsigned maxConnectCount)
        {
          Common::RefObjPtr<ServerImpl> NewSrv = Common::IBaseImpl<ServerImpl>::CreateWithSyn(synObj);
          NewSrv->Init(srv, host, port, threadCount, maxConnectCount, this);
          Srv = NewSrv;
        }
        virtual ~Server()
        {
        }
        virtual bool OnData(const IFacesImpl::IVariantHelper &data, CtrlItem<TSendStrategy> ctrl)
        {
          return false;
        }
      private:
        class ServerImpl
          : public IClientImpl<TRecvStrategy>
        {
        public:
          ServerImpl()
            : Owner(0)
          {
          }
          virtual ~ServerImpl()
          {
          }
          void Init(ITCPServerPtr srv,
                    const std::string &host, unsigned short port,
                    unsigned threadCount, unsigned maxConnectCount,
                    Server<TRecvStrategy, TSendStrategy> *owner)
          {
            Owner = owner;
            Common::RefObjPtr<IClientFactoryType> NewFactory =
              Common::IBaseImpl<IClientFactoryType>::CreateWithSyn(GetSynObj());
            Common::SharedPtr<TCPServer> NewSrv(new TCPServer(srv, host, port, threadCount, maxConnectCount, NewFactory));
            Srv = NewSrv;
            Factory = NewFactory;
          }
          virtual bool OnData(IVariantPtr val)
          {
            try
            {
              return Owner->OnData(val, GetCtrl());
            }
            catch (std::exception &)
            {
            }
            return false;
          }
        private:
          typedef IClientFactoryImpl<ServerImpl> IClientFactoryType;
          Server<TRecvStrategy, TSendStrategy> *Owner;
          Common::RefObjPtr<IClientFactoryType> Factory;
          Common::SharedPtr<TCPServer> Srv;
        };
        Common::RefObjPtr<ServerImpl> Srv;
      };

      DECLARE_RUNTIME_EXCEPTION(TCPClient)

      class TCPClient
      {
      public:
        TCPClient(ITCPClientPtr client);
        void Connect(const std::string &host, unsigned short port);
        void Disconnect();
        bool IsConnected() const;
        void SendData(const void *data, unsigned bytes);
        Common::RefObjPtr<IFaces::IRawDataBuffer> Recv(unsigned long timeout);

      private:
        ITCPClientPtr Client;
      };
    }
  }
}

#endif  // !__NETWORK_H__
