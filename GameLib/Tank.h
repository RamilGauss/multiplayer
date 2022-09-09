/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
    Engine          = 1>>1, // двигатель, поврежден
    EngineBreak     = 2>>1, // двигатель, не работает
    //------------------------------------------------
    Gun             = 3>>1, // пушка 
    GunBreak        = 4>>1, // пушка 
    //------------------------------------------------
    Tower           = 5>>1, // башн€
    TowerBreak      = 6>>1, // башн€
    //------------------------------------------------
    Chassis         = 7>>1, // шасси
    ChassisBreak    = 8>>1, // шасси
    //------------------------------------------------
    Optica          = 9>>1, // триплекс рассто€ни€
    OpticaBreak     = 10>>1, // триплекс рассто€ни€
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
public:
  TTank();
  virtual ~TTank();
//--------------------------------------------------------------------------------
  // от TBaseObjectCommon
  virtual bool GetMirror(char** pData, int& size){return false;}
  virtual void SetMirror(char* pData, int size){}

  virtual int GetSizeProperty();
  virtual char* GetProperty();
  virtual void SetProperty(char* pData,int size);
//--------------------------------------------------------------------------------
  // TTank интерфейс
  void SetTypeTank(unsigned int id_tank);
  unsigned int GetTypeTank();
//--------------------------------------------------------------------------------
public:
  // интерфейс дл€ сервера
  TRoom* pRoom;

  void SetMasterClient(TClient* _pClient);
  TClient* GetMasterClient();

  // дл€ Room, в бою
  unsigned char mGroup;// номер команды в комнате. или 0 или 1.

protected:
  TClient* pMasterClient;
//--------------------------------------------------------------------------------
  // дл€  лиента
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
    // базовые
    float mMassa;                    //масса, кг
    float mPower;                    //мощность двигател€, ¬т
    float mSpeedRotate;              //мощность поворота, град/сек
    float mDistView;                 //дальность обзора, м
    float mVisibilityStatic;         //видимость неподвижный
    float mVisibilityDinamic;        //          подвижный, коэф
    float mSpeedRotateTower;         // скорость вращени€ башни, рад/сек
    float mSpeedReductionGun;        // скорость движени€ пушки, рад/с
    // Property, которые не мен€ютс€
    float mMaxSpeedForward;          //максимальна€ скорость вперед, м/с
    float mMaxSpeedBackward;         //максимальна€ скорость назад, м/с
    float mRadiusRadio;              // радиус действи€ радио, м - зависит от рации и радиста

    // максимальный угол вертикальной наводки у пушки
    float mVMaxGunUgol;  // вверх, рад
    float mVMinGunUgol;  // вниз, рад
    // максимальный угол вертикальной наводки у пушки
    float mHMaxGunUgol;  // по часовой стрелке, рад
    float mHMinGunUgol;  // против часовой стрелке, рад
  };
  #pragma pack(pop)

  struct TDefShell
  {
    float damage;              // урон, хп
    float depthShoot;          // пробиваемость, мм
    float mSpeedShell;               // скорость снар€да
  };

  // характеристики
  // свойства в бою

  unsigned int maskDefectDevice;   // маска неисправности устройств,  0 - исправно, 1 - неисправно см. eMaskDefectDevice
  unsigned int maskDefectCrew;     // маска контузии экипажа,  0 - здоров, 1 - контужен см. eMaskDefectCrew


  // состо€ние

  // ЅЅ
  TDefShell mShellBron; // зависит от пушки
  // ќ‘
  TDefShell mShellExplosive; // зависит от пушки
  //  —
  TDefShell mShellCumul; // зависит от пушки

  float mProbablyFireEngine;       // веро€тность загорани€ танка, зависит от двигател€
  unsigned short mFireDamage; 
  float mProbablyExplosiveStackShell; // веро€тность взрыва Ѕ 
  unsigned short mExplosiveStackShellDamage; 

  unsigned short mHP;              // у.е. зависит от типа танка и типа башни

  float mCentrRotate;              //центр поворота, смещение относительно центра танка, м  от типа танка и типа шасси

  // Property, которые мен€ютс€
  TProperty mProperty;

  // характеристики оруди€
  unsigned short mCntCommonShell;   // общее кол-во снар€дов
  unsigned short mCntBronShell;     // бронебойный
  unsigned short mCntExplosiveShell;// ќ‘
  unsigned short mCntCumulShell;    // кумул€тивный

  unsigned short mMaxCntShell;
  unsigned char mTypeCurShell;
  unsigned char mTypeNextShell;

  float mTimeReload;               // врем€ перезар€дки, сек
  float mSpeedReductionAim;        // скорость наведени€


  // есть два фактора: экипаж и комплектаци€ танка
  // комплектаци€
  unsigned char mTower; // номер, 0 - базова€ (сток)
  unsigned char mGun;
  unsigned char mChassis;
  unsigned char mEngine;
  unsigned char mRadio;

  // установленные модули
  unsigned int maskModule;

  guint32 mTimeRefreshOrientAim;

  // маска нажатых клавиш
  guint32 mTimeRefreshPushButton;// когда последний раз мен€лась маска
  unsigned int mMaskPushButton; // см. ePushButton
  guint32 mRestReload;

};
//-----------------------------------------------------------------------------


#endif

