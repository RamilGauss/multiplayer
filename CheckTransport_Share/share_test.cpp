#include "share_test.h"
#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "..\Melissa\MakerNET_Engine.h"
#include "..\Share\GlobalParams.h"
#include "..\glib-2.0\glib\gthread.h"
#include "..\GBaseLib\ErrorReg.h"
#include "..\GBaseLib\HiTimer.h"
#include "..\share\NetSystem.h"
#include "SaveOnHDD.h"

INET_Transport* pNET_Transport = NULL;
char packet[1450];//8170];
int cntPacket = 100;
unsigned short port_client = 1234;
unsigned short port_server = 1235;

float freq_printf_recv_packet = 10.0f;// %
float limit_recv_packet = 0;// %
//-----------------------------------------------------------------------
void Init()
{
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
  static guint32 startRecvPacket;
  if(cntRecv==0)
    startRecvPacket = ht_GetMSCount();
  cntRecv++;
  if(float(cntRecv)/cntPacket*100>limit_recv_packet)
  {
    printf("RecvPacket, cnt=%d\n",cntRecv);
    limit_recv_packet += freq_printf_recv_packet;
  }
  if(cntRecv==cntPacket)
  {
    guint32 now = ht_GetMSCount();
    guint32 time_recv = now - startRecvPacket;
    printf("Recv time=%u, v=%f\n", time_recv, float(sizeof(packet)*cntPacket)/(time_recv*1000));
  }
}
//-----------------------------------------------------------------------
void RecvStream(void* p, int s)
{
  static int cntStream;
  cntStream++;
  if(!(cntStream%5000))
    printf("RecvStream, cnt=%d\n",cntStream);
}
//-----------------------------------------------------------------------
void Disconnect(void* p, int s)
{
  printf("Disconnect\n");
}
//-----------------------------------------------------------------------
