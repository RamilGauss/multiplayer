#include "StandIndication.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <algorithm>

#ifdef WIN32
  #include "HiTimer.h"
  #define usleep ht_usleep
#else
  #include <iostream.h>
  #include <unistd.h>
#endif


#ifdef WIN32
  #define WRITE_COMPORT mDevice.write
  #define READ_COMPORT mDevice.read
  #define SETUP_COMPORT mDevice.setPortState
#else
  #define WRITE_COMPORT mDevice.wr
  #define READ_COMPORT mDevice.rd
  #define SETUP_COMPORT mDevice.SetRS232State
#endif



using namespace nsStandIndication;
using namespace std;

#define PRINTF_SI mLog.WriteF_time

TStandIndication::TStandIndication()
{
	mLog.SetPrintf(true);
	mLog.SetBufferization(false);
	//mLog.SetEnable(false);	// ��������� �������

	flgActive   = false;
	flgNeedStop = false;	
	mSizeRead   = 0;
	mSizeRest   = 0;

	Init();
}
//--------------------------------------------------------------------------------
TStandIndication::~TStandIndication()
{
	Stop();
  ClearListPacket();
}
//--------------------------------------------------------------------------------
void TStandIndication::SetSignatura(TVectorChar* sSignaturaIn, TVectorChar* sSignaturaOut)
{
	mSignaturaIn  = *sSignaturaIn;
	mSignaturaOut = *sSignaturaOut;
}
//--------------------------------------------------------------------------------
bool TStandIndication::Send(void* pData, int size)
{
	// ��������� � ����� ������������� ������
	TPacketTransportLevel_SI packet;
	packet.SetData((unsigned char*)pData,size);
	const char* pP = packet.GetPacket(size);
	//return mDevice.wr((void*)pP, size);
  return  WRITE_COMPORT((void*)pP, size);
}
//--------------------------------------------------------------------------------
void TStandIndication::Register(TCallBackRegistrator::TCallBackFunc callBack)
{
	mCallBackRegistratorAnswer.Register(callBack);
}
//--------------------------------------------------------------------------------
void TStandIndication::Unregister(TCallBackRegistrator::TCallBackFunc callBack)
{
	mCallBackRegistratorAnswer.Unregister(callBack);
}
//--------------------------------------------------------------------------------
void TStandIndication::NotifyAnswer(TPacketTransportLevel_SI& packet)
{
	int sizeData;
	const char* pData = packet.GetData(sizeData);
	mCallBackRegistratorAnswer.Notify((void*)pData,sizeData);
}
//--------------------------------------------------------------------------------
void* ThreadSI(void* p)
{
	((TStandIndication*)p)->Engine();
	return NULL;
}
//--------------------------------------------------------------------------------
void TStandIndication::StartThread()
{
	int res; 
#ifdef WIN32
    thread_id = g_thread_create(ThreadSI,(gpointer)this, true,  NULL);
    res = (thread_id!=NULL);
#else
    res = pthread_create( &thread_id, NULL, ThreadSI, this );
#endif
	if(res<0)
		PRINTF_SI(strerror(errno));
	else
		PRINTF_SI("SI: StartThread OK\n");
}
//--------------------------------------------------------------------------------
void TStandIndication::StopThread()
{
	if(flgActive)
		PRINTF_SI("SI: StopThread OK\n");
		
	flgNeedStop = true;
	while(flgActive)
		usleep(eTimeWaitStopThread);
}
//--------------------------------------------------------------------------------
void TStandIndication::Engine()
{
	flgActive = true;
	flgNeedStop = false;
	while(flgNeedStop==false)
	{
		PRINTF_SI("SI: engine work\n");
		
		if(Read())
			SearchPacket();
		
		usleep(eTimeSleepEngineThread*1000);
	}
	flgActive = false;
}
//--------------------------------------------------------------------------------
bool TStandIndication::Read()
{
  mSizeRead = 0;
#ifdef WIN32
  int res = READ_COMPORT(&mBufferRead[mSizeRest],mSizeRead,eTimeSleepEngineThread);
  return res==TComPort::rr_Read;
#else
	return mDevice.rd(&mBufferRead[mSizeRest],mSizeRead,eTimeSleepEngineThread);
#endif
}
//--------------------------------------------------------------------------------
void TStandIndication::SearchPacket()
{
	TPacketTransportLevel_SI* pPacket = new TPacketTransportLevel_SI;
	for(unsigned long i = 0 ; i < mSizeRest+mSizeRead ; i++ )
	{
		int res = pPacket->SetPacket((unsigned char*)&mBufferRead[i], 
		mSizeRead/*�� ��� ��������� ������*/ + mSizeRest/*+ �� ��� ������ �� �����*/ - i);
		switch(res)
		{
			case TPacketTransportLevel_SI::eNotFoundHeader:// ��� ���������
			case TPacketTransportLevel_SI::ePacketBreak:   // ����� ���������
			  // ���� ������
				break;
			case TPacketTransportLevel_SI::eNormal:        // ����� ������
				// ���� ������
				// �������� �� ������ ������
				i += pPacket->GetSizePacket() - 1/*�.�. i++*/;
        
				// �������� ����� ������
				NotifyAnswer(*pPacket);// ??? Gauss 28.02.2013? ������ ����� ���� ������������ ������
        mListReadPacket.push_back(pPacket);      
        pPacket = new TPacketTransportLevel_SI;
				break;
			case TPacketTransportLevel_SI::eSmallSize:     // ���� ������
			case TPacketTransportLevel_SI::eNotFoundEnd:   // ��� ��������� 
			  // �������� � ������ ������ � ����� �� �����
				mSizeRest = mSizeRest + mSizeRead - i;
				memmove(&mBufferRead[0],&mBufferRead[i],mSizeRest);
				return;
			case TPacketTransportLevel_SI::eCRC_fail:        // ����������� ����� �� ���������
				break;
			case TPacketTransportLevel_SI::eSizeMoreMax:     // �� ��� ������ ����� ������, � ���������� ����� ������� �����
				break;
			default:;
		}
	}
	mSizeRest = 0;
  delete pPacket;
}
//--------------------------------------------------------------------------------
bool TStandIndication::Open(int port)// ����� �� ������ �������� port???
{
	PRINTF_SI("TStandIndication::Open \n");
	bool ret = false;
	int iPort = port;
	int cnt   = 1;
  if(port==-1)
	{
		iPort = 1;
		cnt = eCntComPort;
	}
	// ����������� ��� ���-�����
	for(; iPort < cnt ; iPort++)
	{
		ret = TryOpenAndReadSignatura(iPort);
		if(ret)
			return ret;// �����!
	}
  return ret;
}
//------------------------------------------------------------------------------
bool TStandIndication::Start(int port)// sync
{
	Stop();
	bool res = Open(port);
	if(res)
		StartThread();
	return res;
}
//------------------------------------------------------------------------------
void TStandIndication::Stop()// sync
{
	StopThread();
	mDevice.close();
}
//------------------------------------------------------------------------------
bool TStandIndication::IsActive()
{
	return flgActive;
}
//------------------------------------------------------------------------------
void TStandIndication::Init()
{

}
//------------------------------------------------------------------------------
bool TStandIndication::TryOpenAndReadSignatura(int port)
{
	bool res = mDevice.open(port);
	if(res)
	{
		PRINTF_SI("TStandIndication::TryOpenAndReadSignatura(%d) OK\n",port);
		//mDevice.SetRS232State(
    SETUP_COMPORT(
			eBaudRate, // ��������, ��� � �������
			eByteSize, // ���-�� �������������� ��� � �������( 8 ��� 7 )
			eParity,   // 0 - ��� �������� ��������, 1 - �������� ��������, 2 - �������� ����������
			eStopBits, // ���-�� �������� ���: 0 - 1 ���, 1 - 2 ����
			eIsCTR_RTS // ���. ���������� ���������� �������
			);    

		for(int i = 0 ; i < eCntTryReadSignatura ; i++)
		{
			// ����� ��
			// �������� ���������
			res = WriteSignatura();
			if(res==false) 
				continue; // ����������� ��� ���
			// ���� �����
			usleep(eTimeWaitSignatura*1000); // ���
			res = ReadSignatura();
			if(res) 
			{
				PRINTF_SI("TStandIndication::TryOpenAndReadSignatura(%d) found SI!\n",port);
				return res;
			}
		}
	}
	else
		PRINTF_SI("TStandIndication::TryOpenAndReadSignatura(%d) FAIL!!!\n",port);
	mDevice.close();
	return res;
}
//------------------------------------------------------------------------------
bool TStandIndication::WriteSignatura()
{
	return Send((void*)&mSignaturaIn[0], mSignaturaIn.size());
}
//------------------------------------------------------------------------------
bool TStandIndication::ReadSignatura()
{
	bool res = Read();
	if(res)
  {
    SearchPacket();// �������� ������ ������ ��������
    // �������� ��� �������� � ��� ��� ������ ���� ��������
    res = false;
    if(mListReadPacket.size()>0)
    {
      TListPtr::iterator bit = mListReadPacket.begin();
      TListPtr::iterator eit = mListReadPacket.end(); 
      while(bit!=eit)
      {
        if(CompareSignatura(*bit)!=NULL) 
        {
          res = true;
          break;
        }
        bit++;
      }
      ClearListPacket();
    }
  }

	return res;
}
//------------------------------------------------------------------------------
template<class Type>
void Delete(Type& elem)
{
  delete elem;
}
//------------------------------------------------------------------------------
void TStandIndication::ClearListPacket()
{
  for_each(mListReadPacket.begin(), mListReadPacket.end(), Delete<TPacketTransportLevel_SI*> );
  mListReadPacket.clear();
}
//------------------------------------------------------------------------------
bool TStandIndication::CompareSignatura(TPacketTransportLevel_SI* pPacket)
{
  // 1. �� �������
  int sizeP;
  const char* p = pPacket->GetData(sizeP);
  if(sizeP!=mSignaturaOut.size())
    return false;
  // 2. �� �����������
  if(memcmp(p,&mSignaturaOut[0],sizeP)!=0)
    return false;

  return true;
}
//------------------------------------------------------------------------------

