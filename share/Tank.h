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
    Engine          = 1>>1, // двигатель, поврежден
    EngineBreak     = 2>>1, // двигатель, не работает
    //------------------------------------------------
    Gun             = 3>>1, // пушка 
    GunBreak        = 4>>1, // пушка 
    //------------------------------------------------
    Tower           = 5>>1, // башня
    TowerBreak      = 6>>1, // башня
    //------------------------------------------------
    Chassis         = 7>>1, // шасси
    ChassisBreak    = 8>>1, // шасси
    //------------------------------------------------
    Optica          = 9>>1, // триплекс расстояния
    OpticaBreak     = 10>>1, // триплекс расстояния
    //------------------------------------------------
    LeftTrack       = 11>>1,
    LeftTrackBreak  = 12>>1,
    //------------------------------------------------
    RightTrack      = 13>>1,
    RightTrackBreak = 14>>1,
    //------------------------------------------------
    BK              = 15>>1,// бое укладка повреждена
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

  // для Room
  // в бою
  unsigned char mGroup;// номер команды в комнате. или 0 или 1.


  // характеристики
  
  // состояние
  unsigned short mHP;              // у.е.
  unsigned int maskDefectDevice;   // маска неисправности устройств,  0 - исправно, 1 - неисправно см. eMaskDefectDevice
  unsigned int maskDefectCrew;     // маска контузии экипажа,  0 - здоров, 1 - контужен см. eMaskDefectCrew

  // базовые
  float mMassa;                    //масса, кг
  float mGeometry[3];              //геометрия, параллелепипед ДхШхВ, м
  float mPower;                    //мощность двигателя, Вт
  float mSpeedRotate;              //мощность поворота, град/сек
  float mCentrRotate;              //центр поворота, смещение относительно центра танка, м 
  float mMaxSpeed;                 //максимальная скорость, км/ч
  float mDistView;                 //дальность обзора, м
  float mVisibilityStatic;         //видимость неподвижный
  float mVisibilityDinamic;        //          подвижный, коэф
  float mSpeedRotateTower;         // скорость вращения башни, град/сек
  float mSpeedReductionGun;
  float mRadiusRadio;              // радиус действия радио, м

  // описание поверхности: углы (град), толщина (мм)
  // ....

  // характеристики орудия
  unsigned short mCntCommonShell;   // общее кол-во снарядов
  unsigned short mCntBronShell;     // бронебойный
  unsigned short mCntExplosiveShell;// ОФ
  unsigned short mCntCumulShell;    // кумулятивный

  unsigned short mMaxCntShell;
  unsigned char mTypeCurShell;

  float mTimeReload;               // время перезарядки, сек
  float mSpeedReductionAim;        // скорость наведения

  struct TDefShell
  {
    float damage;              // урон, хп
    float depthShoot;          // пробиваемость, мм
    float mSpeedShell;               // скорость снаряда
  };

  // ББ
  TDefShell mShellBron;
  // ОФ
  TDefShell mShellExplosive;
  // КС
  TDefShell mShellCumul;

  float mProbablyFireEngine;       // вероятность загорания танка
  unsigned short mFireDamage; 
  float mProbablyExplosiveStackShell; // вероятность взрыва БК
  unsigned short mExplosiveStackShellDamage; 

  // есть два фактора: экипаж и комплектация танка
  // комплектация
  unsigned char mTower; // номер, 0 - базовая (сток)
  unsigned char mGun;
  unsigned char mChassis;
  unsigned char mEngine;
  unsigned char mRadio;

  // установленные модули
  unsigned int maskModule;

  // параметры перемещения
  float mCoordX;//координата центра танка
  float mCoordY;
  float mCoordZ;

  float mSpeed;

  float mCoordX_Vector;// вектор направления (ориентация)
  float mCoordY_Vector;
  float mCoordZ_Vector;

  // маска нажатых клавиш
  unsigned int mMaskPushButton; // см. ePushButton
  guint32 mRestReload;

};
//-----------------------------------------------------------------------------


#endif

