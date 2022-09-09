#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "..\Melissa\MakerNET_Engine.h"
#include "..\Melissa\INET_Transport.h"
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

int main(int argc, char** argv)
{
  Init();
  TL_START(true);

  TMakerNET_Transport maker;
  pNET_Transport = maker.New();TL_POINT("Make NET");
  pNET_Transport->InitLog("client.txt");TL_POINT("InitLog");

  bool res = pNET_Transport->Open(port_client,numNetWork);TL_POINT("Open");
  pNET_Transport->Register(RecvPacket,nsCallBackType::eRcvPacket);
  pNET_Transport->Register(RecvStream,nsCallBackType::eRcvStream);
  pNET_Transport->Register(Disconnect,nsCallBackType::eDisconnect);

  //unsigned int ip_server = ns_inet_addr("192.168.23.230");
  unsigned int ip_server = ns_inet_addr(ns_getSelfIP(numNetWork));
  pNET_Transport->Start(); TL_POINT("Start");

  INET_Transport::InfoData p;
  p.ip_dst   = ip_server;
  p.port_dst = port_server;
  p.packet = &packet[0];
  p.size   = sizeof(packet);
  
  TL_POINT("Before send");
  guint32 start = ht_GetMSCount();
  for(int i = 0 ; i < cntPacket ; i++)
  {
    pNET_Transport->Write(&p,true,false);
  }
  start = ht_GetMSCount() - start;

  TL_POINT("Send");
  TL_PRINT("end",true);
    
  printf("time=%d ms, v=%f \n",start,float(p.size*cntPacket)/(start*1000));
  _getch();
  return 0;
}
