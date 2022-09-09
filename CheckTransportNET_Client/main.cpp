#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "MakerNetTransport.h"
#include "INetTransport.h"
#include "..\Share\GlobalParams.h"
#include "..\glib-2.0\glib\gthread.h"
#include "..\GBaseLib\ErrorReg.h"
#include "..\GBaseLib\HiTimer.h"
#include "..\share\NetSystem.h"
#include "SaveOnHDD.h"

#define TIMER_LOG_ENABLE
#include "TimerLog.h"
#include "..\CheckTransport_Share\share_test.h"

using namespace std;

int numNetWork = 0;

struct TArgData
{
  unsigned int ip;
  int cnt;
  int time_sleep;
  TArgData()
  {
    ip = ns_inet_addr(ns_getSelfIP(numNetWork));
    cnt = 190;
    time_sleep = 15;
  }
};
//-------------------------------------------------------------------
void GetByArg(int argc, char** argv, TArgData &d);
//-------------------------------------------------------------------
int main(int argc, char** argv)
{
  Init();
  TL_START(true);

  INetTransport* pNetTransport = g_MakerNetTransport.New();TL_POINT("Make Net");
  pNetTransport->InitLog("client.txt");TL_POINT("InitLog");

  bool res = pNetTransport->Open(PORT_CLIENT);TL_POINT("Open");
  pNetTransport->Register(RecvPacket, INetTransport::eRcvPacket);
  pNetTransport->Register(RecvStream, INetTransport::eRcvStream);
  pNetTransport->Register(Disconnect, INetTransport::eDisconnect);

  TArgData d;
  GetByArg(argc,argv,d);
  printf("TimeSleep=%d,cnt=%d\n",d.time_sleep, d.cnt);

  TBreakPacket packetForSend;
	if(pNetTransport->Synchro(d.ip, PORT_SERVER))
	{
		pNetTransport->Start(); TL_POINT("Start");

		TL_POINT("Before send");
		unsigned int start = ht_GetMSCount();

		for(int i = 0 ; i < CNT_RECV_PACKET ;)
		{
			for(int j = 0 ; j < d.cnt ; j++ )
			{
        packetForSend.UnlinkPart();
        packetForSend.PushFront(&packet[0], sizeof(packet));
        pNetTransport->Send(d.ip, PORT_SERVER, packetForSend, true);
				i++;
				if(i==CNT_RECV_PACKET)
					break;
			}
			ht_msleep(d.time_sleep);
		}
		start = ht_GetMSCount() - start;

		TL_POINT("Send");
		TL_PRINT("end",true);

		printf("time=%d ms, v=%f \n",start,float(sizeof(packet)*CNT_RECV_PACKET)/(start*1000));
		_getch();

		pNetTransport->Stop();
	}

  pNetTransport->Unregister(RecvPacket,INetTransport::eRcvPacket);
  pNetTransport->Unregister(RecvStream,INetTransport::eRcvStream);
  pNetTransport->Unregister(Disconnect,INetTransport::eDisconnect);

  g_MakerNetTransport.Delete(pNetTransport);

  return 0;
}
//-----------------------------------------------------------------------
void GetByArg(int argc, char** argv, TArgData &d)
{
  if(argc<2) return;
  d.ip = ns_inet_addr(argv[1]);
  if(argc<4) return;
  d.time_sleep = atoi(argv[2]);
  d.cnt = atoi(argv[3]);
}
//-----------------------------------------------------------------------
