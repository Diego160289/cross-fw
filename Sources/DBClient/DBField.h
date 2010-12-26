//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBFIELD_H__
#define __DBFIELD_H__

#include "IFacesTools.h"
#include "DBClient.h"


class IFieldImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::DBIFaces::IField)
      >
{
public:
  DECLARE_UUID(e609001b-57b9-482c-aabb-0eb8dd3ef1f9)

  // IField
  virtual IFaces::RetCode GetField(long *value) const;
  virtual IFaces::RetCode GetField(unsigned long *value) const;
  virtual IFaces::RetCode GetField(double *value) const;
  virtual IFaces::RetCode GetStrField(IFaces::IVariant **value, bool asUnicode) const;
  virtual IFaces::RetCode GetField(IFaces::DBIFaces::DateTime *value) const;
  virtual IFaces::RetCode GetField(IFaces::DBIFaces::Date *value) const;
  virtual IFaces::RetCode GetField(IFaces::DBIFaces::Time *value) const;
  virtual IFaces::RetCode GetField(bool *value) const;

private:
};

#endif  // !__DBFIELD_H__
