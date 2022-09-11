/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CameraH
#define CameraH

#include "ICamera.h"
#include <list>

// Not Thread Safe!

class TCamera : public ICamera
{
  bool flgUseOrient;
  nsStruct3D::TVector3 mNormal2Earth;

  nsStruct3D::TMatrix16 mView;
  nsStruct3D::TMatrix16 mProj;

  nsStruct3D::TVector3  mPosition;
  nsStruct3D::TVector3  mLookAt;
  nsStruct3D::TVector3  mRight;
  nsStruct3D::TVector3  mUp;

  bool mChangedView;

  float mRotateUp;
  float mRotateRight;
  float mRotateLookAt;

  // ��� ������� ��������
  float mfFOV;
  float mfAspect;
  float mfNearPlane;
  float mfFarPlane;

  const float defFOV;
  const float defAspect;
  const float defNearPlan;
  const float defFarPlan;
  const float defDotProductMin;
public:
  TCamera();
  virtual ~TCamera();

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
  //-------------------------------------------
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
  void UpdateView();

  void InitVector();

  void NormalByEarth();
};

#endif
