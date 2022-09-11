/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IMakerObjectCommonH
#define IMakerObjectCommonH

#include <map>

class IBaseObjectCommon;

class IMakerObjectCommon
{
  typedef std::map<unsigned int,unsigned int> TMapUintUint;
  TMapUintUint mMapID;

  bool flgNeedInit;
public:
  IMakerObjectCommon();
  virtual ~IMakerObjectCommon();

  IBaseObjectCommon* New(unsigned int id_model);

  // ������ ���������� ����������� ������: ��� ������� � ������, �� � ���������� ��� ��,
  // �.�. � ������������ ���������� ��� ������ ����������� ��������� �� ������ ���������� ��� ����������

  // In future must overload
  virtual void Delete(IBaseObjectCommon* pObject) = 0;// ������ ������� delete pObject;

protected:
  // In future must overload
  virtual IBaseObjectCommon* NewByID_Behavior(unsigned int id_behavior) = 0;

  unsigned int GetID_ModelByID_Behavior(unsigned int id_model);

  void Init();
};

#endif
