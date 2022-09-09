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


#ifndef BaseObjectDXH
#define BaseObjectDXH

#include "IBaseObject.h"
#include <vector>
#include "glibconfig.h"

class TModelDX;

class IBaseObjectGE : virtual public IBaseObject
{
protected:

  bool flgShow;// ������� �� ������ �� �����
  float mAlphaTransparency;// ������������ ����� �������, ������ ������ ������������ ����� ������
  
public:
  IBaseObjectGE(int typeDX = eDirtyAnimate);
  virtual ~IBaseObjectGE();

  void SetModel(TModelDX* pModel);
  TModelDX* GetModel();

  void Draw(D3DXMATRIXA16* mView);

  void SetShow(bool show){flgShow=show;}

  // ������ true - ������ ���, false - �������� ����
  // ������ ���� ������ ����� ������ �����, ��. IBaseObject::mTimeCreation - ����� �����
  // ������ ��� ��������� ������������� ��������!!!
  // ������ ������� �������������� ������ ��� ����� ������� ��� ������� ������� �������
  // ������������� ���������� ����� ������ ���� � ������ SetupShaderStackModelGE
  virtual bool Animate(guint32 time_ms/*dirty animate ignore this parameter*/) = 0;//{return true;};

  enum{eDirtyAnimate = 0,
       ePureAnimate  = 1,// ��. mTypeGE
  };
  int GetTypeGE(){return mTypeGE;}
  void SetTimeCreation(guint32 t){mTimeCreation=t;};

  float GetAlphaTransparency(){return mAlphaTransparency;}
  void  SetAlphaTransparency(float val){mAlphaTransparency = val;}

protected:
  TModelDX* mModel;// ������� ��� 

  int mTypeGE;

  // ����� ��������, ���������� ��� ������� ��������
  guint32 mTimeCreation;// ��
protected:

  void Done();

  // ����� �� ������� ������ ���� � � Prediction
  void SetupVectorNamePart();
  void SetupVectorOrderPart();

  void SetShaderStackMask(std::vector<int>* shaderStackMask);// ��������� �����
  void SetupShaderStack(int indexSS, int index, void* data,int size);
  int  GetShaderStackIndexByName(int index, const char* nameParam);
  
protected:
  virtual void SetupShaderStackModelGE(){};// ��� ����� ������ ��������� �������
  virtual void EventSetModelGE(){};
};
//-----------------------------------------------------------------



#endif