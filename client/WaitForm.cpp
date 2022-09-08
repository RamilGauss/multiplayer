#include "ClientMain.h"
#include "NetSystem.h"
#include "ClientTank.h"
#include "ApplicationProtocolDef.h"
#include "WaitForm.h"
#include "TransportProtocolTank.h"
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include "ApplicationProtocolPacketAnswer.h"
#include "GameRoomPrepare.h"
#include "ApplicationProtocolPacketStream.h"
#include "GameForm.h"


//-------------------------------------------------------------------------------------------
//void CallBackPacketWaitForm(void* data, int size)
//{
//	gcsWaitForm.lock();
//		//------------------------------------------------
//  if(pWaitForm==NULL) {BL_FIX_BUG();return;}
//  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
//  switch(type)
//  {
//    case APPL_TYPE_A_EXIT_WAIT:
//    {
//      if(pWaitForm==NULL) {BL_FIX_BUG();return;}
//      QEvent * event = new QEvent(QEVENT_A_EXIT_WAIT);
//      QCoreApplication::postEvent( pWaitForm, event );
//      break;
//    }
//    case APPL_TYPE_A_IN_FIGHT:
//    {
//      char* dataPacket = (char*)data + sizeof(TIP_Port);
//      int sizePacket   = size - sizeof(TIP_Port);
//      pWaitForm->SetCodeExit_A_In_Fight(dataPacket,sizePacket);
//      QEvent * event = new QEvent(QEVENT_A_IN_FIGHT);
//      QCoreApplication::postEvent( pWaitForm, event );
//      break;
//    }
//    default:;
//  }
//	//------------------------------------------------
//	gcsWaitForm.unlock();
//}
////-------------------------------------------------------------------------------------------
//void CallBackStreamWaitForm(void* data, int size)
//{
//	gcsWaitForm.lock();
//  //-----------------------------------------------
//	if(pWaitForm==NULL) {BL_FIX_BUG();return;}
//  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
//  switch(type)
//  {
//    case APPL_TYPE_S_WAIT:
//    {
//      char* dataPacket = (char*)data + sizeof(TIP_Port);
//      int sizePacket   = size - sizeof(TIP_Port);
//      TS_Wait S_Wait;
//      S_Wait.setData(dataPacket,sizePacket);
//      pWaitForm->SetCntClient(S_Wait.getWait(),S_Wait.getFight(),S_Wait.getCommon());
//      break;
//    }
//	  default:;
//  }
//	//------------------------------------------------
//	gcsWaitForm.unlock();
//}
////-------------------------------------------------------------------------------------------
//void CallBackDisconnectWaitForm(void* data, int size)
//{
//	gcsWaitForm.lock();
//	//------------------------------------------------
//  if(pWaitForm==NULL) {BL_FIX_BUG();return;}
//  QEvent * event = new QEvent(QEVENT_DISCONNECT);
//  QCoreApplication::postEvent( pWaitForm, event );
//	//------------------------------------------------
//	gcsWaitForm.unlock();
//}
//-------------------------------------------------------------------------------------------
WaitForm::WaitForm(QWidget *parent)
: TBaseGUI(parent)
{
  mWait   = 0;
  mFight  = 0;
  mCommon = 0;

  ui.setupUi(this);

  setWindowTitle(QString(tr("Ожидание, бой %1х%1 ")).arg(COUNT_COMMAND_IN_FIGHT)+tr(STR_VERSION_CLIENT));

  connect(ui.bExitQueue,SIGNAL(clicked()),this,SLOT(sl_ExitQueue()));

  // таймер для отображения времени, проведенного в ожидании, в формат: чч:мм:сс
  mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(sl_Timer()));
}
//---------------------------------------------------------------------------------------------
WaitForm::~WaitForm()
{
}
//---------------------------------------------------------------------------------------------
void WaitForm::sl_ExitQueue()
{
  // отправить запрос на выход из ожидания
  GlobalClientTank.SendRequestExitFromWait();
}
//---------------------------------------------------------------------------------------------
void WaitForm::closeEvent(QCloseEvent* )
{
  _exit(0);
}
//---------------------------------------------------------------------------------------------
void WaitForm::Translate(unsigned short typePacket, char* pData, int size)
{
  switch(typePacket)
  {
    case APPL_TYPE_S_WAIT:
    {
      TS_Wait S_Wait;
      S_Wait.setData(pData,size);
      SetCntClient(S_Wait.getWait(),S_Wait.getFight(),S_Wait.getCommon());
      break;
    }
    //  case QEVENT_A_EXIT_WAIT:
  //    ExitQueueInRoom();
  //    break;
  //  case QEVENT_A_IN_FIGHT:
  //    AnalizCode_A_In_Fight();
  //    break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void WaitForm::sl_Timer()
{
  RefreshInfoStream();

  guint32 delta_ms = ht_GetMSCount()-mStartWait_ms;
  delta_ms /= 1000;
  int hour   = delta_ms/3600;
  int minute = delta_ms/60-hour*60;
  int second = delta_ms%60;
  char str[1000];
  sprintf(str,"%02d:%02d:%02d",hour,minute,second);
  QString str_time = tr("Время ожидания: ")+QString(str);

  ui.lTimeWait->setText(str_time);
}
//---------------------------------------------------------------------------------------------
void WaitForm::SetCntClient(unsigned int wait,unsigned int fight,unsigned int common)
{
  mWait   = wait;
  mFight  = fight;
  mCommon = common;
}
//---------------------------------------------------------------------------------------------
void WaitForm::RefreshInfoStream()
{
  QString str;
  str = QString(tr("Кол-во ожидающих: %1")).arg(mWait);
  ui.lCntWait->setText(str);
  str = QString(tr("Общее кол-во: %1")).arg(mCommon);
  ui.lCntCommon->setText(str);
  str = QString(tr("В бою: %1")).arg(mFight);
  ui.lFight->setText(str);
}
//---------------------------------------------------------------------------------------------
void WaitForm::showGUI()
{
  mTimer->start(eTimeRefresh);
  mStartWait_ms = ht_GetMSCount();
  QWidget::show();
}
//---------------------------------------------------------------------------------------------
void WaitForm::hideGUI()
{
  mTimer->stop();
  QWidget::hide();
}
//---------------------------------------------------------------------------------------------
