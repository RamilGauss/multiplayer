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


#ifndef BaseObjectDXH
#define BaseObjectDXH

#include "ModelDX.h"
#include "BaseObject.h"
#include <vector>
#include "TreeJoint.h"
#include "glibconfig.h"

class TManagerObjectDX;

class IBaseObjectDX : virtual public IBaseObject
{
protected:

  bool flgShow;// показан ли объект на сцене
  float mAlphaTransparency;// прозрачность всего объекта, нельзя задать прозрачность части модели
  
public:
  IBaseObjectDX(int typeDX = eDirtyAnimate);
  virtual ~IBaseObjectDX();

  void SetModel(TModelDX* pModel);
  TModelDX* GetModel();

  void Draw(D3DXMATRIXA16* mView);

  void SetShow(bool show){flgShow=show;}

  // вернет true - объект жив, false - закончил жить
  // должно быть задано время начало жизни, см. IBaseObject::mTimeCreation - хрень хрень
  // только для полностью анимированных объектов!!!
  // данная функция подготавливает данные для стека шейдера для данного момента времени
  // окончательная реализация стека должна быть в методе SetupShaderStackModelDX
  virtual bool Animate(guint32 time_ms/*dirty animate ignore this parameter*/) = 0;//{return true;};

  enum{eDirtyAnimate = 0,
       ePureAnimate  = 1,// см. mTypeDX
  };
  int GetTypeDX(){return mTypeDX;}
  void SetTimeCreation(guint32 t){mTimeCreation=t;};

  float GetAlphaTransparency(){return mAlphaTransparency;}
  void  SetAlphaTransparency(float val){mAlphaTransparency = val;}

protected:
  TModelDX* mModel;// внешний вид 

  int mTypeDX;

  // время создания, необходимо для расчета анимации
  guint32 mTimeCreation;// мс
protected:

  void Done();

  // такие же функции должны быть и в Prediction
  void SetupVectorNamePart();
  void SetupVectorOrderPart();
  
private:
  virtual void SetupShaderStackModelDX(){};// для более тонкой настройки шейдера
  virtual void EventSetModelDX(){};
};
//-----------------------------------------------------------------



#endif