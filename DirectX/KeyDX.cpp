#include "KeyDX.h"
#include "ClientTank.h"


TKeyDX::TKeyDX()
{

}
//---------------------------------------------------------------------------------------------
TKeyDX::~TKeyDX()
{

}
//---------------------------------------------------------------------------------------------
void TKeyDX::Work(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
  switch(nChar)
  {
    case 0x1B://VK_ESCAPE:
      GlobalClientTank.SendRequestExitFromFight();
      break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------
