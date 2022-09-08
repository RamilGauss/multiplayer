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

	mCntClient = 0;
	pArrClient = NULL;

	pServerForm = this;

  ui.setupUi(this);

  setWindowTitle(tr(STR_VERSION_SERVER));

  // запуск таймера для обновления списка клиентов
  QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(sl_RequestArrClient()));
	timer->start(eTime);
  ui.table->clear();
  ui.table->setRowCount(0);
  
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
	ui.table->setRowCount(mCntClient);

	if(pArrClient==NULL) return;
	
	for(int i = 0 ; i < mCntClient ; i++)
	{
    // имя 
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(tr(pArrClient[i].name));
    if(pArrClient[i].flgDisconnect)
      item->setForeground(QBrush(QColor(0xff,0,0)));
    else
      item->setForeground(QBrush(QColor(0,0xff,0)));

    ui.table->setItem(i, 0, item);

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
    ui.table->setItem(i, 1, item);
    // ip:port
    char sIP_Port[100];
    sprintf(sIP_Port,"%s:%u",ns_str_addr(pArrClient[i].ip),pArrClient[i].port);
    QString strIP_Port = tr(sIP_Port);
    item = new QTableWidgetItem();
    item->setText(strIP_Port); 
    ui.table->setItem(i, 2, item);
	}
	ui.table->resizeColumnsToContents();
}
//---------------------------------------------------------------------------------------------
void ServerForm::setArrClient(void* data,int size)
{
	Done();
	if(data)
	{
		pArrClient = (ServerTank::TDefClient*)data;
		mCntClient = size/sizeof(ServerTank::TDefClient);
	}
}
//---------------------------------------------------------------------------------------------
void ServerForm::Done()
{
	delete pArrClient;
	pArrClient = NULL;
	mCntClient = 0;
}
//---------------------------------------------------------------------------------------------
void ServerForm::sl_RequestArrClient()
{
	mServer.requestListClient();
}
//---------------------------------------------------------------------------------------------