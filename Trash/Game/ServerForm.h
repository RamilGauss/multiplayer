/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
??????? ?????? ????????? 
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
the "Tanks" Source Code.  If not, please request a copy in writing from  at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ServerFormH
#define ServerFormH

#include <QWidget>
#include "ui_ServerForm.h"
#include "ServerTank.h"


class ServerForm : public QWidget
{
  Q_OBJECT

		enum{eTime=5000};

	ServerTank::TAnswerRequest* pAnswerRequest;

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
