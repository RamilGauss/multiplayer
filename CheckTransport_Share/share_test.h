#ifndef SHARE_TESTH
#define SHARE_TESTH

#include "..\Melissa\INET_Transport.h"


extern INET_Transport* pNET_Transport;
extern char packet[1450];//8170];
extern int cntPacket;
extern unsigned short port_client;
extern unsigned short port_server;

extern void Init();

extern void RecvPacket(void* p, int s);
extern void RecvStream(void* p, int s);
extern void Disconnect(void* p, int s);


#endif

