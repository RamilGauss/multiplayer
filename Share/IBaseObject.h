/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef BaseObjectH
#define BaseObjectH


#include "TObject.h"
#include "Struct3D.h"
#include <vector>
#include "TreeJoint.h"
#include "CallBackRegistrator.h"
#include <map>


class SHARE_EI IBaseObject : public TObject
{
protected:
  TCallBackRegistrator mCallBackEvent;
  // свойства, характерные дл€ физики и графики
  // ориентаци€, координаты, состо€ние
public:
  enum{
    eWorld = 0, // значение мировой матрицы
    eState,
    eMapUse,
  };

  IBaseObject();
  virtual ~IBaseObject();

  void SetWorld(nsStruct3D::TMatrix16* world);
  void SetID_Model(unsigned int id);
  void SetID_Map(unsigned int id){ID_map = id;}

  unsigned int GetID_Model(){return ID_model;}
  unsigned int GetID_Map(){return ID_map;}
  std::vector<unsigned char>* GetState(){return &mState;}
  const nsStruct3D::TMatrix16* GetWorld()const {return &mWorld;}

  void SetVelocity(float v){mV=v;}// м/с - хрень
  float GetVelocity(){return mV;}

  void SetTree(TTreeJoint::TLoadedJoint* pTree);
  void SetMapUse(std::map<std::string,int>* mapUse = NULL);// использовать при смене используемых частей и при (собственно) начальном задании
  void SetState(std::vector<unsigned char>* state);
  
  void GetDefaultMapUse(std::map<std::string,int>* mapUse);
  
  void* GetPtrInherits(){return mPtrInherits;}

  // регистраци€ на получение событий объекта
  void RegisterOnEvent(TCallBackRegistrator::TCallBackFunc pFunc);
  void UnregisterOnEvent(TCallBackRegistrator::TCallBackFunc pFunc);

  // наклон дл€ камеры
  virtual float GetRoll(){return 0.0f;}// радианы

protected:
  
  float mV;// скорость, м/с
  float mA;// ускорение, м/с^2

  TTreeJoint::TLoadedJoint* pLoadedTree;
  TTreeJoint mTree;

  unsigned int ID_map;  // идентификатор на карте
  unsigned int ID_model;// идентификатор модели

  nsStruct3D::TMatrix16 mWorld; // здесь вс€ инфа по ориентации и координатам объекта

  std::vector<unsigned char> mState;// графика, говорит движку какую из частей рисовать, дл€ физики определ€ет поведение объекта
  std::vector<unsigned char> mMask; // и физика и графика какие части рисовать и использовать
  std::vector<nsStruct3D::TMatrix16*> mVectorMatrix;// и физика и графика, cnt=cntAllJoint

  // size = sizeAllPart
  struct TPart 
  {
    std::string name;
    int use;
  };
  std::vector<TPart> mVectorNamePart;// перечислены имена частей моделей. графический вектор должен точно совпадать.
  // этот вектор используетс€ дл€ заполнени€ маски частей
  //size = sizeAllJoint
  std::vector<std::string> mVectorOrderPart;// пор€док названий частей, без повторений, cnt=cntJoint
  std::map<std::string, int>  mMapUse;// cnt=cntAllPartModel

  // настроить матрицу расположени€ и ориентации локальных видимых частей
  void SetDefaultMatrix();//### эксперимент
  void SetupState();
  void SetupMask();

  void SetupDefaultMapUse();

  // маска отрисовки частей модели
  // например, нарисовать ѕушку1, а не ѕушку0 и т.д.
  // 1 0 0 1 1 1 1


  void* mPtrInherits;// назначить в IBaseObjectCommon (решение проблемы виртуального наследовани€)

  int GetCountPart(const char* name, std::vector<std::string>* pVec);

protected:
  void Notify(int event);
};


#endif

