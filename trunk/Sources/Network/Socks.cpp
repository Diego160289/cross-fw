//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Socks.h"


namespace Socks
{

  Socket::Socket(SocketType type)
    : SockHandle(InvalidSocket)
    , InitSockLib(false)
  {
    InitSocket(type);
  }

  Socket::Socket(SocketHandle handle)
    : SockHandle(handle)
    , InitSockLib(false)
  {
  }

  Socket::~Socket()
  {
    CloseSocket();
    if (InitSockLib)
      SocketInitializer::GetInst().Done();
  }

  void Socket::SetNonBlockingFalg(bool isNonBlocking)
  {
    if (!SetNonBlockingFalgImpl(SockHandle, isNonBlocking))
      throw SocketException("Error set non blocking flag");
  }

  void Socket::Bind(const char *url, unsigned short port)
  {
    sockaddr_in Addr = { 0 };
    FillAddr(url, port, &Addr);
    if (::bind(SockHandle, reinterpret_cast<sockaddr *>(&Addr), sizeof(Addr)) == -1)
      throw SocketException("Error bind socket");
  }
  
  void Socket::Connect(const char *url, unsigned short port)
  {
    sockaddr_in Addr = { 0 };
    FillAddr(url, port, &Addr);
    if (::connect(SockHandle, reinterpret_cast<sockaddr *>(&Addr), sizeof(Addr)) == -1)
      throw SocketException("Error connect socket");
  }

  void Socket::Listen(int maxQueueLen)
  {
    if (::listen(SockHandle, maxQueueLen) == -1)
      throw SocketException("Error listen socket");
  }

  void Socket::Accept(Socket *sock)
  {
    SocketHandle NewSock = InvalidSocket;
    if ((NewSock = ::accept(SockHandle, 0, 0)) == InvalidSocket)
      throw SocketException("Error accept socket");
    Socket &Sock = *sock;
    Sock.SockHandle = NewSock;
  }

  SocketHandle Socket::GetHandle() const
  {
    return SockHandle;
  }

  void Socket::InitSocket(SocketType type)
  {
    SocketInitializer::GetInst().Init();
    int Type = 0;
    switch (type)
    {
    case stTCP :
      Type = SOCK_STREAM;
      break;
    case stUDP :
      Type = SOCK_DGRAM;
      break;
    default :
      throw SocketException("Unsupported socket type");
    };
    if ((SockHandle = ::socket(AF_INET, Type, IPPROTO_TCP)) == InvalidSocket)
    {
      SocketInitializer::GetInst().Done();
      throw SocketException("Error create socket");
    }
    InitSockLib = true;
  }

  void Socket::CloseSocket()
  {
    if (SockHandle != InvalidSocket)
      CloseSocketImpl(SockHandle);
    SockHandle = InvalidSocket;
  }


  class TCPServer::IClientItemImpl
    : public IClientItem
    , public Socket
    , public ICtrlItem
  {
  public:
    IClientItemImpl(SocketHandle sock)
      : Socket(sock)
      , OpenFlag(true)
    {
    }
    virtual ~IClientItemImpl()
    {
    }
    virtual void SetReceiver(IClientPtr receiver)
    {
      Receiver = receiver;
    }
    virtual bool Process(volatile const bool *isRun)
    {
      SocketHandle Sock = GetHandle();
      std::vector<char> Buffer;
      while (*isRun)
      {
        fd_set RDSet = { 0 };
        FD_ZERO(&RDSet);
        FD_SET(Sock, &RDSet);
        fd_set RDErr = { 0 };
        FD_ZERO(&RDErr);
        FD_SET(Sock, &RDErr);
        timeval TV = { 0 };
        TV.tv_sec = SelectTimeout / 1000;
        TV.tv_usec = SelectTimeout % 1000;
        int Ret = ::select(Sock + 1, &RDSet, 0, &RDErr, &TV);
        if (Ret < 0)
        {
          OpenFlag = false;
          break;
        }
        if (!Ret)
        {
          if (!Buffer.empty())
            OpenFlag = Receiver->OnData(&Buffer[0], static_cast<unsigned>(Buffer.size()));
          return OpenFlag;
        }
        if (!FD_ISSET(Sock, &RDSet) || FD_ISSET(Sock, &RDErr))
        {
          OpenFlag = false;
          return false;
        }
        const int BufSize = 4096;
				std::vector<char> TmpBuf(BufSize, 0);
        int RecvBytes = ::recv(Sock, &TmpBuf[0], static_cast<int>(TmpBuf.size()), 0);
        if (RecvBytes <= 0)
        {
          OpenFlag = false;
          return false;
        }
        for (int i = 0 ; i < RecvBytes ; ++i)
          Buffer.push_back(TmpBuf[i]);
      }
      if (!*isRun)
      {
        OpenFlag = false;
        return false;
      }
      if (!OpenFlag)
        return false;
      if (!Buffer.empty())
        OpenFlag = Receiver->OnData(&Buffer[0], static_cast<unsigned>(Buffer.size()));
      return OpenFlag;
    }
    virtual void CloseMe()
    {
      OpenFlag = false;
    }
    virtual bool SendData(const void *data, unsigned bytes)
    {
      if (!data || !bytes)
        return false;
      const char *Buf = reinterpret_cast<const char *>(data);
      unsigned SentBytes = 0;
      do
      {
        int Ret = ::send(GetHandle(), &Buf[SentBytes], bytes - SentBytes, 0);
        if (Ret < 0)
          return false;
        if (!Ret && SentBytes < bytes)
          return false;
        SentBytes += Ret;
      } while (SentBytes < bytes);
      return true;
    }

  private:
    static const long SelectTimeout;
    volatile bool OpenFlag;
    IClientPtr Receiver;
  };

  const long TCPServer::IClientItemImpl::SelectTimeout = 50;


  const long TCPServer::WaitingTimeout = 1000;

  TCPServer::TCPServer(const std::string &host, unsigned short port,
            unsigned threadCount, unsigned maxConnectCount,
            IClientFactoryPtr factory)
    : Socket(stTCP)
    , IsRun(true)
    , Factory(factory)
    , MaxConnectCount(maxConnectCount)
  {
    Bind(host.c_str(), port);
    Listen();
    ClientsEvent.Reset();
    ListenThread.reset(new System::Thread(Common::CreateMemberCallback(*this, &TCPServer::Listener)));
    for (unsigned i = 0 ; i < threadCount ; ++i)
      WorkingThreads.push_back(ThreadPtr(new System::Thread(Common::CreateMemberCallback(*this, &TCPServer::ProcessClients))));
  }

  TCPServer::~TCPServer()
  {
    IsRun = false;
    {
      Common::SyncObject<System::Mutex> Locker(ClientsMtx);
      ClientsEvent.Set();
      ClientPool EmptyPool;
     Clients = EmptyPool;
    }
    ListenThread.release();
    {
      Common::SyncObject<System::Mutex> Locker(ClientsMtx);
      ClientsEvent.Set();
      ClientPool EmptyPool;
      Clients = EmptyPool;
    }
    WorkingThreads.clear();
    Common::SyncObject<System::Mutex> Locker(ClientsMtx);
  }

  void TCPServer::Listener()
  {
    SocketHandle ListenerSocket = GetHandle();
    while (IsRun)
    {
      fd_set RDSet = { 0 };
      FD_ZERO(&RDSet);
      FD_SET(ListenerSocket, &RDSet);
      fd_set RDErr = { 0 };
      FD_ZERO(&RDErr);
      FD_SET(ListenerSocket, &RDErr);
      timeval TV = { 0 };
      TV.tv_sec = WaitingTimeout / 1000;
      TV.tv_usec = WaitingTimeout % 1000;
      int Ret = ::select(ListenerSocket + 1, &RDSet, 0, &RDErr, &TV);
      if (Ret <= 0)
        continue;
      if (FD_ISSET(ListenerSocket, &RDErr))
        continue;
      try
      {
        if (FD_ISSET(ListenerSocket, &RDSet))
        {
          if (!IsRun)
            break;
          std::auto_ptr<IClientItemImpl> NewClient(new IClientItemImpl(InvalidSocket));
          Accept(NewClient.get());
          if (Factory.Get())
          {
            IClientPtr Client = Factory->CreateClient(*NewClient.get());
            if (Client.Get())
            {
              NewClient->SetReceiver(Client);
              Client.Release();
              Common::SyncObject<System::Mutex> Locker(ClientsMtx);
              if (static_cast<unsigned>(Clients.size()) < MaxConnectCount)
              {
                Clients.push(IClientItemPtr(NewClient.release()));
                ClientsEvent.Set();
              }
            }
          }
        }
      }
      catch (std::exception &)
      {
      }
    }
  }

  void TCPServer::ProcessClients()
  {
    while (IsRun)
    {
      try
      {
        ClientsEvent.Wait();
        if (!IsRun)
          break;
        IClientItemPtr Client;
        {
          Common::SyncObject<System::Mutex> Locker(ClientsMtx);
          if (Clients.empty())
          {
            if (IsRun)
            {
              ClientsEvent.Reset();
              continue;
            }
            break;
          }
          Client = Clients.front();
          Clients.pop();
          if (Clients.empty() && IsRun)
            ClientsEvent.Reset();
        }
        if (IsRun && Client.Get())
        {
          if (IsRun && Client->Process(&IsRun))
          {
            Common::SyncObject<System::Mutex> Locker(ClientsMtx);
            if (static_cast<unsigned>(Clients.size()) < MaxConnectCount && IsRun)
            {
              Clients.push(Client);
              Client.Release();
              ClientsEvent.Set();
            }
          }
        }
      }
      catch (std::exception &)
      {
      }
    }
  }


  TCPClient::TCPClient()
    : Socket(InvalidSocket)
  {
  }
  
  TCPClient::~TCPClient()
  {
  }

  void TCPClient::Connect(const std::string &host, unsigned short port)
  {
    if (IsConnected())
      Disconnect();
    InitSocket(stTCP);
    Socket::Connect(host.c_str(), port);
  }

  void TCPClient::Disconnect()
  {
    if (!IsConnected())
      return;
    CloseSocket();
  }

  bool TCPClient::IsConnected() const
  {
    return GetHandle() != InvalidSocket;
  }
    
  void TCPClient::SendData(const void *data, unsigned bytes)
  {
    if (!data || !bytes)
      throw TCPClientException("Empty data");
    const char *Buf = reinterpret_cast<const char *>(data);
    unsigned SentBytes = 0;
    do
    {
      int Ret = ::send(GetHandle(), &Buf[SentBytes], bytes - SentBytes, 0);
      if (Ret < 0)
      {
        Disconnect();
        throw TCPClientException("Error send data");
      }
      if (!Ret && SentBytes < bytes)
      {
        Disconnect();
        throw TCPClientException("Error send data");
      }
      SentBytes += Ret;
    } while (SentBytes < bytes);
  }

  TCPClient::BufPtr TCPClient::Recv(unsigned long timeout)
  {
    SocketHandle Sock = GetHandle();
    BufPtr Buffer(new std::vector<char>);
    while (Buffer->empty())
    {
      fd_set RDSet = { 0 };
      FD_ZERO(&RDSet);
      FD_SET(Sock, &RDSet);
      fd_set RDErr = { 0 };
      FD_ZERO(&RDErr);
      FD_SET(Sock, &RDErr);
      timeval TV = { 0 };
      TV.tv_sec = timeout / 1000;
      TV.tv_usec = timeout % 1000;
      int Ret = ::select(Sock + 1, &RDSet, 0, &RDErr, &TV);
      if (Ret < 0)
      {
        Disconnect();
        throw TCPClientException("Error receive data");
      }
      if (!Ret)
      {
        if (!Buffer->empty())
          return Buffer;
        return BufPtr(0);
      }
      if (!FD_ISSET(Sock, &RDSet) || FD_ISSET(Sock, &RDErr))
      {
        Disconnect();
        throw TCPClientException("Error receive data");
      }
      const int BufSize = 4096;
			std::vector<char> TmpBuf(BufSize, 0);
      int RecvBytes = ::recv(Sock, &TmpBuf[0], static_cast<int>(TmpBuf.size()), 0);
      if (RecvBytes <= 0)
      {
        Disconnect();
        throw TCPClientException("Error receive data");
      }
      for (int i = 0 ; i < RecvBytes ; ++i)
        Buffer->push_back(TmpBuf[i]);
    }
    return Buffer;
  }

}
