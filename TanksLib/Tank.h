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


#ifndef TankH
#define TankH

#include "BaseObjectCommon.h"

//------------------------------------------------------------------------------
class TTank : public TBaseObjectCommon
{

public:
  typedef enum{
    Engine          = 1>>1, // ���������, ���������
    EngineBreak     = 2>>1, // ���������, �� ��������
    //------------------------------------------------
    Gun             = 3>>1, // ����� 
    GunBreak        = 4>>1, // ����� 
    //------------------------------------------------
    Tower           = 5>>1, // �����
    TowerBreak      = 6>>1, // �����
    //------------------------------------------------
    Chassis         = 7>>1, // �����
    ChassisBreak    = 8>>1, // �����
    //------------------------------------------------
    Optica          = 9>>1, // �������� ����������
    OpticaBreak     = 10>>1, // �������� ����������
    //------------------------------------------------
    LeftTrack       = 11>>1,
    LeftTrackBreak  = 12>>1,
    //------------------------------------------------
    RightTrack      = 13>>1,
    RightTrackBreak = 14>>1,
    //------------------------------------------------
    BK              = 15>>1,// ��� ������� ����������
  }eMaskDefectDevice;

  typedef enum{
    e1 = 1>>1, 
    e2 = 2>>1, 
    e3 = 3>>1, 
    e4 = 4>>1, 
    e5 = 5>>1, 
    e6 = 6>>1, 
    e7 = 7>>1, 
    e8 = 8>>1, 
  }eMaskDefectCrew;
  //----------------------------------------
  
  typedef enum{
    eLeft=0,
    eRight,
    eBack,
    eForward,
  }ePushButton;

  TTank();
  virtual ~TTank();

  // ������� �� ������������ �������
  virtual void SetHuman(char* pData, int size){};

  void SetTypeTank(unsigned int id_tank);
  unsigned int GetTypeTank();

protected:
  unsigned int mID_tank;
public:

  // ��������������
  
  // ���������
  unsigned short mHP;              // �.�.

  unsigned int maskDefectDevice;   // ����� ������������� ���������,  0 - ��������, 1 - ���������� ��. eMaskDefectDevice
  unsigned int maskDefectCrew;     // ����� �������� �������,  0 - ������, 1 - �������� ��. eMaskDefectCrew

  // �������
  float mMassa;                    //�����, ��
  float mGeometry[3];              //���������, �������������� �����, �
  float mPower;                    //�������� ���������, ��
  float mSpeedRotate;              //�������� ��������, ����/���
  float mCentrRotate;              //����� ��������, �������� ������������ ������ �����, � 
  float mMaxSpeed;                 //������������ ��������, ��/�
  float mDistView;                 //��������� ������, �
  float mVisibilityStatic;         //��������� �����������
  float mVisibilityDinamic;        //          ���������, ����
  float mSpeedRotateTower;         // �������� �������� �����, ����/���
  float mSpeedReductionGun;
  float mRadiusRadio;              // ������ �������� �����, �

  // �������� �����������: ���� (����), ������� (��)
  // ....

  // �������������� ������
  unsigned short mCntCommonShell;   // ����� ���-�� ��������
  unsigned short mCntBronShell;     // �����������
  unsigned short mCntExplosiveShell;// ��
  unsigned short mCntCumulShell;    // ������������

  unsigned short mMaxCntShell;
  unsigned char mTypeCurShell;
  unsigned char mTypeNextShell;

  float mTimeReload;               // ����� �����������, ���
  float mSpeedReductionAim;        // �������� ���������

  struct TDefShell
  {
    float damage;              // ����, ��
    float depthShoot;          // �������������, ��
    float mSpeedShell;               // �������� �������
  };

  // ��
  TDefShell mShellBron;
  // ��
  TDefShell mShellExplosive;
  // ��
  TDefShell mShellCumul;

  float mProbablyFireEngine;       // ����������� ��������� �����
  unsigned short mFireDamage; 
  float mProbablyExplosiveStackShell; // ����������� ������ ��
  unsigned short mExplosiveStackShellDamage; 

  // ���� ��� �������: ������ � ������������ �����
  // ������������
  unsigned char mTower; // �����, 0 - ������� (����)
  unsigned char mGun;
  unsigned char mChassis;
  unsigned char mEngine;
  unsigned char mRadio;

  // ������������� ������
  unsigned int maskModule;

  guint32 mTimeRefreshOrientAim;

  // ��������� �����������
  //float mCoordX;//���������� ������ �����
  //float mCoordY;
  //float mCoordZ;

  float mSpeed;

  //float mCoordX_Vector;// ������ ����������� (����������)
  //float mCoordY_Vector;
  //float mCoordZ_Vector;

  // ����� ������� ������
  guint32 mTimeRefreshPushButton;// ����� ��������� ��� �������� �����
  unsigned int mMaskPushButton; // ��. ePushButton
  guint32 mRestReload;

};
//-----------------------------------------------------------------------------


#endif

