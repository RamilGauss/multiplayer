#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <windows.h>
#include <functional>

#include "glib\gthread.h"

#include "MakerNetTransport.h"
#include "INetTransport.h"
#include "GlobalParams.h"
#include "ErrorReg.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "SaveOnHDD.h"
#include "TimerLog.h"
#include "ListPtr.h"
#include "BreakPacket.h"

#include "share_test.h"

#define TIMER_LOG_ENABLE

using namespace std;

int numNetWork = 0;

struct TArgData
{
	unsigned short port_client;
  unsigned int ip;
  int cnt;
  int time_sleep;
  TArgData()
  {
		port_client = PORT_CLIENT;
    ip = ns_inet_addr(ns_getSelfIP(numNetWork));
    cnt = 700;//190;
    time_sleep = 0;
  }
};
//-------------------------------------------------------------------
void GetByArg(int argc, char** argv, TArgData &d);
//-------------------------------------------------------------------
int main(int argc, char** argv)
{
  Init("Client");

	TArgData d;
	GetByArg(argc,argv,d);
	printf("port=%u,TimeSleep=%d,cnt=%d\n",d.port_client, d.time_sleep, d.cnt);

  TL_START(true);

  INetTransport* pNetTransport = g_MakerNetTransport.New();TL_POINT("Make Net");

  bool res = pNetTransport->Open(d.port_client);TL_POINT("Open");
  pNetTransport->Register(Recv,       INetTransport::eRecv);
  pNetTransport->Register(Disconnect, INetTransport::eDisconnect);

  TBreakPacket packetForSend;
	if(pNetTransport->Connect(d.ip, PORT_SERVER))
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

  pNetTransport->Unregister(Recv,       INetTransport::eRecv);
  pNetTransport->Unregister(Disconnect, INetTransport::eDisconnect);

  g_MakerNetTransport.Delete(pNetTransport);

  return 0;
}
//-----------------------------------------------------------------------
void GetByArg(int argc, char** argv, TArgData &d)
{
	for( int i = 1 ; i < argc ; i++)
	{
		switch(i)
		{
			case 1:
				d.port_client = atoi(argv[i]);
				break;
			case 2:
			  d.time_sleep = atoi(argv[i]);
				break;
			case 3:
				d.cnt = atoi(argv[i]);
				break;
			case 4:
				d.ip = ns_inet_addr(argv[i]);
				break;
			default:;
		}
	}
}
//-----------------------------------------------------------------------