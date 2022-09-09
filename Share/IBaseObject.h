/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#ifndef BaseObjectH
#define BaseObjectH

#include <map>
#include <vector>

#include "TObject.h"
#include "Struct3D.h"
#include "TreeJoint.h"
#include "CallBackRegistrator.h"

// ������� ����� �������� �����, ������

class SHARE_EI IBaseObject : public TObject
{
protected:
  TCallBackRegistrator mCallBackEvent;
  // ��������, ����������� ��� ������ � �������
  // ����������, ����������, ���������
public:
  enum{
    eWorld = 0, // �������� ������� �������
    eState,
    eMapUse,
  };

  IBaseObject();
  virtual ~IBaseObject();

  void SetWorld(nsStruct3D::TMatrix16* world);
  const nsStruct3D::TMatrix16* GetWorld()const {return &mWorld;}

  void SetID_Model(unsigned int id);
  void SetID_Map(unsigned int id){ID_map = id;}

  unsigned int GetID_Model(){return ID_model;}
  unsigned int GetID_Map(){return ID_map;}
  std::vector<unsigned char>* GetState(){return &mState;}

  virtual const nsStruct3D::TMatrix16* GetMatrixForCamera(){return GetWorld();}

  void SetVelocity(float v){mV=v;}// �/� - ������ ��������
  float GetVelocity(){return mV;}

  void SetTree(TTreeJoint::TLoadedJoint* pTree);
  void SetMapUse(std::map<std::string,int>* mapUse = NULL);// ������������ ��� ����� ������������ ������ � ��� (����������) ��������� �������
  void SetState(std::vector<unsigned char>* state);
  
  void GetDefaultMapUse(std::map<std::string,int>* mapUse);
  
  void* GetPtrInherits(){return mPtrInherits;}

  // ����������� �� ��������� ������� �������
  void RegisterOnEvent(TCallBackRegistrator::TCallBackFunc pFunc);
  void UnregisterOnEvent(TCallBackRegistrator::TCallBackFunc pFunc);

protected:
  
  float mV;// ��������, �/�
  float mA;// ���������, �/�^2

  TTreeJoint::TLoadedJoint* pLoadedTree;
  TTreeJoint mTree;

  unsigned int ID_map;  // ������������� �� �����
  unsigned int ID_model;// ������������� ������

  nsStruct3D::TMatrix16 mWorld; // ����� ��� ���� �� ���������� � ����������� �������

  std::vector<unsigned char> mState;// �������, ������� ������ ����� �� ������ ��������, ��� ������ ���������� ��������� �������
  std::vector<unsigned char> mMask; // � ������ � ������� ����� ����� �������� � ������������
  std::vector<nsStruct3D::TMatrix16*> mVectorMatrix;// � ������ � �������, cnt=cntAllJoint

  // size = sizeAllPart
  struct TPart 
  {
    std::string name;
    int use;
  };
  std::vector<TPart> mVectorNamePart;// ����������� ����� ������ �������. ����������� ������ ������ ����� ���������.
  // ���� ������ ������������ ��� ���������� ����� ������
  //size = sizeAllJoint
  std::vector<std::string> mVectorOrderPart;// ������� �������� ������, ��� ����������, cnt=cntJoint
  std::map<std::string, int>  mMapUse;// 

  // ��������� ������� ������������ � ���������� ��������� ������� ������
  void SetDefaultMatrix();//### �����������
  void SetupState();
  void SetupMask();

  void SetupDefaultMapUse();

  // ����� ��������� ������ ������
  // ��������, ���������� �����1, � �� �����0 � �.�.
  // 1 0 0 1 1 1 1


  void* mPtrInherits;// ��������� � IBaseObjectCommon (������� �������� ������������ ������������)

  int GetCountPart(const char* name, std::vector<std::string>* pVec);

protected:
  void Notify(int event);
};


#endif

