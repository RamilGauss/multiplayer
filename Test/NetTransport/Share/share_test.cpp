#include "share_test.h"
#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>

#include "glib\gthread.h"

#include "ErrorReg.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "SaveOnHDD.h"
#include "Logger.h"


char packet[SIZE_PACKET];

float freq_printf_recv_packet = 10.0f;// %
float limit_recv_packet = 0;// %

volatile bool flgDisconnect = false;

void SetDisconnect();

//TMakerNetTransport
TMakerNetTransport_TCP_UDP 
g_MakerNetTransport;
//-----------------------------------------------------------------------
void Init(char* nameLog)
{
  GetLogger()->Done();
  GetLogger()->Register(STR_NAME_NET_TRANSPORT);
  GetLogger()->Init(nameLog);
  GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF_time("Start.\n");

#ifdef TD_WINDOWS
  setlocale( LC_ALL, "Russian_russia.1251" );
#else
  setlocale( LC_ALL, "ru_RU.koi8r" );
#endif
  bl_initLocale();

  g_thread_init( NULL );
  err_Init();
  errSTR_Init();
  errSTD_Init();
  errSDK_Init();
  ht_Init();
  ns_Init();
}
//-----------------------------------------------------------------------
void RecvPacket(void* p, int s)
{
  static int cntRecv;
  static unsigned int startRecvPacket;
  if(cntRecv==0)
    startRecvPacket = ht_GetMSCount();
  cntRecv++;
  if(float(cntRecv)/CNT_RECV_PACKET*100>limit_recv_packet)
  {
    printf("RecvPacket, cnt=%d\n",cntRecv);
    limit_recv_packet += freq_printf_recv_packet;
  }
  if(cntRecv==CNT_RECV_PACKET)
  {
    unsigned int now = ht_GetMSCount();
    unsigned int time_recv = now - startRecvPacket;
    printf("Recv time=%u, v=%f\n", time_recv, float(sizeof(packet)*CNT_RECV_PACKET)/(time_recv*1000));
  }
}
//-----------------------------------------------------------------------
void RecvStream(void* p, int s)
{
  static int cntStream;
  static unsigned int startRecvStream;
  if(cntStream==0)
    startRecvStream = ht_GetMSCount();
  cntStream++;
  if(float(cntStream)/CNT_RECV_PACKET*100>limit_recv_packet)
  {
    printf("RecvStream, cnt=%d\n",cntStream);
    limit_recv_packet += freq_printf_recv_packet;
  }
  if(cntStream==CNT_RECV_PACKET)
  {
    unsigned int now = ht_GetMSCount();
    unsigned int time_recv = now - startRecvStream;
    printf("Recv time=%u, v=%f\n", time_recv, float(sizeof(packet)*CNT_RECV_PACKET)/(time_recv*1000));
  }
}
//-----------------------------------------------------------------------
void Disconnect(void* p, int s)
{
	TIP_Port* pIP = (TIP_Port*)p;
  printf("Disconnect IP=0x%X port=%u\n", pIP->ip, pIP->port);
  flgDisconnect = true;
}
//-----------------------------------------------------------------------
void Recv(void* p, int s)
{
	INetTransport::TDescRecv* pDesc = (INetTransport::TDescRecv*)p;
	switch(pDesc->type)
	{
		case INetTransport::ePacket:
			RecvPacket(NULL,0);
			break;
		case INetTransport::eStream:
			RecvStream(NULL,0);
			break;
	}
}
//-----------------------------------------------------------------------
void SetDisconnect()
{
  flgDisconnect = true;
}
//-----------------------------------------------------------------------
bool IsDisconnect()
{
  return flgDisconnect;
}
//-----------------------------------------------------------------------
