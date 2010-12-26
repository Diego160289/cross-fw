#ifndef __DBCLIENT_H__
#define __DBCLIENT_H__

#include "IFaces.h"


namespace IFaces
{

  namespace DBIFaces
  {

    struct ITransaction
      : public IBase
    {
      DECLARE_UUID(45458c15-e1aa-433b-acc6-f94f95469d7a)
      virtual RetCode Commit() = 0;
      virtual RetCode Rollback() = 0;
    };

    struct Time
    {
      unsigned short Hour;
      unsigned short Min;
      unsigned short Sec;
    };

    struct Date
    {
      unsigned short Year;
      unsigned short Month;
      unsigned short Day;
    };

    struct DateTime
    {
      Date Dt;
      Time Tm;
    };

    struct IParameter
      : public IBase
    {
      DECLARE_UUID(62d293c0-b6f7-4cb4-a894-5d20aa42977b)
      virtual void SetParam(long value) = 0;
      virtual void SetParam(unsigned long value) = 0;
      virtual void SetParam(double value) = 0;
      virtual void SetParam(const char *value) = 0;
      virtual void SetParam(const wchar_t *value) = 0;
      virtual void SetParam(const DateTime &value) = 0;
      virtual void SetParam(const Date &value) = 0;
      virtual void SetParam(const Time &value) = 0;
      virtual void SetParam(bool value) = 0;
    };

    struct IField
      : public IBase
    {
      DECLARE_UUID(7be61b9f-d7f5-4a64-8eb8-5502902fe4fb)
      virtual RetCode GetField(long *value) const = 0;
      virtual RetCode GetField(unsigned long *value) const = 0;
      virtual RetCode GetField(double *value) const = 0;
      virtual RetCode GetStrField(IVariant **value, bool asUnicode) const = 0;
      virtual RetCode GetField(DateTime *value) const = 0;
      virtual RetCode GetField(Date *value) const = 0;
      virtual RetCode GetField(Time *value) const = 0;
      virtual RetCode GetField(bool *value) const = 0;
    };

    struct IStatement
      : public IBase
    {
      DECLARE_UUID(56d74f23-2285-477a-91e0-b2a907b818c6)
      virtual RetCode Prepare(const char *str) = 0;
      virtual RetCode ExecuteDirect(const char *str) = 0;
      virtual RetCode Execute(const char *str) = 0;
      virtual RetCode Fetch() = 0;
      virtual RetCode GetParameter(IParameter **prm) = 0;
      virtual RetCode GetParameter(IParameter **prm) const = 0;
      virtual RetCode GetField(unsigned index, IField **fld) const = 0;
      virtual RetCode GetFieldsCount(unsigned *count) const = 0;
    };

    struct IConnection
      : public IBase
    {
      DECLARE_UUID({7999427-4881-47bb-af5c-f098d6ac26f1)
      virtual RetCode Connect(const char *connectionString) = 0;
      virtual RetCode CreateTransaction(ITransaction **transaction) = 0;
      virtual RetCode CreateStatement(IStatement **statement) = 0;
    };

  }

}

#endif  // !__DBCLIENT_H__
