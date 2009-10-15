#ifndef __IENVIRONMENT_H__
#define __IENVIRONMENT_H__

#include "IFacesTools.h"
#include "Exceptions.h"
#include "RefObjQIPtr.h"

#include <map>
#include <string>


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retNoInterface;

  DECLARE_RUNTIME_EXCEPTION(IEnvironmentImpl)

  class IEnvironmentImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IEnvironment)
        >
  {
  public:
    DECLARE_UUID(52f0f964-97cb-44f7-af0c-0908c68de0d7)

    virtual ~IEnvironmentImpl();

    // IEnvironment
    virtual RetCode EnumEnvironmentIFacesId(IFaces::IEnum **ifaceIds) const;
    virtual RetCode GetInterface(const char *ifaceId, IFaces::IBase **iface) const;
    virtual RetCode SetInterface(const char *ifaceId, IFaces::IBase *iface);


    void AddIFaceId(const std::string &ifaceId);

    template <typename T>
    Common::RefObjPtr<T> GetInterface() const
    {
      Common::RefObjPtr<IBase> Iface;
      if (GetInterface(T::GetUUID(),  Iface.GetPPtr()) != retOk)
        throw IEnvironmentImplException("No interface");
      Common::RefObjQIPtr<T> Ret(Iface);
      if (!Ret.Get())
        throw IEnvironmentImplException("No interface");
      return Ret;
    }
  private:
    typedef Common::RefObjPtr<IFaces::IBase> IBasePtr;
    typedef std::map<std::string, IBasePtr> IFacesPool;
    mutable IFacesPool Interfaces;
  };

  template <typename TNecessaryIFacesList>
  class IEnvironmentImplEx
    : public IEnvironmentImpl
  {
  public:
    IEnvironmentImplEx()
    {
      const Common::StringVector IDs = Common::IFacesIdFromList<TNecessaryIFacesList>::Get();
      for (Common::StringVector::const_iterator i = IDs.begin() ; i != IDs.end() ; ++i)
        AddIFaceId(*i);
    }
  };

}

#endif // !__IENVIRONMENT_H__
