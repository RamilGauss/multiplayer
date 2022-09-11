#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <windows.h>
#include <functional>

#include "Client.h"
#include "IMakerTransport.h"
#include "HiTimer.h"
#include "ShareMisc.h"
#include "DstEvent.h"
#include "WrapperMakerTransport.h"

static TDstEvent dst;

int main(int argc, char** argv)
{
	int c = GetCountCoreCPU();

  printf("Client Melissa\n");

  nsMelissa::IMakerTransport* pMT = new TWrapperMakerTransport;
  nsMelissa::TClient* pClient = new nsMelissa::TClient;
  //-----------------------------------------------------
  // for SrcEvent
  pClient->SetSelfID(0);
  pClient->SetDstObject(&dst);

  pClient->Init(pMT);
  pClient->Open(1234);
  pClient->Login(0, 0, NULL, 0);

  pClient->Work();// #0

  while(pClient->IsConnectUp()==false)// #1
  {
    ht_msleep(100);// ждем соединения
  }
  //-----------------------------------------------------
  delete pClient;
  delete pMT;
  return 0;
}
//-----------------------------------------------------------------------
