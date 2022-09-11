/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerSession.h"
#include "Logger.h"
#include "Base.h"

#include <boost/foreach.hpp>

//#define NOT_USE_CRYPT_TCP

using namespace std;
using namespace nsMMOEngine;

TManagerSession::TManagerSession()
{
  CleanFlagsForWaitUp();

#ifndef NOT_USE_CRYPT_TCP
  SetUseCryptTCP(true);
#else
  SetUseCryptTCP(false);
#endif
  mNavigateSession = new TNavigateSession;
  mMngTransport = new TManagerTransport(this);

	flgStart = false;
  mTimeLiveSession  = eDefTimeLive;
  mLastID_Session   = 0;
}
//--------------------------------------------------------------------------------------------
TManagerSession::~TManagerSession()
{
  // ������� ���������� ������, ����� ������� �� ����������� ��������� ����������
  lockAccessSession();
  delete mNavigateSession;
  mNavigateSession = NULL;// �� ����� � ������ �������
  unlockAccessSession();

  delete mMngTransport;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::SetMakerTransport(IMakerTransport* pMakerTransport)
{
	mMngTransport->SetTransport(pMakerTransport);
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::Start(TDescOpen* pDesc, int count)
{
	if(flgStart)
	{
		GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() restart.\n");
		BL_FIX_BUG();
		return false;
	}
	flgStart = true;

	for(int i = 0 ; i < count ; i++ )
		if(StartTransport(pDesc[i].port, pDesc[i].subNet)==false)
			return false;
	return true;
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::StartTransport(unsigned short port, unsigned char subNet)
{
	INetTransport* pTransport = mMngTransport->Add(subNet);
  bool resOpen = pTransport->Open(port,subNet);
  if(resOpen==false) 
  {
    GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("TManagerSession::Start() open port %u FAIL.\n", port);
    BL_FIX_BUG();
    return false;
  }
  // ����� ������ ������
  pTransport->Start();
  return resOpen;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Work()
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  mNavigateSession->Work();
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Send(unsigned int id_session, TBreakPacket bp, bool check)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(id_session);
  if(pSession) 
    Send(pSession, bp, check);

  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
unsigned int TManagerSession::Send(unsigned int ip, unsigned short port, TBreakPacket bp, unsigned char subNet, bool check)
{
  lockConnectUp();

  INetTransport* pTransport = mMngTransport->FindBySubNet(subNet);
	if(pTransport==NULL)
  {
    unlockConnectUp();
    return INVALID_HANDLE_SESSION;
  }

  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    unlockConnectUp();
    return INVALID_HANDLE_SESSION;
  }
  //===================================================================
  // ����������� � ��������
  if(pTransport->Connect(ip, port)==false) 
  {
    unlockAccessSession();
    unlockConnectUp();
    BL_FIX_BUG();
    return INVALID_HANDLE_SESSION;// ��� ������ ��������������� �����
  }
  mIP_PortUp.Set(ip,port);  // ��������� ��������� �������� ����������
 
  TSession* pSession = mNavigateSession->FindSessionByIP(mIP_PortUp);
  if(pSession==NULL)
    pSession = NewSession(mIP_PortUp, pTransport);
  else
  {
    unlockAccessSession();
    unlockConnectUp();
    GetLogger(STR_NAME_MMO_ENGINE)->
      WriteF_time("TManagerSession::Send(0x%X,%u) sending to IP with exist session.\n", ip, port);
    BL_FIX_BUG();
		return INVALID_HANDLE_SESSION;
  }
  unsigned int id_session = pSession->GetID();
  // ������� ������� �� AES ����
	SendKeyRSA_Up(pSession);

  unlockAccessSession();
  //===================================================================
  // ���� ������
  bool res = WaitAnswerFromUp();
  CleanFlagsForWaitUp();
  if(res==false)
  {
    unlockConnectUp();
		GetLogger(STR_NAME_MMO_ENGINE)->
			WriteF_time("Wait Answer From Up don't recv answer.\n");
    return INVALID_HANDLE_SESSION;
  }
  //===================================================================
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    // ��������� ������ ����������
    unlockAccessSession();
    unlockConnectUp();
    return INVALID_HANDLE_SESSION;
  }
  // �������� ������ ���� �������, ���� ����� ������
  pSession = mNavigateSession->FindSessionByID(id_session);
  if(pSession)
    Send(pSession, bp, check);
  else
    id_session = INVALID_HANDLE_SESSION;

  unlockConnectUp();
  unlockAccessSession();
  return id_session;
}
//--------------------------------------------------------------------------------------------
unsigned int TManagerSession::GetSessionID(unsigned int ip, unsigned short port)
{
  unsigned int id = INVALID_HANDLE_SESSION;
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return id;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByIP(TIP_Port(ip,port));
  if(pSession)
    id = pSession->GetID();
  unlockAccessSession();
  return id;
}
//--------------------------------------------------------------------------------------------
void TManagerSession::CloseSession(unsigned int ID_Session)
{
  if(ID_Session==INVALID_HANDLE_SESSION) 
    return;

  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(ID_Session);
  if(pSession)
  {
    TIP_Port ip_port;
    pSession->GetInfo(ip_port);
    mMngCtxCrypto.Close(ip_port);

    mNavigateSession->Delete(pSession);
  }
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Recv( INetTransport::TDescRecv* pDescRecv, INetTransport* pTransport)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  // ���������� ����� ������ ��� ���
  TSession* pSession = mNavigateSession->FindSessionByIP(pDescRecv->ip_port);
  if(pSession==NULL)
    pSession = NewSession(pDescRecv->ip_port, pTransport);
  else
    pSession->Recv();// ��������� ������ � ������
  unsigned int id = pSession->GetID();
  //-----------------------------------------------
  TDescRecvSession descRecvSession;
  *((INetTransport::TDescRecv*)&descRecvSession) = *pDescRecv;
  descRecvSession.id_session = id;
  // ������, ��������� �� ������ �������� ���������, ������ ��� ������������
  TSession::THeader* pHeader = (TSession::THeader*)descRecvSession.data;
  descRecvSession.use_crypt  = pHeader->use_crypt;
  switch(pHeader->type)
  {
    case TSession::eEcho:
      break;
    case TSession::ePacket:
      RecvPacket(descRecvSession, pSession);
      break;
    case TSession::eKeyRSA:
      RecvKeyRSA(descRecvSession, pSession);
      break;
    case TSession::eKeyAES:
      RecvKeyAES(descRecvSession);
      break;
    default:
      FixHack("Undefined type packet");
  }
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
void TManagerSession::Disconnect(TIP_Port* ip_port)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return;
  }
  //===================================================================
  mMngCtxCrypto.Close(*ip_port);

  TSession* pSession = mNavigateSession->FindSessionByIP(*ip_port);
  if(pSession)
  {
    unsigned int id = pSession->GetID();
    mCallBackDiconnect.Notify(id);
    mNavigateSession->Delete(pSession);
  }
  unlockAccessSession();
}
//--------------------------------------------------------------------------------------------
TSession* TManagerSession::NewSession(TIP_Port& ip_port, INetTransport* pTransport)
{
  mLastID_Session++;// ��� �������� �� ����������, unsigned int 4 ���� - ������� �����
  TSession* pSession = new TSession(mTimeLiveSession);
  pSession->SetTransport(pTransport);
  pSession->SetInfo(ip_port);
  pSession->SetID(mLastID_Session);

  mNavigateSession->Add(pSession);
  return pSession;
}
//--------------------------------------------------------------------------------------------
bool TManagerSession::IsExist(unsigned int ID_Session)
{
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return false;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(ID_Session);
  unlockAccessSession();
  return (pSession!=NULL);
}
//--------------------------------------------------------------------------------------------
void TManagerSession::SetTimeLiveSession(unsigned int time_ms)
{
  mTimeLiveSession = time_ms;
}
//-------------------------------------------------------------------------
bool TManagerSession::GetInfo(unsigned int ID_Session, TIP_Port& ip_port_out)
{
  bool res = false;
  lockAccessSession();
  if(mNavigateSession==NULL)
  {
    unlockAccessSession();
    return false;
  }
  //===================================================================
  TSession* pSession = mNavigateSession->FindSessionByID(ID_Session);
  if(pSession)
  {
    res = true;
    pSession->GetInfo(ip_port_out);
  }
  unlockAccessSession();
  return res;
}
//-------------------------------------------------------------------------
void TManagerSession::SetUseCryptTCP(bool v)
{
  flgUseCryptTCP = v;
}
//-------------------------------------------------------------------------
bool TManagerSession::GetUseCryptTCP()
{
  return flgUseCryptTCP;
}
//-------------------------------------------------------------------------
void TManagerSession::RecvPacket(TDescRecvSession& descRecvSession, TSession* pSession)
{
  // ��� ������� ������ ������������ ���� ������������ ���� �� ������������ ��������
  if(GetUseCryptTCP())
  {
    // ��� ���������� �������, � ��� �������� �� �� �����������. ���������� �����.
    if(descRecvSession.use_crypt==false)
    {
      FixHack("System use crypt, but recv not crypt");
      return;
    }
  }
  // �������� �������
  if(descRecvSession.sizeData<=sizeof(TSession::THeader))
  {
    FixHack("Size less Header");
    return;
  }
  // ��� �������������� ������
  TContainerPtr c_decrypt;

  descRecvSession.data     += sizeof(TSession::THeader);
  descRecvSession.sizeData -= sizeof(TSession::THeader);

  if(descRecvSession.use_crypt)
  {
    if(descRecvSession.sizeData <= sizeof(unsigned int)  // counter 4 �����
                                 + sizeof(unsigned char))// crc8, 1 ����
    {
      FixHack("Size less Counter + CRC8");
      return;
    }
    // ������������� ����� ���� ������ TCP
    if(descRecvSession.type==INetTransport::eUdp)
    {
      FixHack("Using crypt for UDP");
      BL_FIX_BUG();
			return;
    }
    // ������� ������������
    if(mMngCtxCrypto.Recv(descRecvSession.ip_port, descRecvSession.data,
                          descRecvSession.sizeData,c_decrypt)==false)
    {
      FixHack("Can't decrypt packet");
	    BL_FIX_BUG();
      return;
    }
    // ��������� ���������
    descRecvSession.data     = (char*)c_decrypt.GetPtr();
    descRecvSession.sizeData = c_decrypt.GetSize();
    // ���������� ������� ������ ������ ���� ������ ��� ����������
    if(pSession->GetCounterIn()>=((unsigned int*)descRecvSession.data)[0])
    {
      FixHack("Fail counter in");
      return;
    }
    // ��������� �� ����� ��������
    descRecvSession.data     += sizeof(unsigned int);
    descRecvSession.sizeData -= sizeof(unsigned int);
  }
  mCallBackRecv.Notify(&descRecvSession);
}
//-------------------------------------------------------------------------
void TManagerSession::RecvKeyRSA(TDescRecvSession& descRecvSession, TSession* pSession)
{
  char* pKey  = descRecvSession.data     + sizeof(TSession::THeader);
  int sizeKey = descRecvSession.sizeData - sizeof(TSession::THeader);

  if(mMngCtxCrypto.RecvRSA_PublicKey(descRecvSession.ip_port, pKey, sizeKey)==false)
  {
    FixHack("Incorrect RSA public key");
    return;
  }

  TContainer c_AESkey;
  mMngCtxCrypto.SendAES_Key(descRecvSession.ip_port, c_AESkey);
  pSession->SendKeyAES(c_AESkey);
}
//-------------------------------------------------------------------------
void TManagerSession::RecvKeyAES(TDescRecvSession& descRecvSession)
{
  char* pKey  = descRecvSession.data     + sizeof(TSession::THeader);
  int sizeKey = descRecvSession.sizeData - sizeof(TSession::THeader);

  if(mMngCtxCrypto.RecvAES_Key(descRecvSession.ip_port, pKey, sizeKey)==false)
  {
    FixHack("Incorrect AES public key");
    return;
  }

  flgGetAnswerFromUp = true;
}
//-------------------------------------------------------------------------
void TManagerSession::FixHack(char* sMsg)
{
  GetLogger(STR_NAME_MMO_ENGINE)->WriteF_time("Try hack: %s.\n", sMsg);
}
//-------------------------------------------------------------------------
bool TManagerSession::WaitAnswerFromUp()
{
  unsigned int start_ms = ht_GetMSCount();
  unsigned int delta    = 0;

  while(delta < eLimitWaitTimeAnswerFromUp)
  {
    if(flgGetAnswerFromUp)
      return true;
    ht_msleep(eSleepForWaitUp);
    unsigned int now_ms = ht_GetMSCount();
    delta = now_ms - start_ms;
  }
  return false;
}
//-------------------------------------------------------------------------
void TManagerSession::SetupFlagsForWaitUp()
{
  flgNeedAnswerFromUp = true;
  flgGetAnswerFromUp  = false;
}
//-------------------------------------------------------------------------
void TManagerSession::CleanFlagsForWaitUp()
{
  flgNeedAnswerFromUp = false;
  flgGetAnswerFromUp  = false;
}
//-------------------------------------------------------------------------
void TManagerSession::SendKeyRSA_Up(TSession* pSession)
{
  SetupFlagsForWaitUp();

  TContainer c_RSAkey;
  mMngCtxCrypto.SendRSA_PublicKey(mIP_PortUp, c_RSAkey);
  
  pSession->SendKeyRSA(c_RSAkey);
}
//-------------------------------------------------------------------------
void TManagerSession::Send(TSession* pSession, TBreakPacket bp, bool check)
{
  if(check==true)// TCP
  if(GetUseCryptTCP())  
  {
    // ������� ip � ����
    TIP_Port ip_port;
    pSession->GetInfo(ip_port);
    pSession->IncrementCounterOut();
    unsigned int counter_out = pSession->GetCounterOut();
    bp.PushFront((char*)&counter_out, sizeof(counter_out));
    // �����������
    TContainer c_encrypt;
    mMngCtxCrypto.Send(ip_port, bp, c_encrypt);
    // ������� ����� �����
    TBreakPacket encrypt_bp;
    encrypt_bp.PushFront((char*)c_encrypt.GetPtr(), c_encrypt.GetSize());
    // �������� � ���������� "���������"
    pSession->Send(encrypt_bp, true, true);
    return;
  }
  // ���� UDP, ���� �� ����������� TCP
  pSession->Send(bp, check);
}
//-------------------------------------------------------------------------
