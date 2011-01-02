//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

#include "ComponentUUIDs.h"
#include "IFacesTools.h"
#include "DBClient.h"
#include "DBWrap.h"


class IConnectionImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::DBIFaces::IConnection)
      >
{
public:
  DECLARE_STR_UUID(UUID_DB_ICONNECTIONIMPL)

  // IConnection
  virtual IFaces::RetCode Connect(const char *connectionString, const char *userName, const char *password);
  virtual IFaces::RetCode Disconnect();
  virtual IFaces::RetCode CreateTransaction(IFaces::DBIFaces::ITransaction **transaction);
  virtual IFaces::RetCode CreateStatement(IFaces::DBIFaces::IStatement **statement);

  virtual bool FinalizeCreate();
  virtual void BeforeDestroy();

private:
  Common::SharedPtr<DB::Connection> Connection;
};

#endif  // !__DBCONNECTION_H__
