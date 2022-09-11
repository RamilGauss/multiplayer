#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>

#include "glib\gthread.h"

#include "MakerNetTransport.h"
#include "INetTransport.h"
#include "GlobalParams.h"
#include "ErrorReg.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "share_test.h"

#include "SaveOnHDD.h"

using namespace std;
//-----------------------------------------------------------------------
int numNetWork = 0;
int main(int argc, char** argv)
{
  Init("Server");

  INetTransport* pNetTransport = g_MakerNetTransport.New();

  bool res = pNetTransport->Open(PORT_SERVER);
  pNetTransport->Register(Recv,       INetTransport::eRecv);
  pNetTransport->Register(Disconnect, INetTransport::eDisconnect);

  pNetTransport->Start();

  _getch();

  pNetTransport->Stop();
  pNetTransport->Unregister(Recv,       INetTransport::eRecv);
  pNetTransport->Unregister(Disconnect, INetTransport::eDisconnect);

  g_MakerNetTransport.Delete(pNetTransport);
  return 0;
}
