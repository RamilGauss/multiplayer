#ifndef SHARE_TESTH
#define SHARE_TESTH

#include "INetTransport.h"
#include "MakerNetTransport.h"

#define PORT_CLIENT 1234
#define PORT_SERVER 1235
#define SIZE_PACKET 1350
#define CNT_RECV_PACKET 1000 //100000

extern char packet[SIZE_PACKET];

extern TMakerNetTransport_TCP_UDP g_MakerNetTransport;

extern void Init(char* nameLog);

extern void Recv(void* p, int s);
extern void Disconnect(void* p, int s);

extern bool IsDisconnect();

#endif

