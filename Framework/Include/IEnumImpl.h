#ifndef __IENUMIMPL_H__
#define __IENUMIMPL_H__

#include "IFacesTools.h"
#include "Exceptions.h"

#include <vector>
#include <iterator>

namespace IFacesImpl
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  class IEnumImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IEnum)
        >
  {
  public:
    DECLARE_UUID(6724ad4d-22eb-4d3d-bc0e-4283ef45373e)


    IEnumImpl();
    virtual ~IEnumImpl();

    // IEnum
    virtual RetCode First();
    virtual RetCode Next(IFaces::IBase **item);
    virtual RetCode Clone(IFaces::IEnum **newEnum) const;

    typedef Common::RefObjPtr<IFaces::IBase> IBasePtr;

    void AddItem(IBasePtr item);
    void Clear();
  private:
    typedef std::vector<IBasePtr> IBasePtrPool;
    bool IsModified;
    mutable IBasePtrPool Items;
    typedef IBasePtrPool::iterator Iterator;
    Iterator CurIter;
  };

  template <typename TSyn>
  Common::RefObjPtr<IEnumImpl>
  CreateEnum()
  {
    return Common::IBaseImpl<IEnumImpl>::Create<TSyn>();
  }

  Common::RefObjPtr<IEnumImpl>
  CreateEnum(const Common::ISynObj &syn);

  DECLARE_RUNTIME_EXCEPTION(IEnumHelper)

  class IEnumHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IEnum> IEnumPtr;
    typedef Common::RefObjPtr<IFaces::IBase> IBasePtr;

    IEnumHelper(IEnumPtr enumerator);
    IEnumHelper(const IEnumHelper &enumHelper);
    IEnumHelper& operator = (const IEnumHelper &enumHelper);
    IBasePtr First();
    IBasePtr Next();
  private:
    IEnumPtr Enumerator;
  };

}

#endif  // !__IENUMIMPL_H__
