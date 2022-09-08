#ifndef ManagerGUIH
#define ManagerGUIH

#include "GameForm.h"
#include "GameRoomPrepare.h"
#include "WaitForm.h"
#include "ClientMain.h"
#include <QObject>

class TManagerGUIEvent;

class TManagerGUI : public QObject
{

  Q_OBJECT

public:
  TManagerGUI(QWidget* parent=NULL);
  ~TManagerGUI();
  
  void start();
  void stop();

  void Connect(char* sNick, char* ip,char* port);// поддержка скрипт-управления

protected:
  
  TBaseGUI* pCurrentForm;


// формы
  GameForm mGameForm;
  GameRoomPrepare mGameRoomPrepare;
  WaitForm mWaitForm;
  ClientMain mClientMain;

  void WorkStream(TManagerGUIEvent* event);
  void WorkPacket(TManagerGUIEvent* event);

  virtual void customEvent( QEvent * e  );

  void AnswerFromServer_Enter(unsigned char mCodeAnswer);
  void AnalizCode_A_In_Fight(char* pData, int size);

  void OpenWaitForm();
  void OpenGameForm();
  void OpenRoomForm();
  void OpenClientMainForm();


  void LoadMap(TA_In_Fight& packet);
};

#endif