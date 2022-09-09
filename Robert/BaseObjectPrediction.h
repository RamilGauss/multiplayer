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


#ifndef BaseObjectPredictionH
#define BaseObjectPredictionH

#include "BoundingBox.h"
#include "BaseObject.h"

class TBaseObjectPrediction : virtual public TBaseObject
{
public:
  // тип объекта, важно для Prediction
  // 
 /* typedef 
    enum{
       eTank    = 0,
       eBullet  = 1,
       eObject  = 2,
       eTree    = 3,
       eTerrain = 4,
       eBush    = 5,
  } eTypeObject;*/
protected:

  //char* pUserData;    // в клиенте хранит TObjectDX*, в сервере хранит id
  //int   mUserDataSize;

  //unsigned short mID;// тип
  //unsigned char mType;// см. eTypeObject

public:
  //=====================================
  //unsigned int mMaskState;// конечная цель существования объекта - определить состояние этой переменной mMaskState
  // 1 - нормальное, 0 - разрушенное
  
  //nsStruct3D::TCoord3  mCoord; // конечная цель существования объекта
  //nsStruct3D::TOrient3 mOrient;// 
  //=====================================

  TBoundingBox mBBCommon;// определить коллизии, грубая оценка

  struct TDef
  {
    unsigned short HP;// что бы разрушить надо иметь энергию при вхождении в объект при коллизии
    TBoundingBox   BB;// более точное определение коллизии
    unsigned int   mask;//
  };

  TDef* mArrDef;
  int   mCntDef;


public:
  
  TBaseObjectPrediction();
  virtual ~TBaseObjectPrediction();

  //unsigned short GetID();
  //void SetID(unsigned short id);
  //unsigned char GetType(){return mType;}

  //unsigned int GetMaskState();

  //void SetUserData(char* pData, int size);
  //void GetUserData(char* pData, int& size);

protected:
  //void CleanUserData();
  //void CleanArrDef();
  virtual void SetupVectorNamePart();

};
#endif