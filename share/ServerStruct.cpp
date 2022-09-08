#include "ServerStruct.h"
#include "TypeTank.h"
#include "Tank.h"

using namespace nsServerStruct;


void TGarage::InitArrTank()
{
  // читаем из БД
  mArrTanks.Clear();
  
  int cnt = 2;// ReadFromBD(...);
  mCurTank = 0; // первый в массиве

  TTank* pTank = new TTank(pMasterClient);
  pTank->SetID(nsTank_ID::eID_T_34);
  mArrTanks.Add(pTank);

  pTank = new TTank(pMasterClient);
  pTank->SetID(nsTank_ID::eID_IS);
  mArrTanks.Add(pTank);

}
//------------------------------------------------------------------------------
bool TGarage::SetCurTank(int i)
{
  if(i>=mArrTanks.Count())
    return false;
  mCurTank = i;
  return true;
}
//------------------------------------------------------------------------------
int TGarage::GetCurTank()
{
  return mCurTank;
}
//------------------------------------------------------------------------------
TTank* TGarage::GetPointerCurTank()
{
  return (TTank*)mArrTanks.Get(mCurTank);
}
//------------------------------------------------------------------------------
TClient::TClient():mGarage(this)
{
  sNick=NULL;
  time=ht_GetMSCount();
}
//------------------------------------------------------------------------------
TClient::~TClient()
{
  free(sNick);
}
//------------------------------------------------------------------------------