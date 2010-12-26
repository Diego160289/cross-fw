//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SOCKS_H__
#define __SOCKS_H__

#include <string>
#include <vector>
#include <queue>
#include <memory>

#include "NoCopyable.h"
#include "Exceptions.h"
#include "Mutex.h"
#include "SyncObj.h"
#include "Thread.h"
#include "ManualEvent.h"
#include "SimpleIFaces.h"

#include "SocksImpl.h"


namespace Socks
{

  DECLARE_RUNTIME_EXCEPTION(Socket)

  class Socket
    : private Common::NoCopyable
  {
  public:
    enum SocketType { stTCP, stUDP };
    Socket(SocketType type);
    Socket(SocketHandle handle);
    virtual ~Socket();
    void SetNonBlockingFalg(bool isNonBlocking);
    void Bind(const char *url, unsigned short port);
    void Connect(const char *url, unsigned short port);
    void Listen(int maxQueueLen = 1000);
    void Accept(Socket *sock);

  private:
    SocketHandle SockHandle;
    bool InitSockLib;

  protected:
    SocketHandle GetHandle() const;
    void InitSocket(SocketType type);
    void CloseSocket();
  };

  struct ICtrlItem
  {
    virtual ~ICtrlItem() {}
    virtual void CloseMe() = 0;
    virtual bool SendData(const void *data, unsigned bytes) = 0;
  };

  struct IClient
  {
    virtual ~IClient() {}
    virtual bool OnData(const void *data, unsigned bytes) = 0;
  };

  typedef Common::SharedPtr<IClient> IClientPtr;

  struct IClientFactory
  {
    virtual ~IClientFactory() {}
    virtual IClientPtr CreateClient(ICtrlItem &ctrl) = 0;
  };

  typedef Common::SharedPtr<IClientFactory> IClientFactoryPtr;


  class TCPServer
    : protected Socket
  {
  public:
    TCPServer(const std::string &host, unsigned short port,
              unsigned threadCount, unsigned maxConnectCount,
              IClientFactoryPtr factory);
    virtual ~TCPServer();
  private:
    static const long WaitingTimeout;
    volatile bool IsRun;
    IClientFactoryPtr Factory;
    unsigned MaxConnectCount;
    std::auto_ptr<System::Thread> ListenThread;
    System::ManualEvent ClientsEvent;
    typedef Common::SharedPtr<System::Thread> ThreadPtr;
    typedef std::vector<ThreadPtr> ThreadPool;
    ThreadPool WorkingThreads;
    struct IClientItem
    {
      virtual ~IClientItem() {}
      virtual void SetReceiver(IClientPtr receiver) = 0;
      virtual bool Process(volatile const bool *isRun) = 0;
    };
    class IClientItemImpl;
    typedef Common::SharedPtr<IClientItem> IClientItemPtr;
    typedef std::queue<IClientItemPtr> ClientPool;
    System::Mutex ClientsMtx;
    ClientPool Clients;
    void Listener();
    void ProcessClients();
  };


  DECLARE_RUNTIME_EXCEPTION(TCPClient)

  class TCPClient
    : protected Socket
  {
  public:
    typedef std::auto_ptr<std::vector<char> > BufPtr;
    TCPClient();
    virtual ~TCPClient();
    void Connect(const std::string &host, unsigned short port);
    void Disconnect();
    bool IsConnected() const;
    void SendData(const void *data, unsigned bytes);
    BufPtr Recv(unsigned long timeout);
  };

}

#endif  // !__SOCKS_H__
