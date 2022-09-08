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


#ifndef ManagerDirectXH
#define ManagerDirectXH

#include "DXUT.h"
#include "DXUTcamera.h"
#include <list>
#include "Struct3D.h"
#include "ManagerModelDX.h"


class TBaseObjectDX;

struct IDirect3DDevice9;

// BigJack - графический движок
class TManagerDirectX
{
protected:

  TManagerModelDX mManagerModel;

  // все объекты
  std::list<TBaseObjectDX*> mListAllObject;
  // список на отрисовку
  std::list<TBaseObjectDX*> mListReadyRender;

  CModelViewerCamera mCamera;                // A model viewing camera

  D3DXHANDLE         hmWorldViewProjection;
  D3DXHANDLE         hmWorld;
  D3DXHANDLE         hfTime;

  IDirect3DDevice9* mD3DDevice;
  
public:
  //----------------------------------------------------------------
  //                              INTERFACE
  //----------------------------------------------------------------
  TManagerDirectX();
  ~TManagerDirectX();
  void CreateDeviceEvent(IDirect3DDevice9* pd3dDevice); 
  void VisualEvent( guint32 iTime, float fElapsedTime);

  void AddObject(TBaseObjectDX* pObject);
  void Clear();
  // камера
  void SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt);// расположение камеры
  
  // 21 декабря 2012 года реализую:
  // клиентские эффекты движка, не влияют на физические параметры объектов
  //void SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
  //               D3DXMATRIXA16* loc/*местоположение*/,
  //               double time_rest/*прошедшее время*/ = 0);
  //----------------------------------------------------------------
  //                             ~INTERFACE
  //----------------------------------------------------------------
protected:
  D3DXHANDLE* getWorldViewProjection(){return &hmWorldViewProjection;}
  D3DXHANDLE* getWorld(){return &hmWorld;}
  D3DXHANDLE* getTime(){return &hfTime;}

protected:

  void Optimize();
  void Render(guint32 fTime = 0, float fElapsedTime = 0);
};

#endif