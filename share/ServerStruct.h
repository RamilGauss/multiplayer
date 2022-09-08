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

