#include "FlashView.h"


IFlashViewImpl::IFlashViewImpl()
{
}

bool IFlashViewImpl::FinalizeCreate()
{
  return true;
}

void IFlashViewImpl::BeforeDestroy()
{
}

bool IFlashViewImpl::OnInit()
{
  Common::SharedPtr<Common::ISynObj> NewDataSourceSynObj(GetSynObj().Clone());
  try
  {
    FlashCtrl = new FlashCtrlHolder;
  }
  catch (std::exception &)
  {
    return false;
  }
  DataSourceSynObj = NewDataSourceSynObj;
  return true;
}

void IFlashViewImpl::OnDone()
{
  {
    Common::ISyncObject Locker(*DataSourceSynObj.Get());
    DataSource.Release();
  }

  FlashCtrl->Done();
  Common::ISyncObject Locker(GetSynObj());
  FlashCtrl.Release();
}

RetCode IFlashViewImpl::SetDataSource(IFaces::IStorage *dataSource)
{
  {
    if (!DataSourceSynObj.Get())
      return retFail;
    Common::ISyncObject Locker(*DataSourceSynObj.Get());
    DataSource = dataSource;
  }
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    FlashCtrl->SetDataSource(DataSource);
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

RetCode IFlashViewImpl::PlayMovie(const char *movieName)
{
  FlashCtrlHolderPtr Ctrl;
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!FlashCtrl.Get())
      return retFail;
    Ctrl = FlashCtrl;
  }
  try
  {
    Ctrl->PlayMovie(movieName);
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

long IFlashViewImpl::OnMessage(const IFaces::WindowMessage &msg)
{
  FlashCtrlHolderPtr Ctrl;
  {
    Common::ISyncObject Locker(GetSynObj());
    Ctrl = FlashCtrl;
  }
  return Ctrl.Get() ? Ctrl->OnMessage(msg) : 0;
}
