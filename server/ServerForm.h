#ifndef ServerFormH
#define ServerFormH

#include <QWidget>
#include "ui_ServerForm.h"
#include "ServerTank.h"


class ServerForm : public QWidget
{
  Q_OBJECT

		enum{eTime=5000};

	ServerTank::TAnswerRequest* pArr;

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

  void RefreshTableClient();
  void RefreshTableRoom();

private:
  Ui::ServerFormClass ui;
};

#endif
