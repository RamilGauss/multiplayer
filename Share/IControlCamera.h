/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IControlCameraH
#define IControlCameraH

#include "ICamera.h"

class IBaseObject;

/*
  ��������� ������� ��������� � ����������
  1. Linked object (�� �����)
  2. Orient
  3. Set/Add XXX
*/

class SHARE_EI IControlCamera : public ICamera
{

public:

  typedef enum
  {
    eNotLinked, 
    eCoord,
    eOrientAndCoord,
  }TypeLinked;


  IControlCamera(){}
  virtual ~IControlCamera(){}

  virtual IBaseObject* GetLinkedObject() = 0;
  // ���� ��������� �� ������ ����������, �� ����� � ������� ���������, ������� ������� � �����
  // ������ SetXXX ����� SetXXX, ������� ����� �������. ����� ������ AddXXX, ������� ����� �������.
  virtual void Link(IBaseObject* pObject, TypeLinked maskUse ) = 0;
  virtual void Unlink() = 0;

  //virtual ICamera* GetCamera() = 0;// debug only

  // �������� �� eye ������. ��������, �� 3 ���� dist > 0
  // �� 1 ���� ���������� ����� ����
  // ������ �� �������� ������ (������ ����� ��������)
  virtual float GetDistObj()             = 0;
  virtual void  SetDistObj(float radius) = 0;
  virtual void  AddDistObj(float dR)     = 0;

  // ��� ��������� ������, �.�./���
  virtual void SetSpeedForward(float v) = 0;
  virtual void SetSpeedRight(float v) = 0;
  virtual void SetSpeedUp(float v) = 0;
};

#endif