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


#ifndef BaseObjectDXH
#define BaseObjectDXH

#include "IBaseObject.h"
#include <vector>

class IModelGE;

class IBaseObjectGE : virtual public IBaseObject
{
protected:

  bool flgShow;// ������� �� ������ �� �����
  float mAlphaTransparency;// ������������ ����� �������, ������ ������ ������������ ����� ������
  
public:
  IBaseObjectGE(int typeDX = eDirtyAnimate);
  virtual ~IBaseObjectGE();

  void SetModel(IModelGE* pModel);
  IModelGE* GetModel();

  void Draw(const nsStruct3D::TMatrix16* mView, void* pEffect = NULL);

  void SetShow(bool show){flgShow=show;}
  bool IsShow(){return flgShow;}

  // ������ true - ������ ���, false - �������� ����
  // ������ ���� ������ ����� ������ �����, ��. IBaseObject::mTimeCreation - ����� �����
  // ������ ��� ��������� ������������� ��������!!!
  // ������ ������� �������������� ������ ��� ����� ������� ��� ������� ������� �������
  // ������������� ���������� ����� ������ ���� � ������ SetupShaderStackModelGE
  virtual bool Animate(unsigned int time_ms/*dirty animate ignore this parameter*/) = 0;//{return true;};

  enum{eDirtyAnimate = 0,
       ePureAnimate  = 1,// ��. mTypeGE
  };
  int GetTypeGE(){return mTypeGE;}
  void SetTimeCreation(unsigned int t){mTimeCreation=t;};

  float GetAlphaTransparency(){return mAlphaTransparency;}
  void  SetAlphaTransparency(float val){mAlphaTransparency = val;}

	int GetCountPartForCubeMap();

	void* GetTextureForCubeMap(int i);

  // ���� ���, � ������� ������ ���� const char* GetPostEffect();
  virtual bool IsGlowEffect(){return false;};
  virtual bool IsGlowable(){return false;};

protected:
  IModelGE* mModel;// ������� ��� 

  int mTypeGE;

  // ����� ��������, ���������� ��� ������� ��������
  unsigned int mTimeCreation;// ��

	std::vector<void*> mVectorUseCubeMap;//������ IDirect3DCubeTexture9**
protected:

  void Done();

  // ����� �� ������� ������ ���� � � Prediction
  void SetupVectorNamePart();
  void SetupVectorOrderPart();
	void SetupVectorForCubeMap();

  void SetShaderStackMask(std::vector<int>* shaderStackMask);// ��������� �����
  void SetupShaderStack(int indexSS, int index, void* data,int size);
  int  GetShaderStackIndexByName(int index, const char* nameParam);
  
protected:
  virtual void SetupShaderStackModelGE(){};// ��� ����� ������ ��������� �������
  virtual void EventSetModelGE(){};
};
//-----------------------------------------------------------------



#endif