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

#ifndef ClientH
#define ClientH

#include "TObject.h"
#include "GlobalParamsTank.h"
#include "Garage.h"
class TRoom;

class TClient : public TObject
{
  TRoom*  			 pCurRoom; // либо что-то либо NULL
public:
  enum{eMaxLenNick=250,};

  enum{eGarage=0,eWait=1,eFight=2};
  enum{eRemoveInterval=60*DURATION_FIGHT_MINUTE, // сек
    eTimeRefresh = 60, // сек
  };
  // инфо дл€ сервера
  unsigned int   ip;
  unsigned short port;
  char*          sNick;
  bool           flgDisconnect;
  unsigned int   time; // либо момент времени когда был последний раз посылка Ёха
  // либо момент времени потери св€зи

  // специально дл€ игровой комнаты
  //bool flgReadyForRoom;// готовность клиента к приему данных от комнаты

  TClient();
  ~TClient();

  // инфо дл€ клиента
  int            state;// garage, wait, fight
  TGarage 			 mGarage;

  void SetCurRoom(TRoom* room){pCurRoom=room;}
  TRoom* GetCurRoom(){return pCurRoom;}

  //TTreeTanks mTreeTanks;// читать из Ѕƒ ветку исследований - пока нету

};

#endif
