//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DBWrapper.h"
#include "IVariantImpl.h"


namespace Common
{
  namespace Wrappers
  {
    namespace DB
    {

      Connection::Connection(IConnectionPtr conn)
        : Conn(conn)
      {
        if (!Conn.Get())
          throw ConnectionException("Empty connection pointer");
      }

      void Connection::Connect(const std::string &connectionString,
                               const std::string &userName,
                               const std::string &password)
      {
        if (Conn->Connect(connectionString.c_str(), userName.c_str(), password.c_str()) != IFaces::retOk)
          throw ConnectionException("Error connect");
      }

      void Connection::Disconnect()
      {
        if (Conn->Disconnect() != IFaces::retOk)
          throw ConnectionException("Error disconnect");
      }

      ITransactionPtr Connection::CreateTransaction()
      {
        ITransactionPtr Tr;
        if (Conn->CreateTransaction(Tr.GetPPtr()) != IFaces::retOk)
          throw ConnectionException("Error create transaction");
        return Tr;
      }

      IStatementPtr Connection::CreateStatement()
      {
        IStatementPtr Stmt;
        if (Conn->CreateStatement(Stmt.GetPPtr()) != IFaces::retOk)
          throw ConnectionException("Error create statement");
        return Stmt;
      }

      Transaction::Transaction(ITransactionPtr tr)
        : Tr(tr)
      {
        if (!Tr.Get())
          throw ConnectionException("Empty transaction pointer");
      }

      void Transaction::Commit()
      {
        if (Tr->Commit() != IFaces::retOk)
          throw ConnectionException("Error commit");
      }

      void Transaction::Rollback()
      {
        if (Tr->Rollback() != IFaces::retOk)
          throw ConnectionException("Error rollback");
      }

      Statement::Statement(IStatementPtr stmt)
        : Stmt(stmt)
      {
        if (!Stmt.Get())
          throw StatementException("Empty statement pointer");
      }

      void Statement::Prepare(const std::string &str)
      {
        if (Stmt->Prepare(str.c_str()) != IFaces::retOk)
          throw StatementException("Error prepare");
      }

      void Statement::ExecuteDirect(const std::string &str)
      {
        if (Stmt->ExecuteDirect(str.c_str()) != IFaces::retOk)
          throw StatementException("Error execute");
      }

      void Statement::Execute()
      {
        if (Stmt->Execute() != IFaces::retOk)
          throw StatementException("Error execute");
      }

      bool Statement::Fetch()
      {
        IFaces::RetCode Code = Stmt->Execute();
        if (Code != IFaces::retOk && Code != IFaces::retFalse)
          throw StatementException("Error fetch");
        return Code == IFaces::retOk;
      }

      IParameterPtr Statement::GetParameter(unsigned index)
      {
        IParameterPtr Prm;
        if (Stmt->GetParameter(index, Prm.GetPPtr()) != IFaces::retOk)
          throw StatementException("Error get parameter");
        return Prm;
      }

      IFieldPtr Statement::GetField(unsigned index) const
      {
        IFieldPtr Fld;
        if (Stmt->GetField(index, Fld.GetPPtr()) != IFaces::retOk)
          throw StatementException("Error get field");
        return Fld;
      }

      unsigned Statement::GetFieldsCount() const
      {
        unsigned Count = 0;
        if (Stmt->GetFieldsCount(&Count) != IFaces::retOk)
          throw StatementException("Error get fields count");
        return Count;
      }

      Parameter::Parameter(IParameterPtr prm)
        : Prm(prm)
      {
        if (!Prm.Get())
          throw ParameterException("Empty parameter pointer");
      }

      void Parameter::SetParam(long value)
      {
        Prm->SetParam(value);
      }

      void Parameter::SetParam(unsigned long value)
      {
        Prm->SetParam(value);
      }

      void Parameter::SetParam(double value)
      {
        Prm->SetParam(value);
      }

      void Parameter::SetParam(const std::string &value)
      {
        Prm->SetParam(value.c_str());
      }

      void Parameter::SetParam(const std::wstring &value)
      {
        Prm->SetParam(value.c_str());
      }

      void Parameter::SetParam(const IFaces::DBIFaces::DateTime &value)
      {
        Prm->SetParam(value);
      }

      void Parameter::SetParam(const IFaces::DBIFaces::Date &value)
      {
        Prm->SetParam(value);
      }

      void Parameter::SetParam(const IFaces::DBIFaces::Time &value)
      {
        Prm->SetParam(value);
      }

      void Parameter::SetParam(bool value)
      {
        Prm->SetParam(value);
      }

      Field::Field(IFieldPtr fld)
        : Fld(fld)
      {
        if (!Fld.Get())
          throw FieldException("Empty field pointer");
      }

      Field::operator long () const
      {
        long Value = 0;
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

      Field::operator unsigned long () const
      {
        unsigned long Value = 0;
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

      Field::operator double () const
      {
        double Value = 0.0;
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

      Field::operator std::string () const
      {
        Common::RefObjPtr<IFaces::IVariant> Value;
        if (!Fld->GetStrField(Value.GetPPtr(), false) != IFaces::retOk)
          throw FieldException("Error get field");
        return (const std::string::value_type *)IFacesImpl::IVariantHelper(Value);
      }

      Field::operator std::wstring () const
      {
        Common::RefObjPtr<IFaces::IVariant> Value;
        if (!Fld->GetStrField(Value.GetPPtr(), true) != IFaces::retOk)
          throw FieldException("Error get field");
        return (const std::wstring::value_type *)IFacesImpl::IVariantHelper(Value);
      }

      Field::operator IFaces::DBIFaces::DateTime () const
      {
        IFaces::DBIFaces::DateTime Value = { { 0 }, { 0 } };
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

      Field::operator IFaces::DBIFaces::Date () const
      {
        IFaces::DBIFaces::Date Value = { 0 };
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

      Field::operator IFaces::DBIFaces::Time () const
      {
        IFaces::DBIFaces::Time Value = { 0 };
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

      Field::operator bool () const
        
      {
        bool Value = false;
        if (!Fld->GetField(&Value) != IFaces::retOk)
          throw FieldException("Error get field");
        return Value;
      }

    }
  }
}
