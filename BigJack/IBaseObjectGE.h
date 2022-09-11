/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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