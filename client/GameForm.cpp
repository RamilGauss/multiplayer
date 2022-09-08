#include "GameForm.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "ClientTank.h"
#include "GlobalParamsTank.h"
#include <QMessageBox>
#include <QTimer>
#include "ApplicationProtocolMainPacket.h"
#include "ApplicationProtocolPacketStream.h"
#include "LayerBetween_QT_DirectX.h"
#include "Bufferizator2Thread.h"

//-------------------------------------------------------------------------------------------
GameForm::GameForm(QWidget *parent)
: TBaseGUI(parent)
{
  pCallBackExitDirectX = NULL;
}
//---------------------------------------------------------------------------------------------
GameForm::~GameForm()
{
}
//---------------------------------------------------------------------------------------------
void GameForm::closeEvent(QCloseEvent* )
{
  sl_Exit();
}
//---------------------------------------------------------------------------------------------
void GameForm::sl_Exit()
{
  _exit(0);
}
//---------------------------------------------------------------------------------------------
void GameForm::Translate(unsigned short type, char*pData, int size)
{
  switch(type)
  {
//    case APPL_TYPE_A_IN_FIGHT:
//    {
//      break;
//    }
//    case APPL_TYPE_A_END_FIGHT:
//    {
      //char* dataPacket = (char*)data + sizeof(TIP_Port);
      //int sizePacket   = size - sizeof(TIP_Port);
      //pGameForm->AddListTank(dataPacket,sizePacket);
      //QEvent * event = new QEvent(QEVENT_A_LIST_TANKS);
      //QCoreApplication::postEvent( pGameForm, event );
//      break;
//    }
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void GameForm::showGUI()
{
  startDX();
  //GlobalBufferizator2Thread.RegisterToClientTank();//### только когда загрузится карта и перед тем как отошлется запрос на корректирующий пакет.
}
//---------------------------------------------------------------------------------------------
void GameForm::hideGUI()
{
  stopDX();
  //GlobalBufferizator2Thread.UnregisterFromClientTank();
}
//---------------------------------------------------------------------------------------------
void GameForm::startDX()
{
  g_LB_QT_DX.StartDirectX(pCallBackExitDirectX);
}
//---------------------------------------------------------------------------------------------
void GameForm::stopDX()
{
  g_LB_QT_DX.StopDirectX();
}
//---------------------------------------------------------------------------------------------

