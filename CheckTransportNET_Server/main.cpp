#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "..\Melissa\MakerNET_Engine.h"
#include "..\Melissa\INET_Engine.h"
#include "..\Share\GlobalParams.h"
#include "..\glib-2.0\glib\gthread.h"
#include "..\GBaseLib\ErrorReg.h"
#include "..\GBaseLib\HiTimer.h"
#include "..\share\NetSystem.h"
#include "SaveOnHDD.h"
#include "..\CheckTransport_Share\share_test.h"


using namespace std;

//-----------------------------------------------------------------------
int numNetWork = 0;
int main(int argc, char** argv)
{
  Init();

  TMakerNET_Engine maker;
  pNET_Transport = maker.New();
  pNET_Transport->InitLog("server.txt");//###

  bool res = pNET_Transport->Open(port_server,numNetWork);
  pNET_Transport->Register(RecvPacket,nsCallBackType::eRcvPacket);
  pNET_Transport->Register(RecvStream,nsCallBackType::eRcvStream);
  pNET_Transport->Register(Disconnect,nsCallBackType::eDisconnect);

  pNET_Transport->Start();

  _getch();
  return 0;
}
