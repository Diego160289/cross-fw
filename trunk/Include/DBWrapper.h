//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __DBWRAPPERS_H__
#define __DBWRAPPERS_H__


#include "DBClient.h"
#include "RefObjPtr.h"
#include "Exceptions.h"

#include <string>


namespace Common
{
  namespace Wrappers
  {
    namespace DB
    {

      typedef Common::RefObjPtr<IFaces::DBIFaces::IConnection> IConnectionPtr;
      typedef Common::RefObjPtr<IFaces::DBIFaces::ITransaction> ITransactionPtr;
      typedef Common::RefObjPtr<IFaces::DBIFaces::IStatement> IStatementPtr;
      typedef Common::RefObjPtr<IFaces::DBIFaces::IParameter> IParameterPtr;
      typedef Common::RefObjPtr<IFaces::DBIFaces::IField> IFieldPtr;


      DECLARE_RUNTIME_EXCEPTION(Connection)

      class Connection
      {
      public:
        Connection(IConnectionPtr conn);
        void Connect(const std::string &connectionString, const std::string &userName, const std::string &password);
        void Disconnect();
        ITransactionPtr CreateTransaction();
        IStatementPtr CreateStatement();

      private:
        IConnectionPtr Conn;
      };

      DECLARE_RUNTIME_EXCEPTION(Transaction)

      class Transaction
      {
      public:
        Transaction(ITransactionPtr tr);
        void Commit();
        void Rollback();

      private:
        ITransactionPtr Tr;
      };

      DECLARE_RUNTIME_EXCEPTION(Statement)

      class Statement
      {
      public:
        Statement(IStatementPtr stmt);
        void Prepare(const std::string &str);
        void ExecuteDirect(const std::string &str);
        void Execute();
        bool Fetch();
        IParameterPtr GetParameter(unsigned index);
        IFieldPtr GetField(unsigned index) const;
        unsigned GetFieldsCount() const;

      private:
        IStatementPtr Stmt;
      };

      DECLARE_RUNTIME_EXCEPTION(Parameter)

      class Parameter
      {
      public:
        Parameter(IParameterPtr prm);
        void SetParam(long value);
        void SetParam(unsigned long value);
        void SetParam(double value);
        void SetParam(const std::string &value);
        void SetParam(const std::wstring &value);
        void SetParam(const IFaces::DBIFaces::DateTime &value);
        void SetParam(const IFaces::DBIFaces::Date &value);
        void SetParam(const IFaces::DBIFaces::Time &value);
        void SetParam(bool value);

      private:
        IParameterPtr Prm;
      };

      DECLARE_RUNTIME_EXCEPTION(Field)

      class Field
      {
      public:
        Field(IFieldPtr fld);
        operator long () const;
        operator unsigned long () const;
        operator double () const;
        operator std::string () const;
        operator std::wstring () const;
        operator IFaces::DBIFaces::DateTime () const;
        operator IFaces::DBIFaces::Date () const;
        operator IFaces::DBIFaces::Time () const;
        operator bool () const;

      private:
        IFieldPtr Fld;
      };

    }
  }
}

#endif  // !__DBWRAPPERS_H__
