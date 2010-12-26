//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBTRANSACTION_H__
#define __DBTRANSACTION_H__

#include "IFacesTools.h"
#include "DBClient.h"
#include "DBWrap.h"

class ITransactionImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::DBIFaces::ITransaction)
      >
{
public:
  DECLARE_UUID(4317bbea-34ec-4091-8d5e-056810a444fa)

  // ITransaction
  virtual IFaces::RetCode Commit();
  virtual IFaces::RetCode Rollback();

  virtual bool FinalizeCreate();
  virtual void BeforeDestroy();

  void Init(Common::SharedPtr<DB::Connection> connection);

private:
  Common::SharedPtr<DB::Connection> Connection;
  Common::SharedPtr<DB::Transaction> Transaction;
};

#endif  // !__DBTRANSACTION_H__
