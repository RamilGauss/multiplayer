#ifndef GameFormH
#define GameFormH

#include "BaseGUI.h"

class GameForm : public TBaseGUI
{
  Q_OBJECT

  void* pCallBackExitDirectX;

public:
  GameForm(QWidget *parent = NULL);
  ~GameForm();

  virtual void Translate(unsigned short type, char*pData, int size);

  void setCallbackDirectX(void* _pCallBackExitDirectX){pCallBackExitDirectX=_pCallBackExitDirectX;}

protected:
  friend void CallBackPacketGameForm(void* data, int size);
  friend void CallBackDisconnectGameForm(void* data, int size);

  void Disconnect();

  virtual void closeEvent(QCloseEvent* );
  
  void startDX();// убрать в show и hide
  void stopDX();

protected slots:
  void sl_Exit();

  virtual void showGUI();
  virtual void hideGUI();
};

#endif
