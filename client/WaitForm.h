#ifndef WaitFormH
#define WaitFormH

#include <QWidget>
#include "ui_WaitForm.h"
#include "glibconfig.h"
#include "BaseGUI.h"

class QTimer;

class WaitForm : public TBaseGUI
{
  Q_OBJECT

  QTimer* mTimer;

  guint32 mStartWait_ms;
  enum{eTimeRefresh=1000,};

  unsigned int mWait;
  unsigned int mFight;
  unsigned int mCommon;

public:
  WaitForm(QWidget *parent = NULL);
  ~WaitForm();

  virtual void Translate(unsigned short typePacket, char* pData, int size);

  virtual void showGUI();
  virtual void hideGUI();


protected slots:
  void sl_ExitQueue();
  void sl_Timer();

protected:
  virtual void closeEvent(QCloseEvent* );

  void ExitQueueInRoom();
  void SetCntClient(unsigned int wait,unsigned int fight,unsigned int common);

  void RefreshInfoStream();


private:
  Ui::WaitFormClass ui;
};

#endif
