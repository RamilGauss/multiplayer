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

#ifndef ManagerObjectCommonClientH
#define ManagerObjectCommonClientH


#include "NET_LevelClientServer.h"
#include "ManagerObjectCommon.h"
#include "ApplicationProtocolPacketAnswer.h"

// сердце игры (клиента), тут все правила

class TManagerObjectCommonClient : public TManagerObjectCommon
{
  enum{
    eTimeoutFreshData=10, // мс
  };


  TA_In_Fight mPacketInFight;
  TA_Correct_Packet_State_Tank   mPacketCorrectStateTank;
  TA_Correct_Packet_State_Object mPacketCorrectStateObject;

public:

  TManagerObjectCommonClient();
  virtual ~TManagerObjectCommonClient();

  
  virtual void Work();
  virtual void Translate(unsigned short type, char*pData, int size);

protected:
  //void PrepareTank(TTankTower* pTank, int i);
  //// отослать через какой-то транспорт запрос на получение корректирующего пакет
  //// отсылаетс€ после загрузки карты
  //void SendCorrectPacket(){};
  //void ApplPacketA_Correct_Packet_State_Tank();

  //void AddTankInCommonList();

  //void LoadMapEndEvent();

  //void ApplPacketA_In_Fight(char* pData, int size);

  virtual float GetTimeWork();

};

#endif
