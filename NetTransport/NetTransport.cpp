/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
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
#include "NetSystem.h"

using namespace std;
using namespace nsNetTransportStruct;

#ifndef FAST_NET_TRANSPORT
  #define FULL_DEBUG_TRANSPORT
  //#define DEBUG_STREAM_PACKET
#endif

#ifdef DEBUG_STREAM_PACKET
  #define DEBUG_PACKET_ONLY_WRITE
  #define DEBUG_PACKET_ONLY_READ
#else
// не логировать стрим
  #define DEBUG_PACKET_ONLY_WRITE \
  if(((THeader*)p)->type==eStream) return;

  #define DEBUG_PACKET_ONLY_READ \
  if(((THeader*)mBuffer)->type==eStream) return;

#endif

#ifdef FULL_DEBUG_TRANSPORT
#define DEBUG_LOG
#else
#define DEBUG_LOG return;
#endif

//----------------------------------------------------------------------------
TNetTransport::TNetTransport(char* pPathLog) : INetTransport(pPathLog),	
mTimeOut(eTimeLivePacketDef), mCntTry(eCntTryDef)
{
  flgActive = false;
  mArrConnect.Sort(SortFreshInfoConnect);
	mArrWaitCheck.Sort(SortFreshDefPacket);

	InitLog(pPathLog);
	mLogRcvSend.SetPrintf(false);
  mLogEvent.SetPrintf(false);

#ifndef FULL_DEBUG_TRANSPORT
  mLogRcvSend.SetEnable(false);
  mLogEvent.SetEnable(false);
#endif
}
//----------------------------------------------------------------------------------
TNetTransport::~TNetTransport()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport::Open(unsigned short port, unsigned char numNetWork)
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

  // настройка размеров буфера под прием
  SetupBufferForSocket();
  return true;
}
//----------------------------------------------------------------------------------
// Gauss 04.05.2013
void TNetTransport::Send(unsigned int ip, unsigned short port, 
                         TBreakPacket& packet, 
                         bool check)
{
  // собрать все части пакета, которые образовались при проходе через уровни
  // здесь идет отправка, то есть необходимо копировать в системную память
  packet.Collect();

  TShortDescPacket data;
  data.ip_port_dst.Set(ip, port);
  data.ip_port_src.Set(mUDP.portSetting().m_LocalHost, mUDP.portSetting().m_LocalPort);
  data.packet   = packet.GetCollectPtr();
  data.size     = packet.GetSize();
  //-----------------------------------------------------------------
  // check - добавлять ли в список пакетов, ожидающих подтверждения
  TBasePacketNetTransport* pPacket = new TBasePacketNetTransport;
  pPacket->SetHeader(&data);
	TInfoConnect* pInfoConnect = GetInfoConnect(data.ip_port_dst);
  if(check)
  {
		pPacket->SetType(ePacket);
  	//-------------------------------------------------
		lockSendRcv();

    pInfoConnect->cn_out_p++;
		pPacket->SetCnOut(pInfoConnect->cn_out_p);
		pPacket->SetCnIn(pInfoConnect->cn_in_p);//вообще по фигу но для отладки

	  mArrWaitCheck.Add(pPacket);
		Send(pPacket);

		unlockSendRcv();
		//-------------------------------------------------
    return;
  }
	pInfoConnect->cn_out_s++;
	pPacket->SetCnOut(pInfoConnect->cn_out_s);
	pPacket->SetCnIn(pInfoConnect->cn_in_s);//вообще по фигу но для отладки

	pPacket->SetType(eStream);
  bool resSend = Send(pPacket);
  BL_ASSERT(resSend);

	delete pPacket;
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
		NotifyLostPacket();
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
	while(IsActive()==false)
		ht_msleep(eWaitActivation);
}
//----------------------------------------------------------------------------------
void TNetTransport::Stop()
{
	flgNeedStop = true;
	while(IsActive())
	{
		ht_msleep(WaitThread());
	}
	mUDP.close();
}
//----------------------------------------------------------------------------------
bool TNetTransport::IsActive()
{
	return flgActive;
}
//--------------------------------------------------------------------------
void TNetTransport::AnalizPacket(unsigned int ip,unsigned short port,int size)
{
  THeader* prefix = (THeader*)mBuffer;
  switch(prefix->type)
  {
		case eSynchro:// попытка синхронизации
		{
      TIP_Port ip_port(ip,port);
			TInfoConnect* pFoundConnect = GetInfoConnect(ip_port);
			lockSendRcv();
			  pFoundConnect->cn_in_s  = 0;
			  pFoundConnect->cn_out_s = 0;
			  pFoundConnect->cn_in_p  = 0;
			  pFoundConnect->cn_out_p = 0;
			  SendCheck(prefix,ip,port);
			unlockSendRcv();
			break;
		}
    case eCheck:// подтверждение
			lockSendRcv();
				FindAndCheck(prefix,ip,port);
			unlockSendRcv();
      break;
    case ePacket:// пакет, требующий подтверждения
      SendCheck(prefix,ip,port);
			if(IsPacketFresh())
				NotifyRecv(INetTransport::ePacket, size);
      break;
    case eStream:// пакет, не требующий подтверждения
			if(IsStreamFresh())
        NotifyRecv(INetTransport::eStream, size);
      break;
    default:BL_FIX_BUG();;
  }
}
//----------------------------------------------------------------------------------
void TNetTransport::FindAndCheck(THeader* prefix,unsigned int ip,unsigned short port)
{
  // ищем в массиве пакетов, ожидающих подтверждение
	TBasePacketNetTransport oForSearchDefPacket;
	TBasePacketNetTransport *pForSearchDefPacket = &oForSearchDefPacket;
	TShortDescPacket infoData;
	infoData.ip_port_dst.Set(ip, port);
	infoData.size     = 0;
	pForSearchDefPacket->SetHeader(&infoData);
	int index = mArrWaitCheck.FastSearch(&pForSearchDefPacket,NULL,SortFreshDefPacket);
	if(index==-1)
  {
    mLogEvent.WriteF("FindAndCheck: index==-1\n");
    return;
  }
	//-------------------------------------------------------------------------------------
  int cnt = mArrWaitCheck.Count();
  for(int i = index ; i < cnt ; i++)
  {
	  TBasePacketNetTransport* pFoundDefPacket = (TBasePacketNetTransport*)mArrWaitCheck.Get(i);
    if(pFoundDefPacket->GetCnOut()==prefix->cn_out)
    {
      mArrWaitCheck.Delete(i);
      return;
    }
  }
	mLogEvent.WriteF("FindAndCheck() prefix->cn_out=%u\n",prefix->cn_out);
}
//----------------------------------------------------------------------------------
int TNetTransport::GetTimeout()
{
	int to = eTimeRefreshEngine;//###
	return to;
}
//----------------------------------------------------------------------------------
void TNetTransport::SendUnchecked()
{
	lockSendRcv();
  unsigned int now_ms = ht_GetMSCount();
	for(int i = 0 ; i < mArrWaitCheck.Count() ; i++)
	{
		TBasePacketNetTransport* pDefPacket = (TBasePacketNetTransport*)mArrWaitCheck.Get(i);
		if(pDefPacket)
		{
			if(pDefPacket->GetTime()+eTimeLivePacketDef<now_ms) 
			{
				TIP_Port ip_port;
				ip_port.ip   = pDefPacket->GetIP_dst();
				ip_port.port = pDefPacket->GetPort_dst();
				AddEventLostPacket(ip_port, pDefPacket->GetCntTry());
				if(Send(pDefPacket)==false)
				{
					Disconnect(&ip_port);
					i--;
				}
			}
		}
		else BL_FIX_BUG();
	}
	unlockSendRcv();
}
//----------------------------------------------------------------------------------
void TNetTransport::Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  switch(type)
  {
		case eRecv:
      mCallBackRecv.Register(pFunc);
      break;
		case eLostPacket:
			mCallBackLostPacket.Register(pFunc);
			break;
    case eDisconnect:
      mCallBackDisconnect.Register(pFunc);
      break;
  }
}
//----------------------------------------------------------------------------------
void TNetTransport::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  switch(type)
  {
		case eRecv:
			mCallBackRecv.Unregister(pFunc);
			break;
		case eLostPacket:
			mCallBackLostPacket.Unregister(pFunc);
			break;
		case eDisconnect:
			mCallBackDisconnect.Unregister(pFunc);
			break;
  }
}
//----------------------------------------------------------------------------------
bool TNetTransport::Send(TBasePacketNetTransport* pDefPacket)
{
  unsigned char cntTry = pDefPacket->GetCntTry();
  if((unsigned char)(cntTry+1)>eCntTryDef)
    return false;
  
  pDefPacket->SetCntTry(cntTry+1);  
  pDefPacket->SetTime(ht_GetMSCount());

  int size;
  unsigned int ip;
  unsigned short port;
  void* pData = pDefPacket->GetData(size,ip,port);
  return Write(pData,size,ip,port);
}
//----------------------------------------------------------------------------------
void TNetTransport::Disconnect(TIP_Port* ip_port)
{
  NotifyDisconnect(ip_port);
  // Gauss 04.02.2013
  // удалить пакеты, которые стоят в очереди на отправку и получение квитанции
  // ищем в массиве пакетов, ожидающих подтверждение
  // совпадение по IP и port
  SearchAndDelete(&mArrWaitCheck,ip_port);

  // удалить соединение из массива mArrConnect
  // эта задача лежит на уровне "Сессия"
}
//----------------------------------------------------------------------------------
void TNetTransport::SearchAndDelete(TArrayObject* pArr, TIP_Port* ip_port)
{
  TBasePacketNetTransport oForSearchDefPacket;
  TBasePacketNetTransport *pForSearchDefPacket = &oForSearchDefPacket;
  TShortDescPacket infoConnect;
  infoConnect.ip_port_dst = *ip_port;
  infoConnect.size        = 0;
  pForSearchDefPacket->SetHeader(&infoConnect);

  int index = pArr->FastSearch(&pForSearchDefPacket,NULL,SortFreshDefPacket);
  if(index==-1)// не нашли
  {
    //BL_FIX_BUG();
    return;
  }
  //----------------------------------------------
  // перебор всех значений, для которых ip и port
  for(int i = index ; i < pArr->Count() ; i++ )
  {
    TBasePacketNetTransport* pDesc = (TBasePacketNetTransport*)pArr->Get(i);
    if((pDesc->GetIP_dst()  !=ip_port->ip  )||
       (pDesc->GetPort_dst()!=ip_port->port))
      return;// прекратить поиск
    pArr->Delete(i);
    i--;
  }
}
//----------------------------------------------------------------------------------
void TNetTransport::NotifyRecv(eTypeRecv type, int size)
{
  TDescRecv descRecv;
  descRecv.ip_port  = ((THeader*)mBuffer)->ip_port_src;
  descRecv.data     = mBuffer + sizeof(THeader);
  descRecv.sizeData = size - sizeof(THeader);
	descRecv.type     = type;

  mCallBackRecv.Notify(&descRecv,sizeof(descRecv));
}
//----------------------------------------------------------------------------------
void TNetTransport::SendCheck(THeader* prefix,unsigned int ip,unsigned short port)
{
  THeader check;
  check.cn_in  = prefix->cn_in;
  check.cn_out = prefix->cn_out;
  check.type   = eCheck;
	check.cntTry = 0;
	check.time_ms = ht_GetMSCount();
	check.ip_port_dst.ip   = ip;
	check.ip_port_dst.port = port;
	check.ip_port_src.ip   = mUDP.portSetting().m_LocalHost;
	check.ip_port_src.port = mUDP.portSetting().m_LocalPort;

  Write(&check,sizeof(THeader),ip, port);
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
	LogTransportInfo((THeader*)p,size);
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
	LogTransportInfo((THeader*)mBuffer,size);
	mLogRcvSend.WriteF("\n");
}
//----------------------------------------------------------------------------------
void TNetTransport::LogTransportInfo(THeader* p,int size)
{
	unsigned int hour   = p->time_ms/(1000*3600);// перевод из мс в часы
	unsigned int minute = p->time_ms/(1000*60)-hour*60;// перевод из мс в минуты минус кол-во часов
	unsigned int second = p->time_ms/1000-minute*60-hour*3600;// перевод мс в секунды минускол-во часов и минут
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
	unsigned char* data   = (unsigned char*)p+sizeof(THeader);
	int sizeData = size-sizeof(THeader);
	for(int i = 0 ; i < sizeData ; i++)
		mLogRcvSend.WriteF("0x%X ",data[i]);
	mLogRcvSend.WriteF("\n");
}
//----------------------------------------------------------------------------------
bool TNetTransport::IsPacketFresh()
{
	THeader* p = (THeader*)mBuffer;
	TInfoConnect* pInfoConnect = GetInfoConnect(p->ip_port_src);
  //--------------------------------------------------------------------------------
  bool res = false;
  if(A_more_B(p->cn_out, pInfoConnect->cn_in_p))
  {
    res = true;
    if(p->cn_out==pInfoConnect->cn_in_p+1)
    {
      // ничего не делаем
    }
    else
    {
      // много больше
      // добавить в set пропущенные пакеты
      for(unsigned short i = pInfoConnect->cn_in_p+1; A_more_B(p->cn_out,i)  ; i++ )
        pInfoConnect->setIndexWaitRecv.insert(i);
    }
    pInfoConnect->cn_in_p = p->cn_out;
  }
  //--------------------------------------------------------------------------------
  else if(p->cn_out==pInfoConnect->cn_in_p)
  {
    // ничего не делаем
  }
  //--------------------------------------------------------------------------------
  else // меньше, проверить на вхождение в set
  {
    TInfoConnect::TSetUshort::iterator fit = pInfoConnect->setIndexWaitRecv.find(p->cn_out);
    if(fit!=pInfoConnect->setIndexWaitRecv.end())
    {
      // нашли
      res = true;
      pInfoConnect->setIndexWaitRecv.erase(fit);
    }
  }
  //--------------------------------------------------------------------------------
  return res;
}
//----------------------------------------------------------------------------------
bool TNetTransport::IsStreamFresh()
{
	THeader* p = (THeader*)mBuffer;
	TInfoConnect* pFoundFresh = GetInfoConnect(p->ip_port_src);

	if(A_more_B(p->cn_out, pFoundFresh->cn_in_s))
	{
		pFoundFresh->cn_in_s = p->cn_out;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool TNetTransport::A_more_B(unsigned short A, unsigned short B)
{
  return ::A_more_B_cycle<unsigned short>(A,B);
}
//----------------------------------------------------------------------------------
int TNetTransport::SortFreshInfoConnect(const void* p1, const void* p2)
{
  const TInfoConnect *s1 = *( const TInfoConnect **)p1;
  const TInfoConnect *s2 = *( const TInfoConnect **)p2;

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
  const TBasePacketNetTransport *s1 = *( const TBasePacketNetTransport **)p1;
  const TBasePacketNetTransport *s2 = *( const TBasePacketNetTransport **)p2;

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
TInfoConnect* TNetTransport::GetInfoConnect(TIP_Port& v)
{
  TInfoConnect fresh;
  TInfoConnect* pFresh = &fresh;
  pFresh->ip    = v.ip;
  pFresh->port  = v.port;

	lockArrConnect();
  TInfoConnect* pFoundFresh = (TInfoConnect*)mArrConnect.Get(mArrConnect.FastSearch(&pFresh,NULL,SortFreshInfoConnect));
	unlockArrConnect();
  if(pFoundFresh==NULL)
  {
    pFresh = new TInfoConnect;
    pFresh->ip    = v.ip;
    pFresh->port  = v.port;

    lockArrConnect();
    mArrConnect.Add(pFresh);
		unlockArrConnect();
    return pFresh;
  }

	return pFoundFresh;
}
//----------------------------------------------------------------------------------
bool TNetTransport::Synchro(unsigned int ip, unsigned short port)
{
	if(IsActive())
	{
		BL_MessageBug("Нельзя вызывать Synchro() после вызова Start().");
		return false;
	}

  TIP_Port ip_port(ip, port);
	TInfoConnect* pInfoConnect = GetInfoConnect(ip_port);
	pInfoConnect->cn_in_s = 0;
	pInfoConnect->cn_out_s = 0;
	pInfoConnect->cn_in_p = 0;
	pInfoConnect->cn_out_p = 0;

  if(!mUDP.isOpen()){BL_FIX_BUG();return false;}
  
  unsigned int now_ms = ht_GetMSCount();
  unsigned int start_ms = now_ms;
  int cntTry = 0;
	unsigned int time_send = 0;
  while((start_ms+eWaitSynchro*1000)>now_ms)
  {
		if(now_ms>time_send+eTimeLivePacketDef)
		{
			time_send	= ht_GetMSCount();
			if(!SendSynchro(ip,port,cntTry)) {mUDP.close();return false;}
		  if(cntTry>eCntTryDef) 
        return false;
      cntTry++;
		}
    
    int res = mUDP.read(mBuffer,eSizeBuffer,(eWaitSynchro*1000)/eCntTryDef, ip, port);
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
        THeader* prefix = (THeader*)mBuffer;
        if(prefix->type  ==eCheck)
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
  THeader synchro;
  synchro.cn_out = 0; // в начале ЦН=0
  synchro.cn_in  = 0; // в начале ЦН=0
  synchro.type   = eSynchro;
  synchro.cntTry = cntTry;
	synchro.time_ms = ht_GetMSCount();
  synchro.ip_port_dst.ip   = ip;
  synchro.ip_port_dst.port = port;
  synchro.ip_port_src.ip   = mUDP.portSetting().m_LocalHost;
  synchro.ip_port_src.port = mUDP.portSetting().m_LocalPort;

  return Write(&synchro,sizeof(THeader),ip, port);
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
void TNetTransport::SearchAndDeleteConnect(TIP_Port* ip_port)
{
  TInfoConnect infoConnect;
  infoConnect.ip    = ip_port->ip;
  infoConnect.port  = ip_port->port;
  TInfoConnect* pInfoConnect = &infoConnect;

  lockArrConnect();
  unsigned int indexFound = mArrConnect.FastSearch(&pInfoConnect,NULL,SortFreshInfoConnect);
  if(indexFound!=-1)
    mArrConnect.Delete(indexFound);  
  unlockArrConnect();
  BL_ASSERT(indexFound!=-1);
}
//----------------------------------------------------------------------------------
void TNetTransport::SetupBufferForSocket()
{
  bool resSetRecv = mUDP.SetRecvBuffer(eSizeBufferForRecv);
  bool resSetSend = mUDP.SetSendBuffer(eSizeBufferForSend);
  BL_ASSERT(resSetRecv&resSetSend);
}
//----------------------------------------------------------------------------------
void TNetTransport::SetTimeOutPacket( int t_ms)
{
	mTimeOut = t_ms;
}
//----------------------------------------------------------------------------------
void TNetTransport::SetCntTry( int c)
{
	mCntTry = c;
}
//----------------------------------------------------------------------------------
void TNetTransport::AddEventLostPacket( TIP_Port& ip_port, unsigned char c)
{
	mListLostPacket.push_back(TLostPacket(ip_port,c));
}
//----------------------------------------------------------------------------------
void TNetTransport::NotifyLostPacket()
{
	TListLPIt bit = mListLostPacket.begin();
	TListLPIt eit = mListLostPacket.end();
	while(bit!=eit)
	{
		mCallBackLostPacket.Notify((void*)&(*bit), sizeof(TLostPacket));
		bit++;
	}
	mListLostPacket.clear();
}
//----------------------------------------------------------------------------------
