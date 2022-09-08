#include "ManagerTanks.h"
#include "Tank.h"
#include "TypeTank.h"

TManagerTanks GlobalManagerTanks;

using namespace nsTank_ID;
using namespace nsStrTank;

TManagerTanks::TManagerTanks()
{
}
//--------------------------------------------------------
void TManagerTanks::LoadPropertyFromIni(TTank* pTank)
{
  char path[400];
  strcpy(path,"..\\tanks\\");
  switch(pTank->GetID())
  {
    case eID_T_34:
      strcpy(path,strTank_T_34_Ini);
      break;
    case eID_IS:
      strcpy(path,strTank_IS_Ini);
      break;
    default:return;
  }

  Load(path,pTank);
}
//--------------------------------------------------------
void TManagerTanks::Load(char* path,TTank* pTank)
{
  // перечисление параметров
  
}
//--------------------------------------------------------
