#ifndef GameRoomPrepareH
#define GameRoomPrepareH

#include <QWidget>
#include "ui_GameRoomPrepare.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "BaseGUI.h"

class QTimer;

class GameRoomPrepare : public TBaseGUI
{
  Q_OBJECT

	enum{
		eTimeRefreshCntClient = 1000,
	};

	volatile unsigned int mCntCommon;
	volatile unsigned int mCntInFight;

  TA_Get_List_Tank mPacketListTank;

  QTimer* mTimer;
  
public:
  GameRoomPrepare(QWidget *parent = NULL);
  ~GameRoomPrepare();

  virtual void Translate(unsigned short type, char*pData, int size);
  virtual void showGUI();
  virtual void hideGUI();

protected:
	void SetCntClient(unsigned int common,unsigned int fight);

	virtual void closeEvent(QCloseEvent* );

  void SendRequestListTank();
  void ViewListTanks();
  void AddListTank(char* data, int size);

protected slots:
	void sl_RefreshCntClient();
	void sl_Exit();
  void sl_ChangeCurTank();
  void sl_InFight();

private:
  Ui::GameRoomPrepareClass ui;
};

#endif
