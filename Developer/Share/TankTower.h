/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef TankTowerH
#define TankTowerH

#include "../GameLib/IActor.h"


class TTankTower : public IActor
{
  nsStruct3D::TMatrix16 mMatrixForCamera;

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
  TTankTower();
  virtual ~TTankTower();

  virtual bool GetMirror(char ** pData,int &size);
  virtual void SetMirror(char *pData,int size);

  virtual void SetHuman(char* pData, int size);

  virtual bool Animate(unsigned int time_ms);

  // from IBaseObject
  virtual const nsStruct3D::TMatrix16* GetMatrixForCamera();

public://protected:
  // debug only
  void RotateTurret(float ugol);
  void RotateVerticalGun(float ugol);

protected:
  virtual void SetupShaderStackModelGE();
  virtual void EventSetModelGE();

protected:
  int mIndexTrackR;
  int mIndexTrackL;
  int mIndexTime;
  int mIndexVelocity;
  std::vector<int> mShaderStackMask;

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

  unsigned int mTimeRefreshOrientAim;

  // ����� ������� ������
  unsigned int mTimeRefreshPushButton;// ����� ��������� ��� �������� �����
  unsigned int mMaskPushButton; // ��. ePushButton
  unsigned int mRestReload;

};


#endif