#include "share_test.h"
#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "..\GBaseLib\ErrorReg.h"
#include "..\GBaseLib\HiTimer.h"
#include "..\share\NetSystem.h"
#include "SaveOnHDD.h"

char packet[SIZE_PACKET];

float freq_printf_recv_packet = 10.0f;// %
float limit_recv_packet = 0;// %

//TMakerNetTransport
TMakerNetDoser 
g_MakerNetTransport;
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
  printf("Disconnect\n");
}
//-----------------------------------------------------------------------
