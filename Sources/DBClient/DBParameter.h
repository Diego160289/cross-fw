//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBPARAMETER_H__
#define __DBPARAMETER_H__

#include "IFacesTools.h"
#include "DBClient.h"
#include "DBWrap.h"


class IParameterImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::DBIFaces::IParameter)
      >
{
public:
  DECLARE_UUID(02cfe939-7fb3-499a-9241-04f2f5a54c16)

  IParameterImpl();

  // IParameter
  virtual void SetParam(long value);
  virtual void SetParam(unsigned long value);
  virtual void SetParam(double value);
  virtual void SetParam(const char *value);
  virtual void SetParam(const wchar_t *value);
  virtual void SetParam(const IFaces::DBIFaces::DateTime &value);
  virtual void SetParam(const IFaces::DBIFaces::Date &value);
  virtual void SetParam(const IFaces::DBIFaces::Time &value);
  virtual void SetParam(bool value);

  bool FinalizeCreate();
  void BeforeDestroy();

  void Init(Common::SharedPtr<DB::Connection> connection, Common::SharedPtr<DB::Statement> statement, unsigned index);

private:
  Common::SharedPtr<DB::Connection> Connection;
  Common::SharedPtr<DB::Statement> Statement;
  unsigned Index;
};

#endif  // !__DBPARAMETER_H__
