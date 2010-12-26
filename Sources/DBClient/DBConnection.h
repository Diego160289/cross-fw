//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

#include "IFacesTools.h"
#include "DBClient.h"


class IConnectionImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::DBIFaces::IConnection)
      >
{
public:
  DECLARE_UUID(ce5210ce-1cdc-4c1f-a493-3b4cd1063fbf)

  // IConnection
  virtual IFaces::RetCode Connect(const char *connectionString);
  virtual IFaces::RetCode CreateTransaction(IFaces::DBIFaces::ITransaction **transaction);
  virtual IFaces::RetCode CreateStatement(IFaces::DBIFaces::IStatement **statement);

private:
};

#endif  // !__DBCONNECTION_H__
