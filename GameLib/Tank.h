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


#ifndef TankH
#define TankH

#include "BaseObjectCommon.h"
#include <string>

class TClient;
class TRoom;

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
public:
  TTank();
  virtual ~TTank();
//--------------------------------------------------------------------------------
  // �� TBaseObjectCommon
  virtual bool GetMirror(char** pData, int& size){return false;}
  virtual void SetMirror(char* pData, int size){}

  virtual int GetSizeProperty();
  virtual char* GetProperty();
  virtual void SetProperty(char* pData,int size);
//--------------------------------------------------------------------------------
  // TTank ���������
  void SetTypeTank(unsigned int id_tank);
  unsigned int GetTypeTank();
//--------------------------------------------------------------------------------
public:
  // ��������� ��� �������
  TRoom* pRoom;

  void SetMasterClient(TClient* _pClient);
  TClient* GetMasterClient();

  // ��� Room, � ���
  unsigned char mGroup;// ����� ������� � �������. ��� 0 ��� 1.

protected:
  TClient* pMasterClient;
//--------------------------------------------------------------------------------
  // ��� �������
  std::string sClientName;
public:
  const char* GetClientName(){return sClientName.data();}
  void        SetClientName(char* str){sClientName.insert(0,str);}
//--------------------------------------------------------------------------------

public:

  #pragma pack(push, 1)
  struct TProperty
  {
    unsigned int mID_tank;
    // �������
    float mMassa;                    //�����, ��
    float mPower;                    //�������� ���������, ��
    float mSpeedRotate;              //�������� ��������, ����/���
    float mDistView;                 //��������� ������, �
    float mVisibilityStatic;         //��������� �����������
    float mVisibilityDinamic;        //          ���������, ����
    float mSpeedRotateTower;         // �������� �������� �����, ���/���
    float mSpeedReductionGun;        // �������� �������� �����, ���/�
    // Property, ������� �� ��������
    float mMaxSpeedForward;          //������������ �������� ������, �/�
    float mMaxSpeedBackward;         //������������ �������� �����, �/�
    float mRadiusRadio;              // ������ �������� �����, � - ������� �� ����� � �������

    // ������������ ���� ������������ ������� � �����
    float mVMaxGunUgol;  // �����, ���
    float mVMinGunUgol;  // ����, ���
    // ������������ ���� ������������ ������� � �����
    float mHMaxGunUgol;  // �� ������� �������, ���
    float mHMinGunUgol;  // ������ ������� �������, ���
  };
  #pragma pack(pop)

  struct TDefShell
  {
    float damage;              // ����, ��
    float depthShoot;          // �������������, ��
    float mSpeedShell;               // �������� �������
  };

  // ��������������
  // �������� � ���

  unsigned int maskDefectDevice;   // ����� ������������� ���������,  0 - ��������, 1 - ���������� ��. eMaskDefectDevice
  unsigned int maskDefectCrew;     // ����� �������� �������,  0 - ������, 1 - �������� ��. eMaskDefectCrew


  // ���������

  // ��
  TDefShell mShellBron; // ������� �� �����
  // ��
  TDefShell mShellExplosive; // ������� �� �����
  // ��
  TDefShell mShellCumul; // ������� �� �����

  float mProbablyFireEngine;       // ����������� ��������� �����, ������� �� ���������
  unsigned short mFireDamage; 
  float mProbablyExplosiveStackShell; // ����������� ������ ��
  unsigned short mExplosiveStackShellDamage; 

  unsigned short mHP;              // �.�. ������� �� ���� ����� � ���� �����

  float mCentrRotate;              //����� ��������, �������� ������������ ������ �����, �  �� ���� ����� � ���� �����

  // Property, ������� ��������
  TProperty mProperty;

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

  // ����� ������� ������
  guint32 mTimeRefreshPushButton;// ����� ��������� ��� �������� �����
  unsigned int mMaskPushButton; // ��. ePushButton
  guint32 mRestReload;

};
//-----------------------------------------------------------------------------


#endif

