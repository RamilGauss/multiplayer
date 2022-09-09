#ifndef SHARE_TESTH
#define SHARE_TESTH

#include "..\NetTransport\INetTransport.h"
#include "..\NetTransport\MakerNetTransport.h"

#define PORT_CLIENT 1234
#define PORT_SERVER 1235
#define SIZE_PACKET 1370
#define CNT_RECV_PACKET 100000

extern char packet[SIZE_PACKET];

extern 
//TMakerNetTransport
TMakerNetDoser 
g_MakerNetTransport;

extern void Init();

extern void RecvPacket(void* p, int s);
extern void RecvStream(void* p, int s);
extern void Disconnect(void* p, int s);

#endif

