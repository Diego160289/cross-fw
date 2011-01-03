//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBWrap.h"

#ifdef _MSC_VER
  #pragma warning (disable : 4996)
#endif

#include "DBWrapImpl.h"

#include <sql.h>
#include <sqlext.h>

#include <sstream>
#include <iomanip>

#define TIODBC_SUCCESS_CODE(ret) \
	((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))


namespace
{

  std::string GetLastDBError(SQLSMALLINT handleType, SQLHANDLE handle)
	{
    const unsigned MaxBufLen = 1024;
    SQLCHAR StatusCode[MaxBufLen] = { 0 };
    SQLCHAR ErrorMessage[MaxBufLen] = { 0 };
		SQLINTEGER NativeError = 0;
		SQLSMALLINT TotalBytes = 0;
    SQLRETURN Ret = SQLGetDiagRec(handleType, handle, 1, reinterpret_cast<SQLCHAR *>(&StatusCode),
      &NativeError, reinterpret_cast<SQLCHAR *>(&ErrorMessage), MaxBufLen, &TotalBytes);
		if (TIODBC_SUCCESS_CODE(Ret))
		{
      std::string RetStr = reinterpret_cast<const std::string::value_type *>(ErrorMessage);
      RetStr += "\ncode: ";
      RetStr += reinterpret_cast<const std::string::value_type *>(StatusCode);
			return RetStr;
		}
    return std::string();
	}

  SQLINTEGER* SetParamFlag(std::vector<char> *buff, SQLINTEGER flag)
  {
    *buff = std::vector<char>(sizeof(SQLINTEGER), 0);
    SQLINTEGER *Flag = reinterpret_cast<SQLINTEGER *>(&buff->front());
    *Flag = flag;
    return Flag;
  }

  template <typename T>
  void SetParam(HSTMT stmt, unsigned index, SQLSMALLINT type, SQLSMALLINT sqltype, T *value, SQLINTEGER *flag)
  {
    SQLRETURN Ret = SQLBindParameter(stmt,
      index,
      SQL_PARAM_INPUT,
      type,
      sqltype,
      0,
      0,
      reinterpret_cast<SQLPOINTER *>(value),
      0,
      //reinterpret_cast for x64
      reinterpret_cast<SQLLEN *>(flag)
      );
    if (Ret != SQL_SUCCESS)
      throw DB::ParameterException("Error set parameter", DB::Parameter::peErrorSetParameter);
  }

  void SetParam(HSTMT stmt, unsigned index, const std::string &value, SQLINTEGER *nullTermStringFlag)
  {
    SQLRETURN Ret = SQLBindParameter(stmt,
      index,
      SQL_PARAM_INPUT,
      SQL_C_CHAR,
      SQL_CHAR,
      static_cast<SQLUINTEGER>(value.size()),
      0,
      const_cast<SQLPOINTER *>(reinterpret_cast<const SQLPOINTER *>(value.c_str())),
      static_cast<SQLINTEGER>(value.size() + 1),
      //reinterpret_cast for x64
      reinterpret_cast<SQLLEN *>(nullTermStringFlag));
    if (Ret != SQL_SUCCESS)
      throw DB::ParameterException("Error set parameter", DB::Parameter::peErrorSetParameter);
  }

}

namespace DB
{


  class Environment::EnvironmentHolder
    : private Common::NoCopyable
  {
  public:
    EnvironmentHolder()
    {
      if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &Env) != SQL_SUCCESS)
        throw EnvironmentException("Error allocate environment", eeErrorAllocate);
      if (SQLSetEnvAttr(Env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
      {
        SQLFreeHandle(SQL_HANDLE_ENV, Env);
        throw EnvironmentException("Error set ODBC vrsion", eeErrorSetODBCVersion);
      }
    }
    ~EnvironmentHolder()
    {
      SQLFreeHandle(SQL_HANDLE_ENV, Env);
    }
    SQLHENV GetHandle() const
    {
      return Env;
    }
  private:
    SQLHENV Env;
  };

  Environment::Environment()
    : Holder(new EnvironmentHolder)
  {
  }

  Environment::~Environment()
  {
    delete Holder;
  }

  Environment::EnvironmentHolder& Environment::GetHolder() const
  {
    return *Holder;
  }


  class Connection::ConnectionHolder
    : private Common::NoCopyable
  {
  public:
    ConnectionHolder(Environment &env)
    {
      if (SQLAllocHandle(SQL_HANDLE_DBC, env.GetHolder().GetHandle(), &Conn) != SQL_SUCCESS)
        throw ConnectionException("Error allocate connection", ceErrorAllocConnection);
    }
    ~ConnectionHolder()
    {
      SQLFreeHandle(SQL_HANDLE_DBC, Conn);
    }
    SQLHDBC GetHandle() const
    {
      return Conn;
    }
  private:
    SQLHDBC Conn;
  };


  Connection::Connection(const std::string &dsn,
                         const std::string &userName,
                         const std::string &password)
    : Env(new Environment)
    , ConnHldr(0)
  {
    std::auto_ptr<ConnectionHolder> NewConnHldr(new ConnectionHolder(*Env.get()));
    SQLRETURN Ret = SQLConnect(NewConnHldr->GetHandle(),
      const_cast<SQLCHAR *>(reinterpret_cast<const SQLTCHAR *>(dsn.c_str())), SQL_NTS,
      !userName.empty() ? const_cast<SQLCHAR *>(reinterpret_cast<const SQLCHAR *>(userName.c_str())) : 0, SQL_NTS,
      !password.empty() ? const_cast<SQLCHAR *>(reinterpret_cast<const SQLCHAR *>(password.c_str())) : 0, SQL_NTS);
    if (Ret != SQL_SUCCESS)
      throw ConnectionException(GetLastDBError(SQL_HANDLE_DBC, NewConnHldr->GetHandle()), ceErrorConnect);
    ConnHldr = NewConnHldr.release();
  }

  Connection::~Connection()
  {
    SQLDisconnect(ConnHldr->GetHandle());
    delete ConnHldr;
  }

  Connection::ConnectionHolder& Connection::GetHolder() const
  {
    return *ConnHldr;
  }

  Environment& Connection::GetEnv() const
  {
    return *Env.get();
  }


  Transaction::Transaction(Connection &conn)
    : Conn(conn)
    , SavedOption(0)
    , Commited(false)
  {
    SQLRETURN Ret = SQLGetConnectOption(Conn.GetHolder().GetHandle(), SQL_AUTOCOMMIT, reinterpret_cast<SQLPOINTER *>(&SavedOption));
    if (Ret != SQL_SUCCESS)
      throw TransactionException("Error save options", teErrorSaveOptions);
    Ret = SQLSetConnectOption(Conn.GetHolder().GetHandle(), SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
    if (Ret != SQL_SUCCESS)
      throw TransactionException("Error get init transaction", teErrorInitTransactio);
  }

  Transaction::~Transaction()
  {
    if (!Commited)
    {
      try
      {
        Rollback();
      }
      catch (std::exception &)
      {
      }
    }
    SQLSetConnectOption(Conn.GetHolder().GetHandle(), SQL_AUTOCOMMIT, SavedOption);
  }

  void Transaction::Commit()
  {
    SQLRETURN Ret = SQLTransact(Conn.GetEnv().GetHolder().GetHandle(), Conn.GetHolder().GetHandle(), SQL_COMMIT);
    if (Ret != SQL_SUCCESS)
      throw TransactionException("Error commit transaction", teErrorCommit);
    Commited = true;
  }

  void Transaction::Rollback()
  {
    SQLRETURN Ret = SQLTransact(Conn.GetEnv().GetHolder().GetHandle(), Conn.GetHolder().GetHandle(), SQL_ROLLBACK);
    if (Ret != SQL_SUCCESS)
      throw TransactionException("Error rollback transaction", teErrorRollback);
    Commited = false;
  }


  class Statement::StatementHolder
    : public Common::NoCopyable
  {
  public:
    StatementHolder(Connection &conn)
    {
      if (SQLAllocHandle(SQL_HANDLE_STMT, conn.GetHolder().GetHandle(), &Stmt) != SQL_SUCCESS)
        throw StatementException("Error alloc statement", srErrorAllocStatement);
    }
    ~StatementHolder()
    {
      SQLFreeHandle(SQL_HANDLE_STMT, Stmt);
    }
    SQLHSTMT GetHandle() const
    {
      return Stmt;
    }
  private:
    SQLHSTMT Stmt;
  };

  Statement::Statement(Connection &conn, const std::string &stmt)
    : StmtHldr(new StatementHolder(conn))
    , IsOpen(false)
  {
    if (!stmt.empty())
    {
      try
      {
        Prepare(stmt);
      }
      catch (std::exception &)
      {
        delete StmtHldr;
        throw;
      }
    }
  }

  Statement::~Statement()
  {
    try
    {
      CloseCursor();
    }
    catch (std::exception &)
    {
    }
    delete StmtHldr;
  }

  void Statement::Prepare(const std::string &query)
  {
    if (SQLPrepare(StmtHldr->GetHandle(), const_cast<SQLCHAR *>(reinterpret_cast<const SQLCHAR *>(query.c_str())), SQL_NTS) != SQL_SUCCESS)
      throw StatementException("Error prepare statement", seErrorPrepare);
  }

  void Statement::ClearParameters()
  {
    Parameters.clear();
  }

  Parameter& Statement::GetParameter(unsigned index)
  {
    ++index;
    ParameterPool::const_iterator Iter = Parameters.find(index);
    if (Iter == Parameters.end())
      return *(Parameters[index] = ParameterPtr(new Parameter(*this, index))).Get();
    return *Iter->second.Get();
  }

  const Parameter& Statement::GetParameter(unsigned index) const
  {
    ++index;
    ParameterPool::const_iterator Iter = Parameters.find(index);
    if (Iter == Parameters.end())
      throw StatementException("Parameters not exists", seParameterNotExists);
    return *Iter->second.Get();
  }

  void Statement::ResetParameters()
  {
    if (SQLFreeStmt(StmtHldr->GetHandle(), SQL_RESET_PARAMS) != SQL_SUCCESS)
      throw StatementException("Error reset parameters", seErrorResetParameters);
    Parameters.clear();
  }

  void Statement::Execute()
  {
    CloseCursor();
    SQLRETURN RetCode = SQLExecute(StmtHldr->GetHandle());
    if (RetCode != SQL_SUCCESS)
      throw StatementException(GetLastDBError(SQL_HANDLE_STMT, StmtHldr->GetHandle()), seErrorExecute);
    IsOpen = true;
  }

  void Statement::ExecuteDirect(const std::string &query)
  {
    CloseCursor();
    if (SQLExecDirect(StmtHldr->GetHandle(), const_cast<SQLCHAR *>(reinterpret_cast<const SQLCHAR *>(query.c_str())), SQL_NTS) != SQL_SUCCESS)
      throw StatementException(GetLastDBError(SQL_HANDLE_STMT, StmtHldr->GetHandle()), seErrorExecute);
    IsOpen = true;
  }
    
  bool Statement::Fetch()
  {
    SQLRETURN Ret = SQLFetch(StmtHldr->GetHandle());
    if (Ret == SQL_NO_DATA_FOUND)
      return false;
    if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
      throw StatementException(GetLastDBError(SQL_HANDLE_STMT, StmtHldr->GetHandle()), seErrorFetch);
    return true;
  }



  Field::~Field()
  {
  }

  class FieldImpl
    : public Field
  {
  public:
    FieldImpl(Statement &stmt, unsigned index)
      : Stmt(stmt)
      , Index(index)
    {
    }

    virtual bool GetAsBool() const
    {
      return !!GetData<long>(SQL_C_LONG);
    }
    long GetAsLong() const
    {
      return GetData<long>(SQL_C_LONG);
    }
    long GetAsULong() const
    {
      return GetData<long>(SQL_C_ULONG);
    }
    double GetAsDouble() const
    {
      return GetData<double>(SQL_C_DOUBLE);
    }
    std::tm GetAsTimestamp() const
    {
      SQL_TIMESTAMP_STRUCT Timestamp = GetData<TIMESTAMP_STRUCT>(SQL_C_TIMESTAMP);
      std::tm Tm = { 0 };
      Tm.tm_year = Timestamp.year - 1900;
      Tm.tm_mon = Timestamp.month;
      Tm.tm_mday = Timestamp.day;
      Tm.tm_hour = Timestamp.hour;
      Tm.tm_min = Timestamp.minute;
      Tm.tm_sec = Timestamp.second;
      Tm.tm_isdst = -1;
      std::time_t Seconds = std::mktime(&Tm);
      if (Seconds == -1)
        throw FieldException("Bad date format", feBadFormat);
      return *std::localtime(&Seconds);
    }
    virtual TimeStruct GetAsTime() const
    {
      TIME_STRUCT Time = GetData<TIME_STRUCT>(SQL_C_TIME);
      TimeStruct Tm = { 0 };
      Tm.Hour = Time.hour;
      Tm.Min = Time.minute;
      Tm.Sec = Time.second;
      return Tm;
    }
    std::string GetAsString() const
    {
      SQLINTEGER Needed = 0;
      const unsigned BufSize = 256;
      char SmallBuff[BufSize] = { 0 };
      RETCODE Ret = SQLGetData(Stmt.GetHolder().GetHandle(), static_cast<SQLUSMALLINT>(Index), SQL_C_CHAR,
        SmallBuff, BufSize,
        //reinterpret_cast for x64
        reinterpret_cast<SQLLEN *>(&Needed));
      if (TIODBC_SUCCESS_CODE(Ret))
        return std::string(SmallBuff);
		  if (Needed > 0)
      {
        SQLINTEGER Buff = Needed + 1;
        std::vector<char> Data(Buff, 0);
        Ret = SQLGetData(Stmt.GetHolder().GetHandle(), static_cast<SQLUSMALLINT>(Index), SQL_C_CHAR,
          reinterpret_cast<SQLTCHAR *>(&Data.front()), Buff,
          //reinterpret_cast for x64
          reinterpret_cast<SQLLEN *>(&Needed));
        if (TIODBC_SUCCESS_CODE(Ret))
          return std::string(&Data[0], Data.size());
        throw FieldException("Error get field", feErrorGetField);
		  }
      return std::string();
    }

  private:
    Statement &Stmt;
    unsigned Index;
    template<typename T>
    T GetData(SQLSMALLINT type) const
    {
      T Storage = T();
      SQLINTEGER Needed = 0;
		  RETCODE Ret = SQLGetData(Stmt.GetHolder().GetHandle(), static_cast<SQLUSMALLINT>(Index),
        type, &Storage, sizeof(Storage),
        //reinterpret_cast for x64
        reinterpret_cast<SQLLEN *>(&Needed));
		  if (Ret != SQL_SUCCESS)
			  throw FieldException("Error get field", feErrorGetField);
      if (Needed <= 0)
        throw FieldException("Empty field", feErrorGetField);
      return Storage;
	  }
  };

  FieldPtr Statement::GetField(unsigned index) const
  {
    return FieldPtr(new FieldImpl(*const_cast<Statement *>(this), index + 1));
  }

  unsigned Statement::GetFieldsCount() const
  {
    unsigned FieldsCount = 0;
    if (SQLNumResultCols(StmtHldr->GetHandle(), reinterpret_cast<SQLSMALLINT *>(&FieldsCount)) != SQL_SUCCESS)
      StatementException("Error get fields count", seErrorGetFieldsCount);
    return FieldsCount;
  }

  Statement::StatementHolder& Statement::GetHolder() const
  {
    return *StmtHldr;
  }

  void Statement::CloseCursor()
  {
    if (IsOpen)
    {
      if (SQLCloseCursor(StmtHldr->GetHandle()) != SQL_SUCCESS)
        throw StatementException("Error close cursor", seErrorCloseCursor);
      IsOpen = false;
    }
  }


  Parameter::Parameter(Statement &stmt, unsigned index)
    : Stmt(stmt)
    , Index(index)
    , LongValue(0)
    , ULongValue(0)
    , DoubleValue(0.0)
  {
  }

  void Parameter::SetParam(long value)
  {
    LongValue = value;
    ::SetParam(Stmt.GetHolder().GetHandle(), Index, SQL_C_SLONG, SQL_INTEGER, &LongValue, SetParamFlag(&FlagBuf, 0));
  }

  void Parameter::SetParam(unsigned long value)
  {
    ULongValue = value;
    ::SetParam(Stmt.GetHolder().GetHandle(), Index, SQL_C_ULONG, SQL_INTEGER, &ULongValue, SetParamFlag(&FlagBuf, 0));
  }

  void Parameter::SetParam(double value)
  {
    DoubleValue = value;
    ::SetParam(Stmt.GetHolder().GetHandle(), Index, SQL_C_DOUBLE, SQL_DOUBLE, &DoubleValue, SetParamFlag(&FlagBuf, 0));
  }

  void Parameter::SetParam(const std::string &value)
  {
    StringValue = value;
    ::SetParam(Stmt.GetHolder().GetHandle(), Index, StringValue, SetParamFlag(&FlagBuf, SQL_NTS));
  }

  void Parameter::SetParam(const std::tm &value)
  {
    std::stringstream Io;
    Io
      << std::setw(4) << std::setfill('0') << value.tm_year + 1900 << "-"
      << std::setw(2) << std::setfill('0') << value.tm_mon + 1 << "-"
      << std::setw(2) << std::setfill('0') << value.tm_mday << " "
      << std::setw(2) << std::setfill('0') << value.tm_hour << ":"
      << std::setw(2) << std::setfill('0') << value.tm_min << ":"
      << std::setw(2) << std::setfill('0') << value.tm_sec;
    StringValue = Io.str();
    SetParam(StringValue);
  }

  void Parameter::SetParam(const char *value)
  {
    SetParam(std::string(value));
  }

  void Parameter::SetParam(bool value)
  {
    SetParam(std::string(value ? "TRUE" : "FALSE"));
  }

}
