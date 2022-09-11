/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BaseObjectH
#define BaseObjectH

#include <map>
#include <vector>

//#include "TObject.h"
#include "Struct3D.h"
#include "TreeJoint.h"
#include "CallBackRegistrator.h"

// ������� ����� �������� �����, ������

class SHARE_EI IBaseObject// : public TObject Gauss comment 09.07.2013
{
protected:
  TCallBackRegistrator1<int> mCallBackEvent;
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
  template<typename F, class A1>
  void RegisterOnEvent(F pFunc, A1 pObject){mCallBackEvent.Register(pFunc,pObject);}
  template<typename F, class A1>
  void UnregisterOnEvent(F pFunc, A1 pObject){mCallBackEvent.Unregister(pFunc,pObject);}

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
  void Notify(int event){mCallBackEvent.Notify(event);}
};


#endif

