/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include <QtGui/QApplication>

#include <QtCore/QTextCodec>
#include <QWindowsStyle>
#include <QWidget>
#include "HiTimer.h"
#include "NetSystem.h"
#include "ErrorReg.h"
#include <glib/gthread.h>
#include "BaseGUI_DX.h"
#include "ManagerObjectCommon.h"
#include "ManagerGUI.h"


//------------------------------------------------------
class TBASE : public TBaseGUI_DX
{
public:

  ~TBASE()
  {
    int a = 0;
  }
  virtual void Translate(unsigned short typePacket, char* pData, int size){};
  virtual void VisualEvent(QPaintEvent* pEvent){};

};
//------------------------------------------------------
class TMOC : public TManagerObjectCommon
{
public:
  ~TMOC()
  {
    int a = 0;
  }
  virtual void VisualEvent(guint32 iTime, float fElapsedTime){};

};
//------------------------------------------------------
class TMGUI : public TManagerGUI
{
public:
  ~TMGUI()
  {
    int a = 0;
  }
  virtual void WorkPacket(TManagerGUIEvent* event){};// принятие решения об открытии формы

  virtual void OpenFirstForm(){};

};
//------------------------------------------------------
class TPart : public QWidget
{
  //Q_OBJECT

public:
  ~TPart()
  {
    int a = 0;
  }
};
//------------------------------------------------------
class TWidget : public QWidget
{
  //Q_OBJECT

  TPart * pPart;
public:
  TWidget() : QWidget(NULL)
  {
    pPart = NULL;
  }
  ~TWidget()
  {
    delete pPart;
  }
  void set(TPart * p)
  {
    pPart = p;
  }

};
//------------------------------------------------------
int main(int argc, char *argv[])
{
  QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

  QApplication a(argc, argv);

#if 0
  //---------------------------------------
  // менеджер двигателей
  TManagerObjectCommon* pMOC = new TMOC;

  TManagerGUI* pMGUI = new TMGUI;
  pMGUI->startGUI(NULL,pMOC);

  TBASE* base = new TBASE;
  pMGUI->AddFormInList(base, "base");
#else
  TManagerObjectCommon* pMOC = new TMOC;
  TWidget * pW = new TWidget;
  pMOC->setSomethingAss(pW);
  TPart* pPart = new TPart;
  pW->set(pPart);
  pPart->show();
#endif
  a.exec();
#if 0
  delete pMGUI;
#else
  delete pW;
#endif

  return 0;
}
