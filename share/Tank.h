#ifndef __TankH
#define __TankH

#include "ServerStruct.h"
#include "TObject.h"

class TTank : public TObject
{

  unsigned short mID;
  nsServerStruct::TClient* pMasterClient;

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

  TRoom* pRoom;
  
  TTank(nsServerStruct::TClient* pClient);
  void LoadPropertyFromIni();

  void SetID(int type);
  int GetID();

  nsServerStruct::TClient* GetMasterClient();

  // ��� Room
  // � ���
  unsigned char mGroup;// ����� ������� � �������. ��� 0 ��� 1.


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

  // ��������� �����������
  float mCoordX;//���������� ������ �����
  float mCoordY;
  float mCoordZ;

  float mSpeed;

  float mCoordX_Vector;// ������ ����������� (����������)
  float mCoordY_Vector;
  float mCoordZ_Vector;

  // ����� ������� ������
  unsigned int mMaskPushButton; // ��. ePushButton
  guint32 mRestReload;

};
//-----------------------------------------------------------------------------


#endif

