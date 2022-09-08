#include "Tank.h"
#include "ManagerTanks.h"

using namespace nsServerStruct;

TTank::TTank(TClient* pClient)
{
  pMasterClient = pClient;
  pRoom = NULL;
}
//------------------------------------------------------------------------
void TTank::SetID(int mid)
{
  mID=mid;
}
//------------------------------------------------------------------------
int TTank::GetID()
{
  return mID;
}
//------------------------------------------------------------------------
TClient* TTank::GetMasterClient()
{
  return pMasterClient;
}
//------------------------------------------------------------------------
void TTank::LoadPropertyFromIni()
{
  GlobalManagerTanks.LoadPropertyFromIni(this);
}
//------------------------------------------------------------------------

