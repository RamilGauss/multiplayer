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


#ifndef ServerStructH
#define ServerStructH
#include "ApplicationProtocolPacket.h"
#include "GlobalParamsTank.h"
#include "TObject.h"
#include "hArray.h"
#include "HiTimer.h"

class TBasePacket;
class TRoom;
class TTank;

namespace nsServerStruct
{
  class TClient;
  //-----------------------------------------------------------------------------
	class TGarage
	{
    int mCurTank;// ������� ����, NULL - ���� ��� ������
    TClient* pMasterClient;

  public:
    TArrayObject mArrTanks; // ���� ��� �� ��� ����� ����� ��� ����� �� � ��
    //TArrayObject mArrSomething;// ������� �� ������, � ��.

    TGarage(TClient* pClient){pMasterClient=pClient;mCurTank=0;InitArrTank();};
    ~TGarage()
    {
      mArrTanks.Clear();
    }

    void InitArrTank();
    
    bool SetCurTank(int i);
    int  GetCurTank();
    TTank* GetPointerCurTank();

    TClient* GetMasterClient(){return pMasterClient;};
	};
  //---------------------------------------------------------------
  class TClient : public TObject
  {
		TRoom*  			 pCurRoom; // ���� ���-�� ���� NULL
  public:
    enum{eMaxLenNick=250,};

		enum{eGarage=0,eWait=1,eFight=2};
    enum{eRemoveInterval=60*DURATION_FIGHT_MINUTE, // ���
				 eTimeRefresh = 60, // ���
        };
		// ���� ��� �������
		unsigned int   ip;
    unsigned short port;
    char*          sNick;
		bool           flgDisconnect;
		unsigned int   time; // ���� ������ ������� ����� ��� ��������� ��� ������� ���
		// ���� ������ ������� ������ �����

    // ���������� ��� ������� �������
    //bool flgReadyForRoom;// ���������� ������� � ������ ������ �� �������
		
    TClient();
    ~TClient();
		
		// ���� ��� �������
		int            state;// garage, wait, fight
		TGarage 			 mGarage;
    
    void SetCurRoom(TRoom* room){pCurRoom=room;}
    TRoom* GetCurRoom(){return pCurRoom;}

		//TTreeTanks mTreeTanks;// ������ �� �� ����� ������������ - ���� ����

  };
  //---------------------------------------------------------------
  struct TPacketServer
  {
    unsigned int   ip;
    unsigned short port;
    guint32        ms_time;

    //int* packet;
    TBasePacket* packet;
  
    TPacketServer(){packet=NULL;}
    ~TPacketServer(){delete packet;}
  };
  //---------------------------------------------------------------

}
#endif

