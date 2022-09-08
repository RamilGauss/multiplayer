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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BaseObjectDXH
#define BaseObjectDXH

#include "ModelDX.h"
#include "BaseObjectPrediction.h"

class TManagerObjectDX;

class TBaseObjectDX : public TBaseObjectPrediction
{
protected:

  bool flgShow;// ������� �� ������ �� �����
  

public:
  TBaseObjectDX();
  virtual ~TBaseObjectDX();

  void SetModel(TModelDX* pModel);
  TModelDX* GetModel();

  void Draw(D3DXMATRIXA16* mView,D3DXMATRIXA16* mProj);

  void SetShow(bool show){flgShow=show;}

protected:

  void Done();

  TModelDX* mModel;// ������� ��� 
  
  D3DXMATRIXA16* mArrMatrix;// ���-�� ��. � mModel
  int mCntMatrix;
  std::vector<unsigned char> mMask;

  // ��������� ������� ������������ � ���������� ��������� ������� ������
  virtual void SetupArrMatrix();
  virtual void SetupState();
  virtual void SetupMask();

  // ����� ��������� ������ ������
  // ��������, ���������� �����1, � �� �����0 � �.�.
  // 1 0 0 1 1 1 1
};
//-----------------------------------------------------------------



#endif