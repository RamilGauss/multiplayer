/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "TransportProtocolTank.h"
#include "common_defs.h"
#include "HiTimer.h"
#include "BL_Debug.h"
#include "GlobalParamsTank.h"
#include "NetSystem.h"
#include <Winsock2.h>
#include <limits.h>


#define DEBUG

// не логировать стрим
#ifdef DEBUG_ONLY_PACKET 
  #define DEBUG_PACKET_ONLY_WRITE
  #define DEBUG_PACKET_ONLY_READ
#else
  #define DEBUG_PACKET_ONLY_WRITE \
  if(((TPrefixTransport*)p)->type=='S') return;

  #define DEBUG_PACKET_ONLY_READ \
  if(((TPrefixTransport*)mBuffer)->type=='S') return;

#endif

#ifdef DEBUG
#define DEBUG_LOG
#else
#define DEBUG_LOG return;
#endif

//----------------------------------------------------------------------------
TransportProtocolTank::TransportProtocolTank(char* pPathLog)
{
  mArrFresh.Sort(SortFreshInfoConnect);
	mArrWaitCheck.Sort(SortFreshDefPacket);
	mArrWaitSend.Sort(SortFreshDefPacket);

	InitLog(pPathLog);
	mLogRcvSend.SetPrintf(false);
  mLogEvent.SetPrintf(false);
}
//----------------------------------------------------------------------------------
TransportProtocolTank::~TransportProtocolTank()
{

}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::Open(unsigned short port, int numNetWork)
{
  UdpDevice::TParams params;
#ifdef WIN32
	char* sLocalHost = ns_getSelfIP(numNetWork);
#else
  char sLocalHost[100];
  get_ip_first_eth(sLocalHost);
#endif

  params.m_LocalHost = ns_inet_addr(sLocalHost); 
  params.m_LocalPort = short(port); 

  mUDP.setPortSettings(params);
  bool resOpen = mUDP.open();
  if(!resOpen) {BL_FIX_BUG();return false;}

  return true;
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::write(InfoData* data, bool check)
{
	InfoConnect* pFound = GetInfoConnect(data->ip_dst,data->port_dst);
	data->ip_src   = mUDP.portSetting().m_LocalHost;
	data->port_src = mUDP.portSetting().m_LocalPort;
  // check - добавл€ть ли в список пакетов, ожидающих подтверждени€
  TDefPacket* pDefPacket = new TDefPacket;
  pDefPacket->SetData(data);
	
  if(check)
  {
		pDefPacket->SetType('P');
  	//-------------------------------------------------
		lockSendRcv();
		// есть ли по данному IP:port в ожидании квитанции?
		if(FindInArrWaitCheck(pDefPacket))
			mArrWaitSend.Add(pDefPacket);
		else
		{
			pFound->cn_out_p++;
			pDefPacket->SetCnOut(pFound->cn_out_p);
			pDefPacket->SetCnIn(pFound->cn_in_p/*вообще по фигу но дл€ отладки*/);

			mArrWaitCheck.Add(pDefPacket);
			Send(pDefPacket);
		}

		unlockSendRcv();
		//-------------------------------------------------
    return;
  }

	pFound->cn_out_s++;
	pDefPacket->SetCnOut(pFound->cn_out_s);
	pDefPacket->SetCnIn(pFound->cn_in_s/*вообще по фигу но дл€ отладки*/);

	pDefPacket->SetType('S');
  Send(pDefPacket);

	delete pDefPacket;
}
//----------------------------------------------------------------------------------
void* ThreadTransport(void*p)
{
	((TransportProtocolTank*)p)->Engine();
	return NULL;
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::Engine()
{
	flgNeedStop = false;
	flgActive = true;
	while(!flgNeedStop)
	{
		int timeout = GetTimeout();
		unsigned int ip;
		unsigned short port;
		int res = mUDP.read(mBuffer,eSizeBuffer,timeout, ip, port);
		
		switch(res)
		{
			case RR_ERROR:
			case RR_BREAK:
				break;
			case RR_TIMEOUT:
				break;
			default:
			{
				ReadLog(res,ip,port);
				AnalizPacket(ip, port, res);
			};
		}
		SendUnchecked();
	}
	flgActive = false;
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::start()
{
  thread = g_thread_create(ThreadTransport,
    (gpointer)this,
    true,
    NULL);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::stop()
{
	flgNeedStop = true;
	while(flgActive)
	{
		ht_msleep(eWaitThread);
	}
	mUDP.close();
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::AnalizPacket(unsigned int ip,unsigned short port,int size)
{
  TPrefixTransport* prefix = (TPrefixTransport*)mBuffer;
  switch(prefix->type)
  {
		case 'C':
		{
			InfoConnect* pFoundConnect = GetInfoConnect(ip,port);
			lockSendRcv();
			  pFoundConnect->cn_in_s  = 0;
			  pFoundConnect->cn_out_s = 0;
			  pFoundConnect->cn_in_p  = 0;
			  pFoundConnect->cn_out_p = 0;
			  SendCheck(prefix,ip,port);
			unlockSendRcv();
			break;
		}
    case 'K':
			lockSendRcv();
				FindAndCheck(prefix,ip,port);
			unlockSendRcv();
      break;
    case 'P':
      SendCheck(prefix,ip,port);
			if(IsPacketFresh())
				NotifyRcvPacket(size);
      break;
    case 'S':
			if(IsStreamFresh())
				NotifyRcvStream(size);
      break;
    default:BL_FIX_BUG();;
  }
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::FindAndCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port)
{
	TDefPacket oForSearchDefPacket;
	TDefPacket *pForSearchDefPacket = &oForSearchDefPacket;
	InfoData* infoConnect = new InfoData;
	infoConnect->ip_dst = ip;
	infoConnect->port_dst = port;
	infoConnect->size = 0;
	pForSearchDefPacket->SetData(infoConnect);
	int index = mArrWaitCheck.FastSearch(&pForSearchDefPacket,NULL,SortFreshDefPacket);
	if(index==-1)
  {
    mLogEvent.WriteF("FindAndCheck: index==-1\n");
    //BL_FIX_BUG();
    return;
  }
	//-------------------------------------------------------------------------------------
	TDefPacket* pFoundDefPacket = (TDefPacket*)mArrWaitCheck.Get(index);

	if(pFoundDefPacket->GetCnOut()==prefix->cn_out)
	{
		int indexWaitSend = mArrWaitSend.FastSearch(&pFoundDefPacket,NULL,SortFreshDefPacket);
		mArrWaitCheck.Delete(index);
		if(indexWaitSend!=-1)
		{
			TDefPacket* pDefPacketWaitSend = (TDefPacket*)mArrWaitSend.Unlink(indexWaitSend);

			InfoConnect* pFound = GetInfoConnect(pDefPacketWaitSend->GetIP_dst(),pDefPacketWaitSend->GetPort_dst());

			pFound->cn_out_p++;
			pDefPacketWaitSend->SetCnOut(pFound->cn_out_p);
			pDefPacketWaitSend->SetCnIn(pFound->cn_in_p/*вообще по фигу но дл€ отладки*/);

			mArrWaitCheck.Add(pDefPacketWaitSend);
			Send(pDefPacketWaitSend);
		}
		return;
	}

	mLogEvent.WriteF("FindAndCheck() pDefPacket->GetCnOut()=%u,prefix->cn_out=%u\n",pFoundDefPacket->GetCnOut(),prefix->cn_out);

	BL_FIX_BUG();
}
//----------------------------------------------------------------------------------
int TransportProtocolTank::GetTimeout()
{
	int to = eTimeout;//###
	return to;
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::SendUnchecked()
{
	lockSendRcv();
  guint32 now_ms = ht_GetMSCount();
	for(int i = 0 ; i < mArrWaitCheck.Count() ; i++)
	{
		TDefPacket* pDefPacket = (TDefPacket*)mArrWaitCheck.Get(i);
		if(pDefPacket)
		{
			if(pDefPacket->GetTime()+eTimeLivePacket<now_ms) 
			if(Send(pDefPacket)==false)
			{
				Disconnect(pDefPacket);
				mArrWaitCheck.Delete(i);
				i--;
			}
		}
		else BL_FIX_BUG();
	}
	unlockSendRcv();
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::Register(CallBackRegistrator::TCallBackFunc pFunc, int type)
{
  switch(type)
  {
		case nsCallBackType::eRcvPacket:
      mCallBackRecvPacket.Register(pFunc);
      break;
		case nsCallBackType::eRcvStream:
			mCallBackRecvStream.Register(pFunc);
			break;
    case nsCallBackType::eDisconnect:
      mCallBackDisconnect.Register(pFunc);
      break;
    default:BL_FIX_BUG();
  }
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::Unregister(CallBackRegistrator::TCallBackFunc pFunc, int type)
{
  switch(type)
  {
		case nsCallBackType::eRcvPacket:
			mCallBackRecvPacket.Unregister(pFunc);
			break;
		case nsCallBackType::eRcvStream:
			mCallBackRecvStream.Unregister(pFunc);
			break;
		case nsCallBackType::eDisconnect:
			mCallBackDisconnect.Unregister(pFunc);
			break;
    default:BL_FIX_BUG();
  }
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::Send(TDefPacket* pDefPacket)
{
  unsigned char cntTry = pDefPacket->GetCntTry();
  if((unsigned char)(cntTry+1)>eCntTry)
    return false;
  
  pDefPacket->SetCntTry(cntTry+1);  
  pDefPacket->SetTime(ht_GetMSCount());

  int size;
  unsigned int ip;
  unsigned short port;
  void* pData = pDefPacket->GetData(size,ip, port);
  return Write(pData,size,ip, port);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::Disconnect(TDefPacket* pDefPacket)
{
  int size;
  unsigned int ip;
  unsigned short port;
  void* pData = pDefPacket->GetData(size,ip, port);
  TIP_Port dis;
  dis.ip   = ip;
  dis.port = port;
  notifyDisconnect(&dis);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::NotifyRcvPacket(int size)
{
	// смещение по пакету до места ip и port
  int shift = sizeof(TPrefixTransport)-sizeof(TIP_Port);
  size -= shift;
  notifyRcvPacket(mBuffer+shift,size);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::NotifyRcvStream(int size)
{
	// смещение по пакету до места ip и port
	int shift = sizeof(TPrefixTransport)-sizeof(TIP_Port);
	size -= shift;
	notifyRcvStream(mBuffer+shift,size);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::SendCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port)
{
  TPrefixTransport check;
  check.cn_in  = prefix->cn_in;
  check.cn_out = prefix->cn_out;
  check.type   = 'K';
	check.cntTry = 0;
	check.time_ms = ht_GetMSCount();
	check.ip_port_dst.ip   = ip;
	check.ip_port_dst.port = port;
	check.ip_port_src.ip   = mUDP.portSetting().m_LocalHost;
	check.ip_port_src.port = mUDP.portSetting().m_LocalPort;

  Write(&check,sizeof(TPrefixTransport),ip, port);
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::Write(void *p, int size, unsigned int ip, unsigned short port)
{
	WriteLog(p, size, ip, port);
	return mUDP.write(p, size, ip, port);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::WriteLog(void *p, int size, unsigned int ip, unsigned short port)
{
  DEBUG_PACKET_ONLY_WRITE

	DEBUG_LOG
	char * sIP = ns_str_addr(ip);
	if(sIP==NULL) {BL_FIX_BUG();return;}
	mLogRcvSend.WriteF("Send Packet to ip=%s, port=%u\n",sIP,port);
	LogTransportInfo((TPrefixTransport*)p,size);
	mLogRcvSend.WriteF("\n");
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::ReadLog(int size, unsigned int ip, unsigned short port)
{
  DEBUG_PACKET_ONLY_READ

  DEBUG_LOG
	char * sIP = ns_str_addr(ip);
	if(sIP==NULL) {BL_FIX_BUG();return;}
	mLogRcvSend.WriteF("Recive Packet from ip=%s, port=%u\n",sIP,port);
	LogTransportInfo((TPrefixTransport*)mBuffer,size);
	mLogRcvSend.WriteF("\n");
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::LogTransportInfo(TPrefixTransport* p,int size)
{
	unsigned int hour   = p->time_ms/(1000*3600);
	unsigned int minute = p->time_ms/(1000*60)-hour*60;
	unsigned int second = p->time_ms/1000-minute*60-hour*3600;
	unsigned int ms     = p->time_ms-1000*(p->time_ms/1000);

	unsigned int localTime = ht_GetMSCount();
	unsigned int Lhour   = localTime/(1000*3600);
	unsigned int Lminute = localTime/(1000*60)-Lhour*60;
	unsigned int Lsecond = localTime/1000-Lminute*60-Lhour*3600;
	unsigned int Lms     = localTime-1000*(localTime/1000);

	char sType[3];
	sType[0] = p->type;
	sType[1] = '\0';
  char sIP_dst[50];char sIP_src[50];
  strcpy(sIP_dst,ns_str_addr(p->ip_port_dst.ip));
  strcpy(sIP_src,ns_str_addr(p->ip_port_src.ip));

	mLogRcvSend.WriteF("TransportInfo: \n\t type=%s,\n\t cn_in=%u,\n\t cn_out=%u,\n\t time=%u:%u:%u.%03u,\n\t localTime=%u:%u:%u.%03u,\n\t cntTry=%u,\n\t DST=%s:%u,\n\t SRC=%s:%u\n",
							sType,   // K - квитанци€, S - нет гарантии доставки, P - гаранти€ доставки
							p->cn_in,     // циклический номер
							p->cn_out,     // циклический номер
							hour,minute,second,ms,
							Lhour,Lminute,Lsecond,Lms,
							p->cntTry,
							sIP_dst,
							p->ip_port_dst.port,  //
							sIP_src,  //
							p->ip_port_src.port);
	mLogRcvSend.WriteF("Contain packet:\n");
	unsigned char* data   = (unsigned char*)p+sizeof(TPrefixTransport);
	int sizeData = size-sizeof(TPrefixTransport);
	for(int i = 0 ; i < sizeData ; i++)
		mLogRcvSend.WriteF("0x%X ",data[i]);
	mLogRcvSend.WriteF("\n");
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::IsPacketFresh()
{
	TPrefixTransport* p = (TPrefixTransport*)mBuffer;
	InfoConnect* pFoundFresh = GetInfoConnect(p->ip_port_src.ip,p->ip_port_src.port);

  if(A_more_B(p->cn_out,pFoundFresh->cn_in_p))
  {
    pFoundFresh->cn_in_p = p->cn_out;
    return true;
  }
	
	//###BL_FIX_BUG();
  return false;
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::IsStreamFresh()
{
	TPrefixTransport* p = (TPrefixTransport*)mBuffer;
	InfoConnect* pFoundFresh = GetInfoConnect(p->ip_port_src.ip,p->ip_port_src.port);

	if(A_more_B(p->cn_out,pFoundFresh->cn_in_s))
	{
		pFoundFresh->cn_in_s = p->cn_out;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::A_more_B(unsigned short A, unsigned short B)
{
  if(A>B)
  {
    if(A-B>USHRT_MAX/2) return false;
    else return true;
  }
  else
    if(B-A>USHRT_MAX/2) return true;

  return false;
}
//----------------------------------------------------------------------------------
int TransportProtocolTank::SortFreshInfoConnect(const void* p1, const void* p2)
{
  const InfoConnect *s1 = *( const InfoConnect **)p1;
  const InfoConnect *s2 = *( const InfoConnect **)p2;

  if(s1->ip>s2->ip)
    return -1;
  else 
    if(s1->ip==s2->ip)
    {
      if(s1->port>s2->port)
        return -1;
      else 
        if(s1->port==s2->port)
          return 0;
    }
  return 1;
}
//----------------------------------------------------------------------------------
int TransportProtocolTank::SortFreshDefPacket(const void* p1, const void* p2)
{
  const TDefPacket *s1 = *( const TDefPacket **)p1;
  const TDefPacket *s2 = *( const TDefPacket **)p2;

  if(s1->GetIP_dst()>s2->GetIP_dst())
    return -1;
  else 
    if(s1->GetIP_dst()==s2->GetIP_dst())
    {
      if(s1->GetPort_dst()>s2->GetPort_dst())
        return -1;
      else 
        if(s1->GetPort_dst()==s2->GetPort_dst())
          return 0;
    }
    return 1;
}
//----------------------------------------------------------------------------------
TransportProtocolTank::InfoConnect* TransportProtocolTank::GetInfoConnect(unsigned int ip,unsigned short port)
{
  InfoConnect* pFresh = new InfoConnect;
  pFresh->ip    = ip;
  pFresh->port  = port;

	lockArrFresh();
  InfoConnect* pFoundFresh = (InfoConnect*)mArrFresh.Get(mArrFresh.FastSearch(&pFresh,NULL,SortFreshInfoConnect));
	unlockArrFresh();
  if(pFoundFresh==NULL)
  {
		lockArrFresh();
    mArrFresh.Add(pFresh);
		unlockArrFresh();
    return pFresh;
  }
  delete pFresh;

	return pFoundFresh;
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::synchro(unsigned int ip, unsigned short port)
{
	InfoConnect* pInfoConnect = GetInfoConnect(ip,port);
	pInfoConnect->cn_in_s = 0;
	pInfoConnect->cn_out_s = 0;
	pInfoConnect->cn_in_p = 0;
	pInfoConnect->cn_out_p = 0;

  if(!mUDP.isOpen()){BL_FIX_BUG();return false;}
  
  guint32 now_ms = ht_GetMSCount();
  guint32 start_ms = now_ms;
  int cntTry = 0;
	guint32 time_send = 0;
  while((start_ms+eWaitSynchro*1000)>now_ms)
  {
		if(now_ms>time_send+eTimeLivePacket)
		{
			time_send	= ht_GetMSCount();
			if(!SendSynchro(ip,port,cntTry)) {mUDP.close();return false;}
		  cntTry++;
		}
    
    int res = mUDP.read(mBuffer,eSizeBuffer,(eWaitSynchro*1000)/eCntTry, ip, port);
    switch(res)
    {
      case RR_ERROR:
      case RR_BREAK:
      case RR_TIMEOUT:
			{
				mUDP.close();
				return false;
			}
      default:;
    }

    ReadLog(res,ip,port);
    TPrefixTransport* prefix = (TPrefixTransport*)mBuffer;
    if(prefix->type  =='K')
		if(prefix->cn_in ==0)
		if(prefix->cn_out==0)
      return true;
    now_ms = ht_GetMSCount();
  }
  return true;
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::SendSynchro(unsigned int ip, unsigned short port, int cntTry)
{
  // начало сеанса, обнулить счетчик на той стороне
  TPrefixTransport synchro;
  synchro.cn_out = 0; // в начале ÷Ќ=0
  synchro.cn_in  = 0; // в начале ÷Ќ=0
  synchro.type   = 'C';
  synchro.cntTry = cntTry;
	synchro.time_ms = ht_GetMSCount();
  synchro.ip_port_dst.ip   = ip;
  synchro.ip_port_dst.port = port;
  synchro.ip_port_src.ip   = mUDP.portSetting().m_LocalHost;
  synchro.ip_port_src.port = mUDP.portSetting().m_LocalPort;

  return Write(&synchro,sizeof(TPrefixTransport),ip, port);
}
//----------------------------------------------------------------------------------
void TransportProtocolTank::InitLog(char* pPathLog)
{  
	if(pPathLog)
	{		
		char pPathLogEvent[400];
		sprintf(pPathLogEvent,"%sEvent",pPathLog);

		mLogRcvSend.ReOpen(pPathLog);
		mLogEvent.ReOpen(pPathLogEvent);
	}
}
//----------------------------------------------------------------------------------
bool TransportProtocolTank::FindInArrWaitCheck(TDefPacket* pDefPacket)
{
	if(mArrWaitCheck.FastSearch(&pDefPacket,NULL,SortFreshDefPacket)!=-1)
		return true;

	return false;
}
//----------------------------------------------------------------------------------
