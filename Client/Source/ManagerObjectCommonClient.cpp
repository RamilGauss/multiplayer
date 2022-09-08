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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ManagerObjectCommonClient.h"


//-------------------------------------------------------------------
TManagerObjectCommonClient::TManagerObjectCommonClient()
:TManagerObjectCommon()
{

}
//-------------------------------------------------------------------
TManagerObjectCommonClient::~TManagerObjectCommonClient()
{

}
//-------------------------------------------------------------------
//void TGameForm::PrepareTank(TTankTower* pTank, int i)
//{
//  char* pData = (char*)mPacketInFight.getPointerTankProperty(i);
//  int size = mPacketInFight.getSizeTankProperty(i);
//  pTank->SetProperty(pData,size);
//}
////--------------------------------------------------------------------
//void TGameForm::AddTankInCommonList()
//{
//  TMakerBehavior mMaker;
//  int cntTank = mPacketInFight.getCountTank();
//  for(int i = 0 ; i < cntTank ; i++ )
//  {
//    // 
//    TBaseObjectCommon* pObject = mMaker.New(ID_TANK_TOWER);
//    PrepareTank((TTankTower*)pObject,i);
//
//    pGame->AddObject(pObject);
//  }
//}
////--------------------------------------------------------------------
//void TGameForm::LoadMapEndEvent()
//{
//  AddTankInCommonList();
//  SetWaitTrue();
//  pClient->SendRequestCorrectPacket();// отсылка запроса
//}
////--------------------------------------------------------------------
//void TGameForm::ApplPacketA_In_Fight(char* pData, int size)
//{
//  GlobalLoggerForm.WriteF_time("ѕолучен пакет вход в бой.\n");
//  // создать поток загрузки карты
//  // когда загрузка закончитс€ отослать запрос на корректирующий пакет
//
//  // к сожалению поток загрузки карты прервать нельз€,
//  // ждите
//  mPacketInFight.setData(pData,size);
//  pGame->LoadMap(mPacketInFight.getCodeMap());
//}
////--------------------------------------------------------------------
//void TGameForm::ApplPacketA_Correct_Packet_State_Tank()
//{
//  GlobalLoggerForm.WriteF_time("ѕолучен корректирующий пакет, состо€ние танков.\n");
//  if(IsWaitCorrectPacketTank()==false) return;
//  // применить содержимое пакета
//
//  pGame->EndLoadMap();// окончание загрузки
//
//  SetWaitFalse();
//}
//--------------------------------------------------------------------
void TManagerObjectCommonClient::Work()
{
  guint32 iTime = ht_GetMSCount();
  if(IsLoadMap())
  {
    mProgressBar.VisualEvent(mProcentLoadMap);
  }
  else
  {
    if(iTime>mLastTimeFreshData+eTimeoutFreshData)
    {
      mPrediction.Calc();
      mLastTimeFreshData = ht_GetMSCount();
    }

    mMDX_Scene.Work();
  }
}
//--------------------------------------------------------------------
void TManagerObjectCommonClient::Translate(unsigned short type, char*pData, int size)
{
  /*switch(type)
  {
    case APPL_TYPE_A_IN_FIGHT:
    {
      ApplPacketA_In_Fight(pData,size);
      break;
    }
    case APPL_TYPE_G_A_CORRECT_PACKET_STATE_TANK:
    {
      ApplPacketA_Correct_Packet_State_Tank();
      break;
    }
    case APPL_TYPE_G_S_LOAD_MAP:
    {
      break;
    }
    case APPL_TYPE_G_S_COUNT_DOWN:
    {
      // отображать отсчет на 
      break;
    }
    case APPL_TYPE_G_S_FIGHT_COORD_BULLET:
    {
      //pGame->RefreshFromServer();
      break;
    }
    default:;
  }*/
}
//---------------------------------------------------------------------------------------------
