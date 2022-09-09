/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013, 2013, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BaseObjectPredictionH
#define BaseObjectPredictionH

#include "BoundingBox.h"
#include "IBaseObject.h"

class IBaseObjectPrediction : virtual public IBaseObject
{
public:
  // ��� �������, ����� ��� Prediction
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

  //char* pUserData;    // � ������� ������ TObjectDX*, � ������� ������ id
  //int   mUserDataSize;

  //unsigned short mID;// ���
  //unsigned char mType;// ��. eTypeObject

public:
  //=====================================
  //unsigned int mMaskState;// �������� ���� ������������� ������� - ���������� ��������� ���� ���������� mMaskState
  // 1 - ����������, 0 - �����������
  
  //nsStruct3D::TCoord3  mCoord; // �������� ���� ������������� �������
  //nsStruct3D::TOrient3 mOrient;// 
  //=====================================

  TBoundingBox mBBCommon;// ���������� ��������, ������ ������

  struct TDef
  {
    unsigned short HP;// ��� �� ��������� ���� ����� ������� ��� ��������� � ������ ��� ��������
    TBoundingBox   BB;// ����� ������ ����������� ��������
    unsigned int   mask;//
  };

  TDef* mArrDef;
  int   mCntDef;


public:
  
  IBaseObjectPrediction();
  virtual ~IBaseObjectPrediction();

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