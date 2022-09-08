#include "GameRoomPrepare.h"
#include "ApplicationProtocolDef.h"
#include "GlobalParamsTank.h"
#include "BL_Debug.h"
#include "TransportProtocolPacket.h"
#include "ApplicationProtocolPacketStream.h"
#include "ClientTank.h"
#include <QMessageBox>
#include <QTimer>
#include "ClientMain.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "TypeTank.h"
#include "WaitForm.h"
#include "GameForm.h"



//-------------------------------------------------------------------------------------------
//void CallBackPacketGameRoomPrepare(void* data, int size)
//{
//	gcsGameRoomPrepare.lock();
//	//--------------------------------------------------------
//	if(pGameRoomPrepare==NULL) {BL_FIX_BUG();return;}
//	unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
//	switch(type)
//	{
//		case APPL_TYPE_A_GET_LIST_TANK:
//		{
//			char* dataPacket = (char*)data + sizeof(TIP_Port);
//			int sizePacket   = size - sizeof(TIP_Port);
//			pGameRoomPrepare->AddListTank(dataPacket,sizePacket);
//			QEvent * event = new QEvent(QEVENT_A_LIST_TANKS);
//			QCoreApplication::postEvent( pGameRoomPrepare, event );
//			break;
//		}
//    case APPL_TYPE_A_IN_FIGHT:
//    {
//      char* dataPacket = (char*)data + sizeof(TIP_Port);
//      int sizePacket   = size - sizeof(TIP_Port);
//      pGameRoomPrepare->SetCodeExit_A_In_Fight(dataPacket,sizePacket);
//      QEvent * event = new QEvent(QEVENT_A_IN_FIGHT);
//      QCoreApplication::postEvent( pGameRoomPrepare, event );
//      break;
//    }
//		default:;
//	}
//	//--------------------------------------------------------
//	gcsGameRoomPrepare.unlock();
//}
////-------------------------------------------------------------------------------------------
//void CallBackStreamGameRoomPrepare(void* data, int size)
//{
//	gcsGameRoomPrepare.lock();
//	//--------------------------------------------------------
//	if(pGameRoomPrepare==NULL) {BL_FIX_BUG();return;}
//	unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
//	switch(type)
//	{
//		case APPL_TYPE_S_GARAGE:
//		{
//			char* dataPacket = (char*)data + sizeof(TIP_Port);
//			int sizePacket   = size - sizeof(TIP_Port);
//			TS_Garage S_Garage;
//			S_Garage.setData(dataPacket,sizePacket);
//			pGameRoomPrepare->SetCntClient(S_Garage.getCommon(),S_Garage.getFight());
//			break;
//		}
//		default:;
//	}
//	//--------------------------------------------------------
//}
//-------------------------------------------------------------------------------------------
GameRoomPrepare::GameRoomPrepare(QWidget *parent)
: TBaseGUI(parent)
{
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(sl_RefreshCntClient()));

  mCntCommon = 0;
	mCntInFight = 0;

  ui.setupUi(this);
  setWindowTitle(tr("Комната ")+tr(STR_VERSION_CLIENT));

  connect(ui.cbTank,SIGNAL(activated(int)), this, SLOT(sl_ChangeCurTank()));
  connect(ui.bFight,SIGNAL(clicked()), this, SLOT(sl_InFight()));
	connect(ui.bExit,  SIGNAL(clicked()), this, SLOT(sl_Exit()));
}
//---------------------------------------------------------------------------------------------
GameRoomPrepare::~GameRoomPrepare()
{
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::SetCntClient(unsigned int common,unsigned int fight)
{
	mCntCommon = common;
	mCntInFight = fight;
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::sl_RefreshCntClient()
{
	QString str;
	str = QString(tr("В бою: %1")).arg(mCntInFight);
	ui.lInFight->setText(str);
	
	str = QString(tr("Всего: %1")).arg(mCntCommon);
	ui.lAll->setText(str);
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::closeEvent(QCloseEvent* )
{
	sl_Exit();
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::sl_Exit()
{
	_exit(0);
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::SendRequestListTank()
{
  GlobalClientTank.SendRequestListTank();
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::ViewListTanks()
{
  for(int i = 0 ; i < mPacketListTank.getCnt() ; i++)
  {
    unsigned short typeTank = mPacketListTank.getTypeTank(i);
    switch(typeTank)
    {
      case nsTank_ID::eID_T_34:
				if(mPacketListTank.getFlgBlockTank(i))
          ui.cbTank->addItem(tr("Т-34 заблокирован"));
				else
	        ui.cbTank->addItem(tr("Т-34"));
        break;
      case nsTank_ID::eID_IS:
				if(mPacketListTank.getFlgBlockTank(i))
					ui.cbTank->addItem(tr("ИС заблокирован"));
				else
					ui.cbTank->addItem(tr("ИС"));
        break;
      default:BL_FIX_BUG();
    }
  }
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::AddListTank(char* data, int size)
{
  mPacketListTank.setData(data,size);
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::sl_ChangeCurTank()
{
  int i = ui.cbTank->currentIndex();
  if(i==-1) return;
  GlobalClientTank.SetCurrentTank(i);
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::sl_InFight()
{
  GlobalClientTank.SendRequestInFight();
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::Translate(unsigned short type, char*pData, int size)
{
  switch(type)
  {
		case APPL_TYPE_S_GARAGE:
		{
			TS_Garage S_Garage;
			S_Garage.setData(pData,size);
			SetCntClient(S_Garage.getCommon(),S_Garage.getFight());
			break;
		}
		case APPL_TYPE_A_GET_LIST_TANK:
		{
			AddListTank(pData, size);
      ViewListTanks();
			break;
		}
  	default:;
  }
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::showGUI()
{
  ui.cbTank->clear();
  sl_RefreshCntClient();

  // запуск таймера для обновления списка стрима
  mTimer->start(eTimeRefreshCntClient);

  SendRequestListTank();

  QWidget::show();
}
//---------------------------------------------------------------------------------------------
void GameRoomPrepare::hideGUI()
{
  mTimer->stop();
  QWidget::hide();
}
//---------------------------------------------------------------------------------------------