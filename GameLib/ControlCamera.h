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

#ifndef ControlCameraH
#define ControlCameraH

#include "DXUT.h"
#include "Camera.h"
#include <vector>

class IBaseObjectCommon;

class TControlCamera
{
protected:

  TCamera mCamera;
  // параметры для привязки к объекту
  std::vector<IBaseObjectCommon*> mVecObject;
  int mCurIndex;
  float mRadius;
  // параметры привязки к координатам
  float mCoord[3];


  int mMode;

public:
  enum{ eFree          = 0,// режим свободной камеры
        eBindObjectCoord,// привязка в объекту сцены (навигация см. выше)
        eBindObjectFull,
        eBindCoord, // привязка к координатам
  };

  TControlCamera();
  virtual ~TControlCamera();
  //-----------------------------------------------------------------------
  // BindObject
  //-----------------------------------------------------------------------
  virtual void SetListObject(std::vector<IBaseObjectCommon*>* vec);

  // привязать к одному из объектов камеру
  // работают только если включен режим eBindObject иначе игнорируется.
  virtual void SetFirstObject();
  virtual void SetLastObject();

  virtual void SetBelowObject();
  virtual void SetNextObject();
  virtual void SetRadius(float radius);
  //-----------------------------------------------------------------------
  // Setup
  //-----------------------------------------------------------------------
  virtual void SetMode(int type);
  //-----------------------------------------------------------------------
  virtual void SetView(D3DXMATRIXA16* view);
  virtual void SetProj(D3DXMATRIXA16* proj);
  virtual const D3DXMATRIXA16* GetView()const;
  virtual const D3DXMATRIXA16* GetProj()const;
  //-----------------------------------------------------------------------
  // Free
  //-----------------------------------------------------------------------
  // повторяет
  enum{
    // для начала камеры
    eX = 0, // смещение по оси Х
    eY,     //..
    eZ,     //..
    eXa,    // вращение вокруг оси Х
    eYa,    //..
    eZa,    //..
  };
  virtual void Set(int type, float val);
  virtual void Add(int type, float val);
  //-----------------------------------------------------------------------
  // BindCoord
  //-----------------------------------------------------------------------
  virtual void SetCoordBind(float* coord);

protected:
  void Done();

  bool IsModeFree()      {return mMode==eFree;}
  bool IsModeBindObject(){return (mMode==eBindObjectCoord||mMode==eBindObjectFull);}
  bool IsModeBindCoord() {return mMode==eBindCoord;}

  void ZeroCurIndex(){mCurIndex=0;}

protected:
  friend void ObjectEvent(void* p, int s);
  void Notify(void* p, int s);


  void RegisterOnCurObjectEvent();
  void UnregisterOnCurObjectEvent();
};


#endif
