#ifndef SHARE_TESTH
#define SHARE_TESTH

#include "INetTransport.h"
#include "..\NetTransport\MakerNetTransport.h"

#define PORT_CLIENT 1234
#define PORT_SERVER 1235
#define SIZE_PACKET 1350
#define CNT_RECV_PACKET 100000

extern char packet[SIZE_PACKET];

extern 
//TMakerNetTransport
TMakerNetDoser 
g_MakerNetTransport;

extern void Init();

extern void Recv(void* p, int s);
extern void LostPacket(void* p, int s);
extern void Disconnect(void* p, int s);

extern int GetCntLostPacket();

#endif

