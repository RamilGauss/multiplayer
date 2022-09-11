/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IManagerObjectCommonH
#define IManagerObjectCommonH

#include "SrcEvent.h"
#include <vector>

#define STR_NAME_MOC "MOC"

class IBaseObjectCommon;
class IMakerObjectCommon;

class IManagerObjectCommon : public TSrcEvent
{
protected:
  std::vector< IBaseObjectCommon* > mVectorObject;

public:

  // ������� ������ �������
  typedef enum
  {
    eBeginLoadMap = 0,
    eLoadingMap10,       // ������ 10% �� ��������� ��������
    eEndLoadMap,
    eStoppedLoadMap,
    //---------------------------
    eErrorLoadMap,
  }tEvent;

  IManagerObjectCommon();
  virtual ~IManagerObjectCommon();

  virtual void Init(IMakerObjectCommon* _pMakerObjectCommon) = 0;
  virtual void Clear() = 0;// ����� ��� �������
  virtual void Done() = 0; // ������ � ���������� ���������
  virtual void LoadMap(unsigned int id_map) = 0;// ��������� ����� � ������������ ������
  virtual int  GetProgressLoadMap() = 0;// �������� �������� �����
  virtual IBaseObjectCommon* CreateObject(unsigned int id_model) = 0;

  void AddObject(IBaseObjectCommon* pObject);// ������ �� ��������
  IBaseObjectCommon* Get(int index);// ������ ������ �� ��������� ������� (� �������� ��� �������)
  int GetCountObject();

};

#endif
