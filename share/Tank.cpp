#include "Tank.h"
#include "ManagerTanks.h"

using namespace nsServerStruct;

TTank::TTank(TClient* pClient):
TObjectPrediction(eTank)
{
  pMasterClient = pClient;
  pRoom = NULL;
}
//------------------------------------------------------------------------
TTank::~TTank()
{

}
//------------------------------------------------------------------------
void TTank::SetTypeTank(unsigned short typeTank)
{
  mTypeTank = typeTank;
}
//------------------------------------------------------------------------
unsigned short TTank::GetTypeTank()
{
  return mTypeTank;
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

