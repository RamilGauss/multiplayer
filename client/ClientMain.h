#ifndef ClientMainH
#define ClientMainH

#include <QWidget>
#include "ui_ClientMain.h"
#include "ApplicationProtocolPacket.h"
#include "BaseGUI.h"


class ClientMain : public TBaseGUI
{
  Q_OBJECT

public:
  ClientMain(QWidget *parent = NULL);
  ~ClientMain();

  void Connect(char* sNick, char* ip,char* port);// поддержка скрипт-управления

  virtual void showGUI(){QWidget::show();};
  virtual void hideGUI(){QWidget::hide();};

protected slots:
  void sl_Exit();
  void sl_Enter();

protected:
  void SaveInReestr(unsigned int ip, unsigned int port,QString& nick);
  void LoadFromReestr();

protected:
  virtual void Translate(unsigned short typePacket, char* pData, int size);
	virtual void closeEvent(QCloseEvent* );

private:
  Ui::ClientMainClass ui;
};

#endif
