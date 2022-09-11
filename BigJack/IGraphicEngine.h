/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef IGraphicEngineH
#define IGraphicEngineH

#include "glibconfig.h"
#include "Struct3D.h"
#include "SrcEvent.h"
#include <list>

#define STR_NAME_GE "GE"

class IBaseObjectGE;
class IGUI_Core;
class ICamera;

class IGraphicEngine : public TSrcEvent
{
  bool flgCreateWindow;

protected:
  unsigned int mTime_ms;// время для рендера, используется для анимации
  
  IGUI_Core* mGUI;// NOT Thread Safe

  ICamera* mICamera;                                // кросс платформенная камера

public:
	enum{
        eEnvMapSizeCubemap   = 312,// размер для буфера под CubeMap
        };

  IGraphicEngine(ICamera* pCamera);
  virtual ~IGraphicEngine();

  void SetGUI(IGUI_Core* pGUI);
  void ZeroGUI();

  virtual bool IsFullScreen() = 0;
  virtual void ToggleFullScreen() = 0;
  virtual void SetTitleWindow(const char* sTitle) = 0;
  //------------------------------------------------------------------------
  virtual void Init() = 0;
  virtual void Work(unsigned int time_ms) = 0;
  virtual bool HandleInternalEvent() = 0;// true - success, false - exit
  virtual void Done() = 0;
  virtual void GetResolutionFrame(int& h, int& w ) = 0;// формат X8R8G8B8
  virtual void SetResolutionFrame(int  h, int  w ) = 0;// формат X8R8G8B8
  //------------------------------------------------------------------------
  virtual void AddObject(IBaseObjectGE* pObject) = 0;
  virtual void Clear() = 0;

  // клиентские эффекты движка, не влияют на физические параметры объектов
  virtual void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
    nsStruct3D::TVector3* coord3,     // где
    nsStruct3D::TVector3* orient3,    // ориентация эффекта
    unsigned int time_past/* прошло времени, мс*/ = 0) = 0;
  virtual void SetViewFPS(bool val) = 0;
  // источники освещения
  // ввод освещения накладывает условия на шейдер. он обязан содержать интерфейс
  virtual void SetLightEnable(int index, bool v) = 0;
  virtual bool GetLightEnable(int index) = 0;
  virtual int  GetCountAllLight() = 0;

	virtual const nsStruct3D::TVector3* GetLightPosition(int index) = 0;	
	virtual void SetLightPosition(int index,nsStruct3D::TVector3* m3) = 0;
	virtual const nsStruct3D::TVector3* GetLightColor(int index) = 0;
	virtual void SetLightColor(int index, nsStruct3D::TVector3* color) = 0;

	virtual bool GetPostEffectMirror() = 0;
	virtual void SetPostEffectMirror(bool v) = 0;

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

