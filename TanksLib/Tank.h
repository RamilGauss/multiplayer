/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

  TTank();
  virtual ~TTank();

  // реакция на человеческую реакцию
  virtual void SetHuman(char* pData, int size){};

  void SetTypeTank(unsigned int id_tank);
  unsigned int GetTypeTank();

protected:
  unsigned int mID_tank;
public:

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
  unsigned char mTypeNextShell;

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

  guint32 mTimeRefreshOrientAim;

  // параметры перемещения
  //float mCoordX;//координата центра танка
  //float mCoordY;
  //float mCoordZ;

  float mSpeed;

  //float mCoordX_Vector;// вектор направления (ориентация)
  //float mCoordY_Vector;
  //float mCoordZ_Vector;

  // маска нажатых клавиш
  guint32 mTimeRefreshPushButton;// когда последний раз менялась маска
  unsigned int mMaskPushButton; // см. ePushButton
  guint32 mRestReload;

};
//-----------------------------------------------------------------------------


#endif

