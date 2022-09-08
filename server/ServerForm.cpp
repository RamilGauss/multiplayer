#include "ServerForm.h"
#include <QTimer>
#include <QTableWidgetItem>
#include "ServerStruct.h"
#include "NetSystem.h"

ServerForm* pServerForm = NULL;

void CallBackEventServerForm(void* data, int size)
{
  if(pServerForm==NULL) {BL_FIX_BUG();return;}
  pServerForm->setArrClient(data,size);

  QEvent * event = new QEvent(QEvent::User);
  QCoreApplication::postEvent( pServerForm, event );
}
//-------------------------------------------------------------------------------------------
ServerForm::ServerForm(QWidget *parent)
: QWidget(parent),mServer(SERVER_NUMBER_OF_NETWORK)
{
	mServer.registerRequest(CallBackEventServerForm);

	pArr = NULL;

	pServerForm = this;

  ui.setupUi(this);

  setWindowTitle(tr(STR_VERSION_SERVER));

  // запуск таймера для обновления списка клиентов
  QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(sl_RequestArrClient()));
	timer->start(eTime);
  ui.tableClient->clear();
  ui.tableClient->setRowCount(0);

  ui.tableRoom->clear();
  ui.tableRoom->setRowCount(0);


  mServer.start();
	RefreshTable();
}
//---------------------------------------------------------------------------------------------
ServerForm::~ServerForm()
{
  mServer.stop();
	Done();
}
//---------------------------------------------------------------------------------------------
void ServerForm::customEvent( QEvent * event  )
{
	switch(event->type())
	{
		case QEvent::User:
			RefreshTable();
		default:;
	}
}
//---------------------------------------------------------------------------------------------
void ServerForm::RefreshTable()
{
  if(pArr)
  {
    ui.tableClient->setRowCount(pArr->cntClient);
    ui.tableRoom->setRowCount(pArr->cntRoom);
  }
  else
  {
    ui.tableClient->setRowCount(0);
    ui.tableRoom->setRowCount(0);
  }

	if(pArr==NULL) return;
  
  RefreshTableClient();
  RefreshTableRoom();
}
//---------------------------------------------------------------------------------------------
void ServerForm::setArrClient(void* data,int size)
{
	Done();
	if(data)
		pArr = (ServerTank::TAnswerRequest*)data;
}
//---------------------------------------------------------------------------------------------
void ServerForm::Done()
{
	delete pArr;
	pArr = NULL;
}
//---------------------------------------------------------------------------------------------
void ServerForm::sl_RequestArrClient()
{
	mServer.requestListClient();
}
//---------------------------------------------------------------------------------------------
void ServerForm::RefreshTableClient()
{
  ServerTank::TDefClient * pArrClient = pArr->mArrClient;
  int cntClient                       = pArr->cntClient;

  for(int i = 0 ; i < cntClient ; i++)
  {
    // имя 
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(tr(pArrClient[i].name));
    if(pArrClient[i].flgDisconnect)
      item->setForeground(QBrush(QColor(0xff,0,0)));
    else
      item->setForeground(QBrush(QColor(0,0xff,0)));

    ui.tableClient->setItem(i, 0, item);

    // состояние
    QString sState;
    switch(pArrClient[i].state)
    {
    case nsServerStruct::TClient::eGarage:
      sState = tr("Гараж");
      break;
    case nsServerStruct::TClient::eWait:
      sState = tr("Ждет");
      break;
    case nsServerStruct::TClient::eFight:
      sState = tr("Бой");
      break;
    default:BL_FIX_BUG();;
    }
    item = new QTableWidgetItem();
    item->setText(sState); 
    ui.tableClient->setItem(i, 1, item);
    // ip:port
    char sIP_Port[100];
    sprintf(sIP_Port,"%s:%u",ns_str_addr(pArrClient[i].ip),pArrClient[i].port);
    QString strIP_Port = tr(sIP_Port);
    item = new QTableWidgetItem();
    item->setText(strIP_Port); 
    ui.tableClient->setItem(i, 2, item);
  }
  ui.tableClient->resizeColumnsToContents();
}
//-------------------------------------------------------------------------------------------
void ServerForm::RefreshTableRoom()
{
  ServerTank::TDefRoom * pArrRoom = pArr->mArrRoom;
  int cntRoom                     = pArr->cntRoom;

  for(int i = 0 ; i < cntRoom ; i++)
  {
    // имя 
    QTableWidgetItem* item = new QTableWidgetItem;
    char s_time_rest[100];
    sprintf(s_time_rest,"%u:%u",pArrRoom[i].time_rest/60,pArrRoom[i].time_rest%60);
    item->setText(tr(s_time_rest));
    ui.tableRoom->setItem(i, 0, item);
    //-------------------------------------------------------
    item = new QTableWidgetItem();
    char s_active[100];
    sprintf(s_active,"%d",pArrRoom[i].cntActiveClient);
    item->setText(tr(s_active)); 
    ui.tableRoom->setItem(i, 1, item);
  }
  ui.tableRoom->resizeColumnsToContents();
}
//-------------------------------------------------------------------------------------------