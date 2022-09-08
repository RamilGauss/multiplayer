/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#ifndef ManagerGUIH
#define ManagerGUIH

#include <QObject>
#include <QEvent>

#include "BL_Debug.h"
#include "BaseGUI_DX.h"
#include <map>

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
template <class Robert, class Client, class BigJack > // какие-то данные, 
//было классно если бы можно было: template <class T0, ... >
class TManagerGUI : public QObject
{
public:
  
  typedef TBaseGUI   < Robert, Client, BigJack> TBaseGUI_G;// игровые формы
  typedef TBaseGUI_DX< Robert, Client, BigJack> TBaseGUI_DX_G;

  TManagerGUI(QWidget* parent=NULL);
  virtual ~TManagerGUI();
  
  void start(Robert  *_pRobert,
             BigJack *_pBigJack,
             Client  *_pClient );
  void stop();

  void AddForm(TBaseGUI_G* pStrForm, char* name);

protected:
  
  Robert  *pRobert;
  BigJack *pBigJack;  
  Client  *pClient;

  TBaseGUI_G* pCurrentForm;

protected:

  virtual void customEvent( QEvent * e  );

  virtual void WorkStream(TManagerGUIEvent* event);// чиста€ трансл€ци€ данных
  virtual void WorkPacket(TManagerGUIEvent* event) = 0;// прин€тие решени€ об открытии формы
  
  virtual void OpenFirstForm();

  void OpenForm(char* pStrForm);
  TBaseGUI_G* FindForm(char* pStrForm)
  {
    std::list<TDescForm>::iterator bit = list.begin();
    std::list<TDescForm>::iterator eit = list.end();
    while(bit!=eit)
    {
      TBaseGUI* pForm = (*bit).pForm;
      char* name = (*bit).Name();
      if(strcmp(pStrForm,name)==0)
        return pForm;
      bit++;
    }

    BL_FIX_BUG();
    return NULL;
  }
  //---------------------------------------------------------------------------------------------
  void Done();

protected:
  struct TDescForm
  {
    TBaseGUI_G* pForm;
    TDescForm()
    {
      name = NULL;
    }
    ~TDescForm()
    {
      delete[] name;
    }
  protected:
    char* name;
  public:
    char* Name(){return name;}
    void SetName(char* str)
    {
      int len = strlen(str);
      delete[] name;
      name = new char[len+1];
      strcpy(name,str);
    }
  };

  std::list<TDescForm> list;
};

#endif