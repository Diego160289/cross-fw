//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __IVARIANTIMPL_H__
#define __IVARIANTIMPL_H__

#include "IFaces.h"
#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"
#include "RefObjPtr.h"
#include "Pointers.h"
#include "Typedefs.h"
#include "CommonUtils.h"

#include <string.h>

#include <vector>


namespace IFacesImpl
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  class IVariantImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IVariant)
        >
  {
  public:
    DECLARE_UUID(0b9550da-6466-4fd4-9a43-40901551f727)

    IVariantImpl();
    virtual ~IVariantImpl();

    // IVariant
    virtual RetCode SetValue(IFaces::IVariant::VariantType type, const void *value);
    virtual RetCode SetBinaryValue(const void *value, unsigned long bytes);
    virtual RetCode GetValue(void **value) const;
    virtual unsigned long GetValueSize() const;
    virtual bool IsEmpty() const;
    virtual IFaces::IVariant::VariantType GetType() const;
    virtual RetCode Clear();
    virtual const char* PackToBase64() const;
    virtual RetCode FromBase64Pack(const char *pkg);

    struct IVariantHolder;
  private:
    typedef Common::SharedPtr<IVariantHolder> IVariantHolderPtr;

    IVariantHolderPtr Holder;
  };

  template <typename TSyn>
  typename Common::RefObjPtr<IVariantImpl>
  CreateVariant()
  {
    return Common::IBaseImpl<IVariantImpl>::Create<TSyn>();
  }

  Common::RefObjPtr<IVariantImpl>
  CreateVariant(const Common::ISynObj &syn);

  DECLARE_RUNTIME_EXCEPTION(IVariantHelper)

  class IVariantHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IVariant> IVariantPtr;

    IVariantHelper(IVariantPtr var);
    ~IVariantHelper();

    IVariantHelper& operator = (bool value);
    IVariantHelper& SetBinaryData(const void *data, unsigned long bytes);
    IVariantHelper& operator = (IFaces::IBase *ptr);
    IVariantHelper& operator = (char value);
    IVariantHelper& operator = (unsigned char value);
    IVariantHelper& operator = (short value);
    IVariantHelper& operator = (unsigned short value);
    IVariantHelper& operator = (int value);
    IVariantHelper& operator = (unsigned int value);
    IVariantHelper& operator = (long value);
    IVariantHelper& operator = (unsigned long value);
    IVariantHelper& operator = (float value);
    IVariantHelper& operator = (double value);
    IVariantHelper& operator = (const char *value);
    IVariantHelper& operator = (const wchar_t *value);

    operator const bool () const;
    operator const void* () const;
    operator Common::RefObjPtr<IFaces::IBase> () const;
    operator const char () const;
    operator const unsigned char () const;
    operator const short () const;
    operator const unsigned short () const;
    operator const int () const;
    operator const unsigned int () const;
    operator const long () const;
    operator const unsigned long () const;
    operator const float () const;
    operator const double () const;
    operator const char* () const;
    operator const wchar_t* () const;

    IFaces::IVariant::VariantType GetType() const;
    bool IsEmpty() const;
    unsigned long GetDataSize() const;

    IVariantPtr GetVariant() const;
  private:
    mutable IVariantPtr Var;
  };
}

#endif  // !__IVARIANTIMPL_H__
