#ifndef ServerFormH
#define ServerFormH

#include <QWidget>
#include "ui_ServerForm.h"
#include "ServerTank.h"


class ServerForm : public QWidget
{
  Q_OBJECT

		enum{eTime=5000};

	int                     mCntClient;
	ServerTank::TDefClient* pArrClient;

protected:
	ServerTank mServer;

public:
  ServerForm(QWidget *parent = NULL);
  ~ServerForm();

protected slots:
	void sl_RequestArrClient();

protected:
	virtual void customEvent( QEvent * event  );

  friend void CallBackEventServerForm(void* data, int size);

	void setArrClient(void* data,int size);
	void Done();
	void RefreshTable();	

private:
  Ui::ServerFormClass ui;
};

#endif
