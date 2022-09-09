/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
  // ��������, ����������� ��� ������ � �������
  // ����������, ����������, ���������
public:
  TBaseObject();
  virtual ~TBaseObject();

  void SetWorld(D3DXMATRIXA16& world){mWorld=world;}
  void SetID_Model(unsigned int id);
  void SetID_Map(unsigned int id){ID_map = id;}

  unsigned int GetID_Model(){return ID_model;}
  unsigned int GetID_Map(){return ID_map;}
  std::vector<unsigned char>* GetState(){return &mState;}
  D3DXMATRIXA16 GetWorld(){return mWorld;}


  void SetTree(TTreeJoint::TLoadedJoint* pTree);
  void SetMapUse(std::map<std::string,int>* mapUse = NULL);// ������������ ��� ����� ������������ ������ � ��� (����������) ��������� �������
  void SetState(std::vector<unsigned char>* state);
  
  void GetDefaultMapUse(std::map<std::string,int>* mapUse);
  
  void* GetPtrInherits(){return mPtrInherits;}

protected:
  TTreeJoint::TLoadedJoint* pLoadedTree;
  TTreeJoint mTree;

  unsigned int ID_map;  // ������������� �� �����
  unsigned int ID_model;// ������������� ������

  D3DXMATRIXA16 mWorld; // ����� ��� ���� �� ���������� � ����������� �������

  std::vector<unsigned char> mState;// �������, ������� ������ ����� �� ������ ��������, ��� ������ ���������� ��������� �������
  std::vector<unsigned char> mMask; // � ������ � ������� ����� ����� �������� � ������������
  std::vector<D3DXMATRIXA16*> mVectorMatrix;// � ������ � �������, cnt=cntAllJoint

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
  std::map<std::string, int>  mMapUse;// cnt=cntAllPartModel

  // ��������� ������� ������������ � ���������� ��������� ������� ������
  void SetDefaultMatrix();//### �����������
  void SetupState();
  void SetupMask();

  void SetupDefaultMapUse();

  //virtual std::vector<std::string>* GetOrderPart(){return &mVectorOrderPart;};

  // ����� ��������� ������ ������
  // ��������, ���������� �����1, � �� �����0 � �.�.
  // 1 0 0 1 1 1 1


  void* mPtrInherits;// ��������� � TBaseObjectCommon (������� �������� ������������ ������������)

  int GetCountPart(const char* name, std::vector<std::string>* pVec);
};


#endif

