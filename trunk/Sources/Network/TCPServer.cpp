//============================================================================
// Date        : 27.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "TCPServer.h"

IFaces::RetCode ITCPServerImpl::Start(const char *host,
                                      unsigned short port,
                                      unsigned threadCount,
                                      unsigned maxConnectCount,
                                      IFaces::Network::IClientFactory *factory)
{
  Common::ISyncObject Locker(GetSynObj());
  return IFaces::retNotImpl;
}

IFaces::RetCode ITCPServerImpl::Stop()
{
  Common::ISyncObject Locker(GetSynObj());
  return IFaces::retNotImpl;
}
