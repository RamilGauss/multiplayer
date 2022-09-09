#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "..\NetTransport\MakerNetTransport.h"
#include "..\NetTransport\INetTransport.h"
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

  INetTransport* pNetTransport = g_MakerNetTransport.New();
  pNetTransport->InitLog("server.txt");

  bool res = pNetTransport->Open(PORT_SERVER);
  pNetTransport->Register(RecvPacket,INetTransport::eRcvPacket);
  pNetTransport->Register(RecvStream,INetTransport::eRcvStream);
  pNetTransport->Register(Disconnect,INetTransport::eDisconnect);

  pNetTransport->Start();

  _getch();

  pNetTransport->Stop();
  pNetTransport->Unregister(RecvPacket,INetTransport::eRcvPacket);
  pNetTransport->Unregister(RecvStream,INetTransport::eRcvStream);
  pNetTransport->Unregister(Disconnect,INetTransport::eDisconnect);

  g_MakerNetTransport.Delete(pNetTransport);
  return 0;
}
