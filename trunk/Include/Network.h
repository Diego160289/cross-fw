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


namespace Common
{
  namespace Wrappers
  {
    namespace Network
    {

      typedef Common::RefObjPtr<IFaces::Network::ITCPServer> ITCPServerPtr;
      typedef Common::RefObjPtr<IFaces::Network::IClientFactory> IClientFactoryPtr;
      typedef Common::RefObjPtr<IFaces::Network::ICtrlItem> ICtrlItemPtr;

      DECLARE_RUNTIME_EXCEPTION(TCPServer)

      class TCPServer
      {
      public:
        TCPServer(ITCPServerPtr server, const std::string &host, unsigned short port,
          unsigned threadCount, unsigned maxConnectCount, IClientFactoryPtr factory);
        virtual ~TCPServer();

      private:
        ITCPServerPtr Server;
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

      DECLARE_RUNTIME_EXCEPTION(CtrlItem)

      template <typename TSendStrategy>
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
        }

      private:
        ICtrlItemPtr Ctrl;
      };

      class DefRecvStrategy
      {
      public:
        typedef Common::RefObjPtr<IFaces::IVariant> IVariantPtr;
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

      private:
        ICtrlItemPtr Ctrl;
      };
    }
  }
}

#endif  // !__NETWORK_H__
