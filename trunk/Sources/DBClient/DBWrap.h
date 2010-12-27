//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBWRAP_H__
#define __DBWRAP_H__

#include <string>
#include <memory>
#include <map>
#include <ctime>
#include <vector>

#include "Pointers.h"
#include "NoCopyable.h"
#include "Exceptions.h"


namespace DB
{

  DECLARE_RUNTIME_EXCEPTION(Environment)

  class Environment
    : private Common::NoCopyable
  {
  public:
    enum EnvironmentError
    {
      eeErrorAllocate,
      eeErrorSetODBCVersion
    };

    class EnvironmentHolder;

    Environment();
    ~Environment();

    EnvironmentHolder& GetHolder() const;

  private:
    EnvironmentHolder *Holder;
  };


  class Connection;

  DECLARE_RUNTIME_EXCEPTION(Transaction)

  class Transaction
    : public Common::NoCopyable
  {
  public:
    enum TransactionError
    {
      teErrorSaveOptions,
      teErrorInitTransactio,
      teErrorCommit,
      teErrorRollback
    };

    Transaction(Connection &conn);
    virtual ~Transaction();

    void Commit();
    void Rollback();

  private:
    Connection &Conn;
    unsigned long SavedOption;
    bool Commited;
  };


  DECLARE_RUNTIME_EXCEPTION(Connection)

  class Connection
    : private Common::NoCopyable
  {
  public:
    enum ConnectionError
    {
      ceErrorAllocConnection,
      ceErrorConnect
    };

    class ConnectionHolder;

    Connection(const std::string &dsn, const std::string &userName, const std::string &password);
    virtual ~Connection();

    ConnectionHolder& GetHolder() const;
    Environment& GetEnv() const;

  private:
    std::auto_ptr<Environment> Env;
    ConnectionHolder *ConnHldr;
  };


  class Statement;

  DECLARE_RUNTIME_EXCEPTION(Parameter)

  class Parameter
    : private Common::NoCopyable
  {
  public:
    enum ParameterError
    {
      peErrorSetParameter
    };
    Parameter(Statement &stmt, unsigned index);
    void SetParam(long value);
    void SetParam(unsigned long value);
    void SetParam(double value);
    void SetParam(const std::string &value);
    void SetParam(const std::tm &value);
    void SetParam(const char *value);
    void SetParam(bool value);

  private:
    Statement &Stmt;
    unsigned Index;
    std::vector<char> FlagBuf;
    long LongValue;
    unsigned long ULongValue;
    double DoubleValue;
    std::string StringValue;
  };

  typedef Common::SharedPtr<Parameter> ParameterPtr;
  typedef std::map<unsigned, ParameterPtr> ParameterPool;


  struct TimeStruct
  {
    unsigned Hour;
    unsigned Min;
    unsigned Sec;
  };

  DECLARE_RUNTIME_EXCEPTION(Field)

  class Field
    : private Common::NoCopyable
  {
  public:
    enum FieldError
    {
      feErrorGetField,
      feBadFormat
    };
    virtual ~Field();
    virtual bool GetAsBool() const = 0;
    virtual long GetAsLong() const = 0;
    virtual long GetAsULong() const = 0;
    virtual double GetAsDouble() const = 0;
    virtual std::tm GetAsTimestamp() const = 0;
    virtual TimeStruct GetAsTime() const = 0;
    virtual std::string GetAsString() const = 0;
  };

  typedef Common::SharedPtr<Field> FieldPtr;


  DECLARE_RUNTIME_EXCEPTION(Statement)

  class Statement
    : private Common::NoCopyable
  {
  public:
    enum StatementError
    {
      srErrorAllocStatement,
      seErrorPrepare,
      seErrorExecute,
      seErrorFetch,
      seParameterNotExists,
      seErrorResetParameters,
      seErrorGetFieldsCount,
      seErrorCloseCursor
    };

    class StatementHolder;

    Statement(Connection &conn, const std::string &stmt = std::string());
    virtual ~Statement();

    void Prepare(const std::string &query);

    void ClearParameters();
    Parameter& GetParameter(unsigned index);
    const Parameter& GetParameter(unsigned index) const;
    void ResetParameters();

    void Execute();
    void ExecuteDirect(const std::string &query);
    
    bool Fetch();
    FieldPtr GetField(unsigned index) const;
    unsigned GetFieldsCount() const;

    StatementHolder& GetHolder() const;

  private:
    StatementHolder *StmtHldr;
    ParameterPool Parameters;
    bool IsOpen;
    void CloseCursor();
  };

}

#endif  // !__DBWRAP_H__
