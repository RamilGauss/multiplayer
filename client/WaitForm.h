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
