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

#ifndef BaseObjectH
#define BaseObjectH


#include "TObject.h"
#include "Struct3D.h"
#include <windows.h>
#include <d3d9types.h>
#include <d3dx10math.h>
#include <vector>
#include "TreeJoint.h"
//#include "ManagerBaseObject.h"


class TBaseObject : public TObject
{
  // свойства, характерные для физики и графики
  // ориентация, координаты, состояние
public:
  TBaseObject();
  virtual ~TBaseObject();

  virtual void SetWorld(D3DXMATRIXA16& world){mWorld=world;}
  virtual void SetState(std::vector<unsigned char>* state);
  virtual void SetID_Model(unsigned int id);
  virtual void SetID_Map(unsigned int id){ID_map = id;}

  virtual unsigned int GetID_Model(){return ID_model;}
  virtual unsigned int GetID_Map(){return ID_map;}
  virtual std::vector<unsigned char>* GetState(){return &mState;}
  virtual D3DXMATRIXA16 GetWorld(){return mWorld;}

  void SetTree(TTreeJoint::TLoadedJoint* pTree);

  void* GetPtrInherits(){return mPtrInherits;}

protected:
  D3DXMATRIXA16* SetOneMatrix(D3DXMATRIXA16* matrix);

protected:
  TTreeJoint::TLoadedJoint* pLoadedTree;
  TTreeJoint mTree;

  unsigned int ID_map;// идентификатор на карте
  unsigned int ID_model;// идентификатор модели

  std::vector<unsigned char> mState;

  D3DXMATRIXA16 mWorld; // здесь вся инфа по ориентации и координатам объекта


  std::vector<unsigned char> mMask;
  std::vector<std::string> mVectorOrderPart;

  // настроить матрицу расположения и ориентации локальных видимых частей
  void SetDefaultMatrix();//### эксперимент
  virtual void SetupState();
  virtual void SetupMask();

  virtual std::vector<std::string>* GetOrderPart(){return &mVectorOrderPart;};

  // маска отрисовки частей модели
  // например, нарисовать Пушку1, а не Пушку0 и т.д.
  // 1 0 0 1 1 1 1


  std::vector<D3DXMATRIXA16*> mVectorMatrix;


  void* mPtrInherits;// назначить в TBaseObjectCommon (решение проблемы виртуального наследования)

};


#endif

