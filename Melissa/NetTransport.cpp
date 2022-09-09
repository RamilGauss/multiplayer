/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

/*
  04.02.2013: 
    Gauss: Add:     queue for Write.
           Fix bug: delete desc_packet from arrWaitSend and arrWaitCheck when disconnect.
           Add:     delete desc_packet when recv packet's check.
*/


#include "NetTransport.h"
#include "common_defs.h"
#include "HiTimer.h"
#include "BL_Debug.h"
#include "GlobalParams.h"
#include "NetSystem.h"


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
TNetTransport::TNetTransport(char* pPathLog)
{
  flgActive = false;
  mArrFresh.Sort(SortFreshInfoConnect);
	mArrWaitCheck.Sort(SortFreshDefPacket);
	mArrWaitSend.Sort(SortFreshDefPacket);

	InitLog(pPathLog);
	mLogRcvSend.SetPrintf(false);
  mLogEvent.SetPrintf(false);
}
//----------------------------------------------------------------------------------
TNetTransport::~TNetTransport()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport::Open(unsigned short port, int numNetWork)
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
void TNetTransport::Write(InfoData* data, bool check, bool add_in_queue )
{
	InfoConnect* pFound = GetInfoConnect(data->ip_dst,data->port_dst);
	data->ip_src   = mUDP.portSetting().m_LocalHost;
	data->port_src = mUDP.portSetting().m_LocalPort;
  // check - добавлять ли в список пакетов, ожидающих подтверждения
  TDescPacket* pDescPacket = new TDescPacket;
  pDescPacket->SetData(data);
	
  if(check)
  {
		pDescPacket->SetType('P');
    pDescPacket->SetIsQueue(add_in_queue);
  	//-------------------------------------------------
		lockSendRcv();
		// есть ли по данному IP:port в ожидании квитанции?
		if(add_in_queue && // нужно поместить в очередь
       FindInArrWaitCheckQ(pDescPacket))
			mArrWaitSend.Add(pDescPacket);
		else
		{
			pFound->cn_out_p++;
			pDescPacket->SetCnOut(pFound->cn_out_p);
			pDescPacket->SetCnIn(pFound->cn_in_p/*вообще по фигу, но для отладки*/);

  	  mArrWaitCheck.Add(pDescPacket);
			Send(pDescPacket);
		}

		unlockSendRcv();
		//-------------------------------------------------
    return;
  }

	pFound->cn_out_s++;
	pDescPacket->SetCnOut(pFound->cn_out_s);
	pDescPacket->SetCnIn(pFound->cn_in_s/*вообще по фигу но для отладки*/);

	pDescPacket->SetType('S');
  Send(pDescPacket);

	delete pDescPacket;
}
//----------------------------------------------------------------------------------
void* ThreadTransport(void*p)
{
	((TNetTransport*)p)->Engine();
	return NULL;
}
//----------------------------------------------------------------------------------
void TNetTransport::Engine()
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
void TNetTransport::Start()
{
  thread = g_thread_create(ThreadTransport,
    (gpointer)this,
    true,
    NULL);
}
//----------------------------------------------------------------------------------
void TNetTransport::Stop()
{
	flgNeedStop = true;
	while(flgActive)
	{
		ht_msleep(eWaitThread);
	}
	mUDP.close();
}
//----------------------------------------------------------------------------------
void TNetTransport::AnalizPacket(unsigned int ip,unsigned short port,int size)
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
void TNetTransport::FindAndCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port)
{
  // ищем в массиве пакетов, ожидающих подтверждение
	TDescPacket oForSearchDefPacket;
	TDescPacket *pForSearchDefPacket = &oForSearchDefPacket;
	InfoData* infoConnect = new InfoData;
	infoConnect->ip_dst   = ip;
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
  int cnt = mArrWaitCheck.Count();
  for(int i = index ; i < mArrWaitCheck.Count() ; i++)
  {
	  TDescPacket* pFoundDefPacket = (TDescPacket*)mArrWaitCheck.Get(i);
    if(pFoundDefPacket->GetCnOut()==prefix->cn_out)
    {
      if(pFoundDefPacket->GetIsQueue())
      {
        int indexWaitSend = mArrWaitSend.FastSearch(&pFoundDefPacket,NULL,SortFreshDefPacket);
        mArrWaitCheck.Delete(i);
        if(indexWaitSend!=-1)
        {
          TDescPacket* pDefPacketWaitSend = (TDescPacket*)mArrWaitSend.Unlink(indexWaitSend);

          InfoConnect* pFound = GetInfoConnect(pDefPacketWaitSend->GetIP_dst(),pDefPacketWaitSend->GetPort_dst());

          pFound->cn_out_p++;
          pDefPacketWaitSend->SetCnOut(pFound->cn_out_p);
          pDefPacketWaitSend->SetCnIn(pFound->cn_in_p/*вообще по фигу, но для отладки*/);

          mArrWaitCheck.Add(pDefPacketWaitSend);
          Send(pDefPacketWaitSend);
        }
      }
      else
        mArrWaitCheck.Delete(i);
      return;
    }
  }

	mLogEvent.WriteF("FindAndCheck() prefix->cn_out=%u\n",prefix->cn_out);

	BL_FIX_BUG();
}
//----------------------------------------------------------------------------------
int TNetTransport::GetTimeout()
{
	int to = eTimeout;//###
	return to;
}
//----------------------------------------------------------------------------------
void TNetTransport::SendUnchecked()
{
	lockSendRcv();
  guint32 now_ms = ht_GetMSCount();
	for(int i = 0 ; i < mArrWaitCheck.Count() ; i++)
	{
		TDescPacket* pDefPacket = (TDescPacket*)mArrWaitCheck.Get(i);
		if(pDefPacket)
		{
			if(pDefPacket->GetTime()+eTimeLivePacket<now_ms) 
			if(Send(pDefPacket)==false)
			{
        Disconnect(pDefPacket);
        i--;
			}
		}
		else BL_FIX_BUG();
	}
	unlockSendRcv();
}
//----------------------------------------------------------------------------------
void TNetTransport::Register(TCallBackRegistrator::TCallBackFunc pFunc, int type)
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
void TNetTransport::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type)
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
bool TNetTransport::Send(TDescPacket* pDefPacket)
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
void TNetTransport::Disconnect(TDescPacket* pDefPacket)
{
  TIP_Port dis;
  dis.ip   = pDefPacket->GetIP_dst();
  dis.port = pDefPacket->GetPort_dst();
  notifyDisconnect(&dis);
  // Gauss 04.02.2013
  // удалить пакеты, которые стоят в очереди на отправку и получение квитанции
  // ищем в массиве пакетов, ожидающих подтверждение
  // совпадение по IP и port
  SearchAndDelete(&mArrWaitCheck,pDefPacket);
  SearchAndDelete(&mArrWaitSend, pDefPacket);
}
//----------------------------------------------------------------------------------
void TNetTransport::SearchAndDelete(TArrayObject* pArr, TDescPacket* pDefPacket)
{
  int index = pArr->FastSearch(&pDefPacket,NULL,SortFreshDefPacket);
  if(index==-1)// не нашли
  {
    BL_FIX_BUG();
    return;
  }
  //----------------------------------------------
  int cnt = pArr->Count();// перебор всех значений, для которых ip и port
  for(int i = index ; i < cnt ; i++ )
  {
    TDescPacket* pDesc = (TDescPacket*)pArr->Get(i);
    if((pDesc->GetIP_dst()  !=pDefPacket->GetIP_dst()  )||
       (pDesc->GetPort_dst()!=pDefPacket->GetPort_dst()))
      return;// прекратить поиск
    pArr->Delete(i);
    i--;
  }
}
//----------------------------------------------------------------------------------
void TNetTransport::NotifyRcvPacket(int size)
{
	// смещение по пакету до места ip и port
  int shift = sizeof(TPrefixTransport)-sizeof(TIP_Port);
  size -= shift;
  notifyRcvPacket(mBuffer+shift,size);
}
//----------------------------------------------------------------------------------
void TNetTransport::NotifyRcvStream(int size)
{
	// смещение по пакету до места ip и port
	int shift = sizeof(TPrefixTransport)-sizeof(TIP_Port);
	size -= shift;
	notifyRcvStream(mBuffer+shift,size);
}
//----------------------------------------------------------------------------------
void TNetTransport::SendCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port)
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
bool TNetTransport::Write(void *p, int size, unsigned int ip, unsigned short port)
{
	WriteLog(p, size, ip, port);
	return mUDP.write(p, size, ip, port);
}
//----------------------------------------------------------------------------------
void TNetTransport::WriteLog(void *p, int size, unsigned int ip, unsigned short port)
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
void TNetTransport::ReadLog(int size, unsigned int ip, unsigned short port)
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
void TNetTransport::LogTransportInfo(TPrefixTransport* p,int size)
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
							sType,   // K - квитанция, S - нет гарантии доставки, P - гарантия доставки
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
bool TNetTransport::IsPacketFresh()
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
bool TNetTransport::IsStreamFresh()
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
bool TNetTransport::A_more_B(unsigned short A, unsigned short B)
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
int TNetTransport::SortFreshInfoConnect(const void* p1, const void* p2)
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
int TNetTransport::SortFreshDefPacket(const void* p1, const void* p2)
{
  const TDescPacket *s1 = *( const TDescPacket **)p1;
  const TDescPacket *s2 = *( const TDescPacket **)p2;

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
TNetTransport::InfoConnect* TNetTransport::GetInfoConnect(unsigned int ip,unsigned short port)
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
bool TNetTransport::Synchro(unsigned int ip, unsigned short port)
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
		  if(cntTry>eCntTry) 
        return false;
      cntTry++;
		}
    
    int res = mUDP.read(mBuffer,eSizeBuffer,(eWaitSynchro*1000)/eCntTry, ip, port);
    switch(res)
    {
      case RR_ERROR:
      case RR_BREAK:
			{
				mUDP.close();
				return false;
			}
      case RR_TIMEOUT:
        break;
      default:
      {
        ReadLog(res,ip,port);
        TPrefixTransport* prefix = (TPrefixTransport*)mBuffer;
        if(prefix->type  =='K')
          if(prefix->cn_in ==0)
            if(prefix->cn_out==0)
              return true;
          break;       
      };
    }

    now_ms = ht_GetMSCount();
  }
  return false;
}
//----------------------------------------------------------------------------------
bool TNetTransport::SendSynchro(unsigned int ip, unsigned short port, int cntTry)
{
  // начало сеанса, обнулить счетчик на той стороне
  TPrefixTransport synchro;
  synchro.cn_out = 0; // в начале ЦН=0
  synchro.cn_in  = 0; // в начале ЦН=0
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
void TNetTransport::InitLog(char* pPathLog)
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
bool TNetTransport::FindInArrWaitCheckQ(TDescPacket* pDefPacket)
{
	//if(mArrWaitCheck.FastSearch(&pDefPacket,NULL,SortFreshDefPacket)!=-1)
	//	return true;

	//return false;
  
  int index = mArrWaitCheck.FastSearch(&pDefPacket,NULL,SortFreshDefPacket);
  if(index==-1)// не нашли
    return false;
  //----------------------------------------------
  int cnt = mArrWaitCheck.Count();// перебор всех значений, для которых ip и port
  for(int i = index ; i < cnt ; i++ )
  {
    TDescPacket* pDesc = (TDescPacket*)mArrWaitCheck.Get(i);
    if((pDesc->GetIP_dst()  !=pDefPacket->GetIP_dst()  )||
       (pDesc->GetPort_dst()!=pDefPacket->GetPort_dst()))
      return false;// прекратить поиск
    if(pDesc->GetIsQueue())
      return true;
  }
  return false;
}
//----------------------------------------------------------------------------------
void TNetTransport::Lock(void* pLocker)
{
  GCS* pGCS = (GCS*)pLocker;
  pGCS->lock();
}
//----------------------------------------------------------------------------------
void TNetTransport::Unlock(void* pLocker)
{
  GCS* pGCS = (GCS*)pLocker;
  pGCS->unlock();
}
//----------------------------------------------------------------------------------
