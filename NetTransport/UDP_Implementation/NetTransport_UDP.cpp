/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

/*
  03.07.2103:
    Gauss: замена TArrayObject на std::map, но не факт что будет работать (не проверял)
  04.02.2013: 
    Gauss: Add:     queue for Write.
           Fix bug: delete desc_packet from arrWaitSend and arrWaitCheck when disconnect.
           Add:     delete desc_packet when recv packet's check.
*/


#include "NetTransport_UDP.h"

#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>

#include "common_defs.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "Logger.h"

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
TNetTransport_UDP::TNetTransport_UDP()
{
  flgActive = false;

	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->SetPrintf(false);
#ifndef FULL_DEBUG_TRANSPORT
  GetLogger()->Get(STR_NAME_NET_TRANSPORT)->SetEnable(false);
#endif
}
//----------------------------------------------------------------------------------
TNetTransport_UDP::~TNetTransport_UDP()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport_UDP::Open(unsigned short port, unsigned char numNetWork)
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

  // настройка размеров буфера под прием и отсылку
  SetupBufferForSocket();
  return true;
}
//----------------------------------------------------------------------------------
// Gauss 04.05.2013
void TNetTransport_UDP::Send(unsigned int ip, unsigned short port, 
                             TBreakPacket packet, bool check)
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
  
  lockSendRcv();
	//-------------------------------------------------
  TInfoConnect* pInfoConnect = GetInfoConnect(data.ip_port_dst);
  if(check)
  {
		pPacket->SetType(ePacket);

    pInfoConnect->cn_out_p++;
		pPacket->SetCnOut(pInfoConnect->cn_out_p);
		pPacket->SetCnIn(pInfoConnect->cn_in_p);//вообще по фигу но для отладки

    mMultiMapWaitCheck.insert(TMultiMapIP_Ptr::value_type(data.ip_port_dst,pPacket));
		Send(pPacket);

    unlockSendRcv();
    //-------------------------------------------------
    return;
  }
	pInfoConnect->cn_out_s++;
	pPacket->SetCnOut(pInfoConnect->cn_out_s);
	pPacket->SetCnIn(pInfoConnect->cn_in_s);//вообще по фигу но для отладки

  unlockSendRcv();
  //-------------------------------------------------
	pPacket->SetType(eStream);
  bool resSend = Send(pPacket);
  BL_ASSERT(resSend);

	delete pPacket;
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::Engine()
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
void TNetTransport_UDP::Start()
{
  boost::thread work_thread(boost::bind(&TNetTransport_UDP::Engine, this));

	while(IsActive()==false)
		ht_msleep(eFeedBack);
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::Stop()
{
	flgNeedStop = true;
	while(IsActive())
		ht_msleep(eFeedBack);

	mUDP.close();
}
//----------------------------------------------------------------------------------
bool TNetTransport_UDP::IsActive()
{
	return flgActive;
}
//--------------------------------------------------------------------------
void TNetTransport_UDP::AnalizPacket(unsigned int ip,unsigned short port,int size)
{
  THeader* prefix = (THeader*)mBuffer;
  switch(prefix->type)
  {
		case eSynchro:// попытка синхронизации
		{
      TIP_Port ip_port(ip,port);
			lockSendRcv();
        TInfoConnect* pFoundConnect = GetInfoConnect(ip_port);
			  pFoundConnect->cn_in_s  = 0;
			  pFoundConnect->cn_out_s = 0;
			  pFoundConnect->cn_in_p  = 0;
			  pFoundConnect->cn_out_p = 0;
			unlockSendRcv();
      SendCheck(prefix,ip,port);
			break;
		}
    case eCheck:// подтверждение
			lockSendRcv();
				FindAndCheck(prefix,ip,port);
			unlockSendRcv();
      break;
    case ePacket:// пакет, требующий подтверждения
    {
      SendCheck(prefix,ip,port);
      lockSendRcv();
        bool resFresh = IsPacketFresh();
      unlockSendRcv();
      if(resFresh)
        NotifyRecv(INetTransport::ePacket, size);
    }
      break;
    case eStream:// пакет, не требующий подтверждения
    {
      lockSendRcv();
        bool resFresh = IsStreamFresh();
      unlockSendRcv();
      if(resFresh)
        NotifyRecv(INetTransport::eStream, size);
    }
      break;
    default:BL_FIX_BUG();;
  }
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::FindAndCheck(THeader* prefix, unsigned int ip, unsigned short port)
{
  // ищем в массиве пакетов, ожидающих подтверждение
  TIP_Port ip_port(ip, port);
  TMultiMapIP_Ptr_It fit = mMultiMapWaitCheck.lower_bound(ip_port);
  if(fit==mMultiMapWaitCheck.end())
  {
    GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("FindAndCheck: index==-1\n");
    return;
  }
  //-------------------------------------------------------------------------------------
  while(fit!=mMultiMapWaitCheck.end())
  {
    if(fit->second->GetCnOut()==prefix->cn_out)
    {
      mMultiMapWaitCheck.erase(fit);
      return;
    }
    fit++;
  }
  GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("FindAndCheck() prefix->cn_out=%u\n",prefix->cn_out);
}
//----------------------------------------------------------------------------------
int TNetTransport_UDP::GetTimeout()
{
	int to = eTimeRefreshEngine;
	return to;
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::SendUnchecked()
{
  set<TIP_Port> setIP;
	lockSendRcv();
  unsigned int now_ms = ht_GetMSCount();
  BOOST_FOREACH(TMultiMapIP_Ptr::value_type& bit,mMultiMapWaitCheck)
  {
    TBasePacketNetTransport* pDefPacket = bit.second;
    if(pDefPacket)
    {
      if(pDefPacket->GetTime() + eTimeLivePacket < now_ms) 
      {
        if(Send(pDefPacket)==false)
        {
          TIP_Port ip_port;
          ip_port.ip   = pDefPacket->GetIP_dst();
          ip_port.port = pDefPacket->GetPort_dst();
          NotifyDisconnect(&ip_port);
          setIP.insert(ip_port);
        }
      }
    }
    else BL_FIX_BUG();
  }
  // отложенное удаление
  SearchAndDeleteInMapWaitCheck(setIP);
	unlockSendRcv();
}
//----------------------------------------------------------------------------------
bool TNetTransport_UDP::Send(TBasePacketNetTransport* pDefPacket)
{
  unsigned char cntTry = pDefPacket->GetCntTry();
  if((unsigned char)(cntTry+1)>eCntTry)
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
void TNetTransport_UDP::SearchAndDeleteInMapWaitCheck(set<TIP_Port>& setIP_port)
{
  BOOST_FOREACH(set<TIP_Port>::value_type& bit, setIP_port)
    mMultiMapWaitCheck.erase(bit);
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::NotifyRecv(eTypeRecv type, int size)
{
  TDescRecv descRecv;
  descRecv.ip_port  = ((THeader*)mBuffer)->ip_port_src;
  descRecv.data     = mBuffer + sizeof(THeader);
  descRecv.sizeData = size - sizeof(THeader);
	descRecv.type     = type;

  NotifyReceive(&descRecv);
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::SendCheck(THeader* prefix,unsigned int ip,unsigned short port)
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
bool TNetTransport_UDP::Write(void *p, int size, unsigned int ip, unsigned short port)
{
	WriteLog(p, size, ip, port);
	return mUDP.write(p, size, ip, port);
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::WriteLog(void *p, int size, unsigned int ip, unsigned short port)
{
  DEBUG_PACKET_ONLY_WRITE

	DEBUG_LOG
	char * sIP = ns_str_addr(ip);
	if(sIP==NULL) {BL_FIX_BUG();return;}
	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("Send Packet to ip=%s, port=%u\n",sIP,port);
	LogTransportInfo((THeader*)p,size);
	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("\n");
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::ReadLog(int size, unsigned int ip, unsigned short port)
{
  DEBUG_PACKET_ONLY_READ

  DEBUG_LOG
	char * sIP = ns_str_addr(ip);
	if(sIP==NULL) {BL_FIX_BUG();return;}
	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("Recive Packet from ip=%s, port=%u\n",sIP,port);
	LogTransportInfo((THeader*)mBuffer,size);
	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("\n");
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::LogTransportInfo(THeader* p, int size)
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

	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("TransportInfo: \n\t type=%s,\n\t cn_in=%u,\n\t cn_out=%u,\n\t time=%u:%u:%u.%03u,\n\t localTime=%u:%u:%u.%03u,\n\t cntTry=%u,\n\t DST=%s:%u,\n\t SRC=%s:%u\n",
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
	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("Contain packet:\n");
	unsigned char* data   = (unsigned char*)p+sizeof(THeader);
	int sizeData = size-sizeof(THeader);
	for(int i = 0 ; i < sizeData ; i++)
		GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("0x%X ",data[i]);
	GetLogger()->Get(STR_NAME_NET_TRANSPORT)->WriteF("\n");
}
//----------------------------------------------------------------------------------
bool TNetTransport_UDP::IsPacketFresh()
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
bool TNetTransport_UDP::IsStreamFresh()
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
bool TNetTransport_UDP::A_more_B(unsigned short A, unsigned short B)
{
  return ::A_more_B_cycle<unsigned short>(A,B);
}
//----------------------------------------------------------------------------------
TInfoConnect* TNetTransport_UDP::GetInfoConnect(TIP_Port& v)
{
  TMapIP_Ptr_It fit = mMapInfoConnect.find(v);
  if(fit==mMapInfoConnect.end())
  {
    mMapInfoConnect.insert(TMapIP_Ptr::value_type(v,new TInfoConnect));
    fit = mMapInfoConnect.find(v);
  }
  return fit->second;
}
//----------------------------------------------------------------------------------
bool TNetTransport_UDP::Connect(unsigned int ip, unsigned short port)
{
	if(IsActive())
	{
		BL_MessageBug("Нельзя вызывать Connect() после вызова Start().");
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
        THeader* prefix = (THeader*)mBuffer;
        if(prefix->type==eCheck)
          if(prefix->cn_in==0)
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
bool TNetTransport_UDP::SendSynchro(unsigned int ip, unsigned short port, int cntTry)
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
void TNetTransport_UDP::Lock(void* pLocker)
{
  GCS* pGCS = (GCS*)pLocker;
  pGCS->lock();
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::Unlock(void* pLocker)
{
  GCS* pGCS = (GCS*)pLocker;
  pGCS->unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::SetupBufferForSocket()
{
  bool resSetRecv = mUDP.SetRecvBuffer(eSizeBufferForRecv);
  bool resSetSend = mUDP.SetSendBuffer(eSizeBufferForSend);
  BL_ASSERT(resSetRecv&resSetSend);
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::NotifyReceive(TDescRecv* p)  
{
  mCallBackEventRecieve.Notify(p);
}
//----------------------------------------------------------------------------------
void TNetTransport_UDP::NotifyDisconnect(TIP_Port* p)
{
  mCallBackEventDisconnect.Notify(p);
}
//----------------------------------------------------------------------------------
