//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBSTATEMENT_H__
#define __DBSTATEMENT_H__

#include "IFacesTools.h"
#include "DBClient.h"
#include "DBWrap.h"


class IStatementImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::DBIFaces::IStatement)
      >
{
public:
  DECLARE_UUID(747d13ed-5f6b-4585-aa55-4fd4a315b7d3)

  // IStatement
  virtual IFaces::RetCode Prepare(const char *str);
  virtual IFaces::RetCode ExecuteDirect(const char *str);
  virtual IFaces::RetCode Execute();
  virtual IFaces::RetCode Fetch();
  virtual IFaces::RetCode GetParameter(unsigned index, IFaces::DBIFaces::IParameter **prm);
  virtual IFaces::RetCode GetField(unsigned index, IFaces::DBIFaces::IField **fld) const;
  virtual IFaces::RetCode GetFieldsCount(unsigned *count) const;

  bool FinalizeCreate();
  void BeforeDestroy();
  
  void Init(Common::SharedPtr<DB::Connection> connection);

private:
  Common::SharedPtr<DB::Connection> Connection;
  Common::SharedPtr<DB::Statement> Statement;
};

#endif  // !__DBSTATEMENT_H__
