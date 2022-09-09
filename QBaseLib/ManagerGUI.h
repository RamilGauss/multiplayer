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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef ManagerGUIH
#define ManagerGUIH

#include <iostream>
#include <string>
#include <map>

#include <QObject>
#include <QEvent>
#include <QMap>

#include "BL_Debug.h"
#include "BaseGUI_DX.h"


//-------------------------------------------------------------------------------------------
class TManagerGUIEvent: public QEvent
{
  int mSize;
  char* pData;
  unsigned short typePacket;

public:
  TManagerGUIEvent(Type type):QEvent(type)
  {
    mSize = 0;
    pData = NULL;
  }
  ~TManagerGUIEvent()
  {
    mSize = 0;
    delete pData;
    pData = NULL;
  }
  void SetTypePacket(unsigned short val){typePacket=val;}
  unsigned short GetType(){return typePacket;}
  void SetData(char* data, int size)
  {
    pData = data;
    mSize = size;
  }
  char* GetData(int &size)
  {
    char* data = pData;
    size = mSize;

    mSize = 0;
    pData = NULL;
    return data;
  }
};
//-------------------------------------------------------------------------------------------
class TManagerGUI : public QObject
{
public:
  
  TManagerGUI(QWidget* parent=NULL);
  virtual ~TManagerGUI();
  
  void startGUI(TClientTank*_pClient,TManagerObjectCommon*_pGame);
  void stopGUI();

  void AddFormInList(TBaseGUI* pStrForm, char* name);

protected:

  // транспорт
  TClientTank*                  pClient;
  // управляющий двигателями
  TManagerObjectCommon*         pGame;

  TBaseGUI* pCurrentForm;

  std::map<std::string,void*> mMap;

protected:

  virtual void StartEvent() = 0;
  virtual void StopEvent() = 0;

  virtual void customEvent( QEvent * e );

  virtual void WorkStream(TManagerGUIEvent* event);// чистая трансляция данных
  virtual void WorkPacket(TManagerGUIEvent* event) = 0;// принятие решения об открытии формы
  
  virtual void OpenFirstForm() = 0;

  void OpenForm(char* pStrForm);

  TBaseGUI* FindForm(char* pStrForm)
  {
    std::map<std::string,void*>::iterator ifind = mMap.find(pStrForm);
    BL_ASSERT(ifind->second);
    return (TBaseGUI*)ifind->second;
  }
protected:
  //---------------------------------------------------------------------------------------------
  void Done();
};

#endif
