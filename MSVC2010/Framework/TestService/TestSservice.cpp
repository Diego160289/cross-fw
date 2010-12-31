#include "ServiceBase.h"
#include "Module.h"
#include "Mutex.h"
#include "Network.h"


class TestService
  : public Common::ServiceBase<>
{
public:
  DECLARE_UUID(TestService)
  virtual bool OnInit()
  {
    try
    {    
      Srv.Reset(new Common::Wrappers::Network::Server<>(
        CreateObject<IFaces::Network::ITCPServer>("17fde71c-1e91-4984-8cb9-2bd2f4ec8ac6"),
        GetSynObj(),
        "127.0.0.1",
        10001,
        5,
        50
        ));

      Common::Wrappers::Network::TCPClient Client(
        CreateObject<IFaces::Network::ITCPClient>("be565c33-f38b-47e5-8833-889b57fcd29e")
        );
      Client.Connect("127.0.0.1", 10001);
      std::string S = "Test String!";
      Client.SendData(S.c_str(), S.length() + 1);
      Client.Recv(5000);
    }
    catch (std::exception &)
    {
      return false;
    }
    return true;
  }
  virtual void OnDone()
  {
  }

private:
  Common::SharedPtr<Common::Wrappers::Network::Server<> > Srv;
};


DECLARE_MODULE_ENTRY_POINT(
  "TestService",
  TestService,
  System::Mutex,
  TYPE_LIST_1(TestService))
