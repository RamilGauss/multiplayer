/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#include "ManagerGUI.h"
#include <QMessageBox>

template <class Robert, class Client, class BigJack >
TManagerGUI<Robert, Client, BigJack >::TManagerGUI(QWidget* parent):QObject(parent)
{
  pRobert  = NULL;
  pBigJack = NULL;
  pClient  = NULL;

  pCurrentForm = NULL;
}
//-----------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
TManagerGUI<Robert, Client, BigJack >::~TManagerGUI()
{
  Done();
}
//-----------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::start(Robert  *_pRobert,
                                                  BigJack *_pBigJack,
                                                  Client  *_pClient )
{
  pRobert  = _pRobert;
  pBigJack = _pBigJack;
  pClient  = _pClient; 

  if(pCurrentForm)
    pCurrentForm->showGUI();
}
//-----------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::stop()
{
  _exit(0);
}
//-----------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::customEvent( QEvent * e  )
{
  TManagerGUIEvent* event = (TManagerGUIEvent*)e;
  switch(event->type())
  {
    case QEVENT_DISCONNECT:
      Q_MESSAGE(tr("������ ����� � ��������."))
      // ������� ��������� �����
      OpenFirstForm();
      break;
    case QEVENT_PACKET:
      WorkPacket(event);    
      break;
    case QEVENT_STREAM:
      WorkStream(event);
      break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::OpenForm(char* pStrForm)
{
  // ������� ����� ��������
  pCurrentForm->hideGUI();
  pCurrentForm = FindForm(pStrForm);
  pCurrentForm->showGUI();
}
//---------------------------------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::AddForm(TBaseGUI_G* pForm, char* name)
{
  if(pCurrentForm==NULL)
    pCurrentForm = pForm;

  pForm->setup(pRobert,pBigJack,pClient);
  //-----------------------------------------------------------------------------
  TDescForm desc;
  desc.pForm = pForm;
  desc.SetName(name);
  list.push_back(desc);
}
//---------------------------------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::Done()
{
  list.clear();
}
//---------------------------------------------------------------------------------------------
template <class Robert, class Client, class BigJack >
void TManagerGUI<Robert, Client, BigJack >::WorkStream(TManagerGUIEvent* event)
{
  int size;
  char* pData = event->GetData(size);
  unsigned short type = event->GetType();
  switch(type)
  {
    default:
    {  
      if(pCurrentForm)
        pCurrentForm->Translate(type,pData,size);
    };
  }
  delete pData;
}
//---------------------------------------------------------------------------------------------