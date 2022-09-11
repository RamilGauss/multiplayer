/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ControlCameraH
#define ControlCameraH

#include "IControlCamera.h"

// Not Thread Safe!

class TControlCamera : public IControlCamera
{
  ICamera* mCamera;

  IBaseObject* mLinkObj;

  float mDist;
  
  TypeLinked mMask;

  // ��� ��������� ��������
  nsStruct3D::TVector3 mSpeedShift;// x - forward, y - right, z - up
  unsigned int         mLastTime;// ����� ���������, ��

public:
  TControlCamera();
  virtual ~TControlCamera();

  virtual IBaseObject* GetLinkedObject();
  virtual void Link(IBaseObject* pObject, TypeLinked mask);
  virtual void Unlink();

  //virtual ICamera* GetCamera();

  // �������� �� eye ������. ��������, �� 3 ���� ��� ���������� ����� ������ 0
  // �� 1 ���� ���������� ����� ����
  // ������ �� �������� ������ (������ ����� ��������)
  virtual float GetDistObj();
  virtual void  SetDistObj(float v);
  virtual void  AddDistObj(float dV);

  // ��� ��������� ������
  virtual void SetSpeedForward(float v);
  virtual void SetSpeedRight(float v);
  virtual void SetSpeedUp(float v);
  //---------------------------------------------------
  // ��������� ������
  virtual void Update();
  // ������ ��������� �����������
  virtual const nsStruct3D::TMatrix16* GetView();
  virtual const nsStruct3D::TMatrix16* GetProj();
  virtual const nsStruct3D::TVector3*  GetEyePt();

  // ������� ���������
  virtual void SetView(nsStruct3D::TMatrix16* view);
  virtual void SetProj(nsStruct3D::TMatrix16* proj);
  virtual void SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane );
  virtual void SetOrient(nsStruct3D::TVector3* up, bool use = true);

  // ���������
  virtual void SetPositionLookAt(nsStruct3D::TVector3* pPosLookAt);
  virtual void SetPosition(nsStruct3D::TVector3* pPos);
  virtual void MoveInDirection(float dist, nsStruct3D::TVector3* pDir);
  virtual void MoveForward(float dist);// ����� ���� ������
  virtual void MoveRight(float dist);
  virtual void MoveUp(float dist);

  // �������� ��������
  virtual void ClearRotate();
  // ������� 
  virtual void RotateDown(float angle);
  virtual void RotateRight(float angle);
  virtual void Roll(float angle);

  virtual void SetDir(nsStruct3D::TVector3* right, nsStruct3D::TVector3* up, nsStruct3D::TVector3* lookat);

protected:
  void ApplySpeed();
  void InitLastTime();
  void GetFromLinkObj();
};

#endif