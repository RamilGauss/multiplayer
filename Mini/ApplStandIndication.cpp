#include "ApplStandIndication.h"
#include <stdio.h>

#ifdef WIN32
  #include "HiTimer.h"
  #define usleep ht_usleep
#else
#endif


#ifdef WIN32
#else
  #include <iostream.h>
  #include <termios.h>
  #include <unistd.h>
  #include <sys/ioctl.h>
  #include <asm/ioctls.h>
  #include <sys/time.h>
#endif

#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


using namespace nsStandIndication;
using namespace std;

#define PRINTF_ASI mLog.WriteF_time

static TApplStandIndication* pApplStandIndication = NULL; 

#ifdef WIN32
  static GCS mutex_ApplSI;
  void m_lock(){mutex_ApplSI.lock();}
  void m_unlock(){mutex_ApplSI.unlock();}
#else
  static Mutex mutex_ApplSI;
  void m_lock(){mutex_ApplSI.begin();}
  void m_unlock(){mutex_ApplSI.end();}
#endif

void FromTransport_Answer(void* pData, int size)
{
	m_lock();
	//----------------------------------------	
	if(pApplStandIndication)
	{
		pApplStandIndication->Answer(pData, size);
	}
	//----------------------------------------
	m_unlock();
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
TApplStandIndication::TApplStandIndication()
{
	mLog.SetPrintf(true);
	mLog.SetBufferization(false);
	//mLog.SetEnable(false);	// выключить отладку

	std::vector<char> sSignaturaIn;
	sSignaturaIn.push_back(eGetVersion);	

  std::vector<char> sSignaturaOut;
	sSignaturaOut.push_back(eAnswerVersion);	
  TPacketAnswerVersion answerVersion;
	sSignaturaOut.push_back(answerVersion.version);	

	mSI.SetSignatura(&sSignaturaIn,&sSignaturaOut);
	
	Init();
}
//--------------------------------------------------------------------
TApplStandIndication::~TApplStandIndication()
{
	Done();
}
//--------------------------------------------------------------------
bool TApplStandIndication::Send( void* packet, int sizePacket, int type)
{
  flgRecvPacket = false;
	
	bool res = false;
	if(TryConnect())
	{
		// упаковка в пакет 
		TPacketApplLevel_SI packetAppl;
		packetAppl.SetData(packet,sizePacket,type);
		//-----------------------------------
		int size    = packetAppl.GetSizePacket();
		void* pData = packetAppl.GetPacket();
	  for(int i = 0 ; i < TStandIndication::eTryCount ; i++ )
		{
			mSI.Send(pData,size);
			usleep(TStandIndication::eTimeRequest);
			if(flgRecvPacket)
			{
				res = true;
			  break;
			}
		}
		//-----------------------------------		
		if(res==false) mSI.Stop();// разрыв соединения
	}
	return res;
}
//--------------------------------------------------------------------
bool TApplStandIndication::SendRequest(void* pOut, int sizeOut, int type)
{
	bool res = Send(NULL,0,type);
	if(res)
	{
		if(sizeOut==mPacketRecv.GetSizeData())
			memcpy(pOut,mPacketRecv.GetData(),sizeOut);
		else
			res = false;
	}
	return res;
}
//--------------------------------------------------------------------
bool TApplStandIndication::Set( TPacketSet& pIn)// sync
{
	if(Send((void*)&pIn,sizeof(pIn),eSet))
	{
		if(memcmp(&pIn,mPacketRecv.GetData(),sizeof(pIn))==0)
			return true;
	}
	return false;
}
//--------------------------------------------------------------------
bool TApplStandIndication::GetPRD(TPacketAnswerPrd& pOut)	// sync
{
	return SendRequest(&pOut, sizeof(pOut),eGetPrd);
}
//--------------------------------------------------------------------
bool TApplStandIndication::GetState(TPacketAnswerState& pOut)// sync
{
	return SendRequest(&pOut, sizeof(pOut),eGetState);
}
//--------------------------------------------------------------------
bool TApplStandIndication::GetStateSystemCool(TPacketAnswerStateSystemCool& pOut)			 // sync
{
	return SendRequest(&pOut, sizeof(pOut),eGetStateSystemCool);
}
//--------------------------------------------------------------------
/*void TApplStandIndication::Register(TCallBackRegistrator::TCallBackFunc callBack)
{
	mCallBackRegistratorAnswer.Register(callBack);
}
//--------------------------------------------------------------------
void TApplStandIndication::Unregister(TCallBackRegistrator::TCallBackFunc callBack)
{
	mCallBackRegistratorAnswer.Unregister(callBack);
}
//--------------------------------------------------------------------
void TApplStandIndication::NotifyAnswer(void* pData, int size)
{
	mCallBackRegistratorAnswer.Notify(pData, size);
}*/
//--------------------------------------------------------------------
void TApplStandIndication::Init()
{
	m_lock();
	//--------------------------------------------------------
	pApplStandIndication = this;
  mSI.Register(FromTransport_Answer);
	//--------------------------------------------------------	
	m_unlock();
}
//--------------------------------------------------------------------
void TApplStandIndication::Done()
{
	m_lock();
	//--------------------------------------------------------
  mSI.Unregister(FromTransport_Answer);
	pApplStandIndication = NULL;  
	//--------------------------------------------------------	
	m_unlock();
}
//--------------------------------------------------------------------
void TApplStandIndication::Answer(void* pData, int size)
{
	mPacketRecv.SetPacket(pData, size);
	//----------------------------------------------------------------
	PRINTF_ASI("TApplStandIndication::Answer(0x%X,%d) type=0x%X\n",mPacketRecv.GetData(), mPacketRecv.GetSizeData(), mPacketRecv.GetType());	
	flgRecvPacket = true;
}
//--------------------------------------------------------------------
bool TApplStandIndication::TryConnect()
{
	bool res = true;
	if(mSI.IsActive()==false)
		res = mSI.Start();

	return res;
}
//--------------------------------------------------------------------
