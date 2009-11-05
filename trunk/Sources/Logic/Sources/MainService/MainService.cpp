#include "MainService.h"

#include "../../../Framework/Include/CommonUtils.h"

IMainServiceImpl::IMainServiceImpl()
{
}

bool IMainServiceImpl::FinalizeCreate()
{
  return true;
}

void IMainServiceImpl::BeforeDestroy()
{
}

#include <conio.h>
#include <iostream>

bool IMainServiceImpl::OnInit()
{
  try
  {
    DataSrc = StartService("e1156d71-fd1a-4a4a-82ed-a1ab73dc9c87");
    View = StartService("ac0dd189-8db2-47fb-a590-438fd151910a");
    View->SetCallback(this);
    View->ShowView();
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IMainServiceImpl::OnDone()
{
  DataSrc.Release();
}

RetCode IMainServiceImpl::OnQueryExternalResource(const char *resName, IFaces::IStream **stream)
{
  Common::ISyncObject Locker(GetSynObj());
  return !DataSrc.Get() ? retFail : DataSrc->GetResource(resName, stream);
}

RetCode IMainServiceImpl::GetBusinessCategories(const wchar_t *objectId,
                                                const wchar_t *frameId)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  IFaces::BusinessCategoriesItem Categories[] = {
    {2222222, L"Телефоны", L"description", L"pics/fx_appicon-tn.gif"},
    {11111111, L"ЖКХ", L"description", L"pics/dw_appicon-tn.gif"}
  };
  return Funcs->OnGetBusinessCategories(&Categories[0], Common::SizeOfArray(Categories));
}

RetCode IMainServiceImpl::BusinessCategorySelected(const wchar_t *objectId,
                                                   const wchar_t *frameId,
                                                   const wchar_t *businessCategoryId)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  return Funcs->ChangeFrame(L"PROVIDERS_SCREEN");
}

RetCode IMainServiceImpl::GetProviders(const wchar_t *objectId, const wchar_t *frameId)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  IFaces::BusinessCategoriesItem Providers[] = {
    {10, L"Провайдер 1", L"Описание Провайдер 1", L"pics/lr_appicon-tn.gif"},
    {20, L"Провайдер 2", L"Описание Провайдер 2", L"pics/fl_player_appicon-tn.gif"}
  };
  return Funcs->OnGetProviders(&Providers[0], Common::SizeOfArray(Providers));
}

RetCode IMainServiceImpl::ProviderSelected(const wchar_t *objectId,
                                           const wchar_t *frameId,
                                           const wchar_t *providerId)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  return Funcs->ChangeFrame(L"ENTER_CELL_PHONE_NUMBER_SCREEN");
}

RetCode IMainServiceImpl::CellPhoneNumberEntered(const wchar_t *objectId,
                                                 const wchar_t *frameId,
                                                 const wchar_t *cellPhoneNumber)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  return Funcs->ChangeFrame(L"VERIFY_CELL_PHONE_NUMBER_SCREEN");
}

RetCode IMainServiceImpl::CellPhoneNumberVerified(const wchar_t *objectId,
                                                  const wchar_t *frameId)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  return Funcs->ChangeFrame(L"PAYMENT_SCREEN");
}

RetCode IMainServiceImpl::ProcessPayment(const wchar_t *objectId,
                                         const wchar_t *frameId)
{
  Common::ISyncObject Locker(GetSynObj());
  Common::RefObjQIPtr<IFaces::IMainViewFuncs> Funcs(View);
  if (!Funcs.Get())
    return retFail;
  return Funcs->ChangeFrame(L"THANK_YOU_SCREEN");
}
