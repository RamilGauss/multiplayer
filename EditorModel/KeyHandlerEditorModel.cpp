#include "KeyHandlerEditorModel.h"
#include "LoggerDX.h"
#include "ClientTank.h"
#include "ObjectDX.h"
#include "ManagerDirectx.h"


//-----------------------------------------------------------------------------------------------
TKeyHandlerEditorModel::TKeyHandlerEditorModel()
{

}
//-----------------------------------------------------------------------------------------------
TKeyHandlerEditorModel::~TKeyHandlerEditorModel()
{

}
//-----------------------------------------------------------------------------------------------
unsigned int TKeyHandlerEditorModel::Work(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
  switch(nChar)
  {
    case 0x1B://VK_ESCAPE:
    {
      if(bKeyDown==true)
      {
        TObjectDX* pObj = mManagerDirectX->LoadObjectDX(1);
        if(pObj)
        {
          pObj->SetState(1);
        }
      }
      break;
    }
    default:;
  }

  return 0;
}
//-----------------------------------------------------------------------------------------------
