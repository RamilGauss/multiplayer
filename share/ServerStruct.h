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
    int mCurTank;// текущий танк, NULL - если нет танков
    TClient* pMasterClient;

  public:
    TArrayObject mArrTanks; // пока без БД тут будут всего два танка СТ и ТТ
    //TArrayObject mArrSomething;// снаряды на складе, и др.

    TGarage(TClient* pClient){pMasterClient=pClient;mCurTank=0;InitArrTank();};
    ~TGarage(){}

    void InitArrTank();
    
    bool SetCurTank(int i);
    int  GetCurTank();
    TTank* GetPointerCurTank();

    TClient* GetMasterClient(){return pMasterClient;};
	};
  //---------------------------------------------------------------
  class TClient : public TObject
  {
		TRoom*  			 pCurRoom; // либо что-то либо NULL
  public:
    enum{eMaxLenNick=250,};

		enum{eGarage=0,eWait=1,eFight=2};
    enum{eRemoveInterval=60*DURATION_FIGHT_MINUTE, // сек
				 eTimeRefresh = 60, // сек
        };
		// инфо для сервера
		unsigned int   ip;
    unsigned short port;
    char*          sNick;
		bool           flgDisconnect;
		unsigned int   time; // либо момент времени когда был последний раз посылка Эха
		// либо момент времени потери связи

    // специально для игровой комнаты
    //bool flgReadyForRoom;// готовность клиента к приему данных от комнаты
		
    TClient();
    ~TClient();
		
		// инфо для клиента
		int            state;// garage, wait, fight
		TGarage 			 mGarage;
    
    void SetCurRoom(TRoom* room){pCurRoom=room;}
    TRoom* GetCurRoom(){return pCurRoom;}

		//TTreeTanks mTreeTanks;// читать из БД ветку исследований - пока нету

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

