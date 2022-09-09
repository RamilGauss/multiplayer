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


#ifndef IGraphicEngineH
#define IGraphicEngineH

#include "glibconfig.h"
#include "Struct3D.h"
#include "SrcEvent.h"
#include <list>

class IBaseObjectGE;
class IGUI_Core;

class IGraphicEngine : public TSrcEvent
{
  bool flgCreateWindow;

protected:
  guint32 mTime_ms;// время для рендера, используется для анимации
  
  IGUI_Core* mGUI;// NOT Thread Safe


public:

  IGraphicEngine();
  virtual ~IGraphicEngine();

  void SetGUI(IGUI_Core* pGUI);
  void ZeroGUI();

  virtual bool IsFullScreen() = 0;
  virtual void ToggleFullScreen() = 0;
  virtual void SetTitleWindow(const char* sTitle) = 0;
  //virtual void ForceResizeEventGUI() = 0;
  //------------------------------------------------------------------------
  virtual void Init() = 0;
  virtual void Work(guint32 time_ms) = 0;
  virtual bool HandleInternalEvent() = 0;// true - success, false - exit
  virtual void Done() = 0;
  virtual void GetResolutionFrame(int& h, int& w ) = 0;// формат X8R8G8B8
  virtual void SetResolutionFrame(int  h, int  w ) = 0;// формат X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectGE* pObject) = 0;
  virtual void Clear() = 0;
  // камера
  virtual void SetViewParams(nsStruct3D::TVector3* pvEyePt3, nsStruct3D::TVector3* pvLookatPt3) = 0;// расположение камеры

  // 21 декабря 2012 года реализую:
  // клиентские эффекты движка, не влияют на физические параметры объектов
  virtual void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
    nsStruct3D::TVector3* coord3,     // где
    nsStruct3D::TVector3* orient3,    // ориентация эффекта
    guint32 time_past/* прошло времени, мс*/ = 0) = 0;
  virtual void SetViewFPS(bool val) = 0;
  // источники освещения
  // ###
  // ввод освещения накладывает условия на шейдер. он обязан содержать интерфейс
  virtual int GetCountLight() = 0;
  virtual const float* GetCoordLight(int index) = 0;
  virtual const float* GetCoordAtLight(int index) = 0;
  virtual unsigned int GetColorLight(int index) = 0;
  virtual void SetCoordLight(int index,nsStruct3D::TVector3* m3) = 0;
  virtual void SetCoordAtLight(int index,nsStruct3D::TVector3* m3) = 0;
  virtual void SetColorLight(int index, unsigned int color) = 0;
  virtual void AddLight() = 0;
  virtual void RemoveLight(int index) = 0;
  // ###

  typedef enum
  {
    eKeyEmpty = 0,
    eAlt   = 1<<1,
    eCtrl  = 1<<2,
    eShift = 1<<3,
  }tKeyModifier;
  typedef enum
  {
    eMouseEmpty = 0,
    eLClick = 1<<1,
    eMClick = 1<<2,
    eRClick = 1<<3,
  }tMouseButton;
  typedef enum
  {
    eButtonDown     = 1<<1,
    eButtonUp       = 1<<2,
    eButtonDblClick = 1<<3,
    eWheel          = 1<<4,
    eMove           = 1<<5,
  }tTypeMouseEvent;

  struct TKeyEvent
  {
    int key;
    bool pressed/*or released*/;
    tKeyModifier modifier;
  };
  struct TMouseEvent
  {
    tTypeMouseEvent state;
    int x;
    int y;
    tMouseButton button;
    int delta_wheel;
  };
protected:

  // работа с GUI
  virtual bool InitGUI() = 0;

  void SetIsCreateWindow(bool val){flgCreateWindow=val;}
  bool IsCreateWindow(){return flgCreateWindow;}
  void RenderGUI();
  void ResizeGUI();
  void LostDeviceGUI();

};


#endif

