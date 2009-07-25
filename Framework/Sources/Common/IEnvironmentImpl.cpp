#include "IEnvironmentImpl.h"
#include "IEnumImpl.h"
#include "IVariantImpl.h"
#include "RefObjQIPtr.h"


namespace IFacesImpl
{

  IEnvironmentImpl::~IEnvironmentImpl()
  {
  }

  RetCode IEnvironmentImpl::EnumEnvironmentIFacesId(IFaces::IEnum **ifaceIds) const
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      Common::RefObjPtr<Common::IBaseImpl<IEnumImpl> > Enum =
        Common::IBaseImpl<IEnumImpl>::CreateWithSyn(GetSynObj());
      for (IFacesPool::iterator i = Interfaces.begin() ; i != Interfaces.end() ; ++i)
      {
        IVariantHelper Var(Common::RefObjQIPtr<IFaces::IVariant>(Common::IBaseImpl<IVariantImpl>::CreateWithSyn(GetSynObj())));
        Var = static_cast<const char*>(i->first.c_str());
        Enum->AddItem(Common::RefObjQIPtr<IFaces::IBase>(Var.GetVariant().Get()));
      }
      return Enum.QueryInterface(ifaceIds);
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retFail;
  }

  RetCode IEnvironmentImpl::GetInterface(const char *ifaceId, IFaces::IBase **iface) const
  {
    Common::ISyncObject Locker(GetSynObj());
    IFacesPool::iterator Iter = Interfaces.find(ifaceId);
    if (Iter == Interfaces.end())
      return retNoInterface;
    return Iter->second.QueryInterface(iface);
  }

  RetCode IEnvironmentImpl::SetInterface(const char *ifaceId, IFaces::IBase *iface)
  {
    Common::ISyncObject Locker(GetSynObj());
    IFacesPool::iterator Iter = Interfaces.find(ifaceId);
    if (Iter != Interfaces.end() && Iter->second.Get())
      return retFail;
    Interfaces[ifaceId] = iface;
    return retOk;
  }

  void IEnvironmentImpl::AddIFaceId(const std::string &ifaceId)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Interfaces.find(ifaceId) != Interfaces.end())
      throw IEnvironmentImplException("IFace id already exists");
    Interfaces[ifaceId];
  }

}
