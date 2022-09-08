#ifndef BaseGUIH
#define BaseGUIH

#include <QWidget>

class TBaseGUI : public QWidget
{

public:
  TBaseGUI(QWidget* parent):QWidget(parent){};
  virtual ~TBaseGUI(){};

  virtual void Translate(unsigned short typePacket, char* pData, int size) = 0;

  virtual void showGUI() = 0;
  virtual void hideGUI() = 0;


};

#endif

