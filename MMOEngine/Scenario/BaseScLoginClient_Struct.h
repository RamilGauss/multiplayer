/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BASE_SC_LOGIN_CLIENT_STRUCT_H
#define BASE_SC_LOGIN_CLIENT_STRUCT_H

#include "IScenario.h"
#include "DescRecvSession.h"

namespace nsMMOEngine
{
  namespace nsLoginClientStruct
  {
    enum{
      // ������, ��������� ���������
      eTryLoginC2M,
      eRequestM2SS,
      eCheckRequestSS2M,
      eResultLoginM2C,
      eLeaveQueueC2M,
      eCheckLeaveQueueM2C,
      //-------------------------------
      // ������������� ����� ��������
      eInfoClientM2S,
      eCheckInfoClientS2M,
      eInfoSlaveM2C,
      eConnectToSlaveC2S,
      eClientConnectS2M,
      eCheckClientConnectM2S,
      eCheckConnectToSlaveS2C,
    };
    enum
    {
      eClient,
      eSlave,
      eMaster,
      eSuperServer,
    };
    //-------------------------------------------------
#ifdef WIN32
#pragma pack(push, 1)
#endif
    struct THeader : public IScenario::TBaseHeader
    {
      THeader();
      unsigned char from;
      unsigned int  id_client;
    };
	  //-------------------------------------------------
    struct THeaderC  : public THeader{ THeaderC (){from=eClient;}};
    struct THeaderM  : public THeader{ THeaderM (){from=eMaster;}};
    struct THeaderS  : public THeader{ THeaderS (){from=eSlave;}};
    struct THeaderSS : public THeader{ THeaderSS(){from=eSuperServer;}};
    //-------------------------------------------------
	  struct THeaderTryLoginC2M : THeaderC
	  {
      THeaderTryLoginC2M();
		  int sizeData;
	  };
	  //-------------------------------------------------
	  struct THeaderRequestM2SS : THeaderM
	  {
		  THeaderRequestM2SS();
	  };
	  //-------------------------------------------------
    struct THeaderCheckRequestSS2M : THeaderSS
    {
      THeaderCheckRequestSS2M();
      char isExistInSystem;// ������ ������ ������������ � �������
    };
    //-------------------------------------------------
    struct THeaderResultLoginM2C : THeaderM
    {
      enum{eAccept,eReject,eQueue};
      THeaderResultLoginM2C();
      char result;
      int numInQueue;
      int sizeResClient;
    };
    //-------------------------------------------------
	  struct THeaderLeaveQueueC2M : public THeaderC
	  {
      THeaderLeaveQueueC2M();
	  };
	  //-------------------------------------------------
	  struct THeaderCheckLeaveM2C : public THeaderM
	  {
      THeaderCheckLeaveM2C();
	  };
	  //------------------------------------------------------------------------------------------------
	  //------------------------------------------------------------------------------------------------
	  struct THeaderInfoClientM2S : public THeaderM
	  {
      THeaderInfoClientM2S();
	  };
	  //-------------------------------------------------
    struct THeaderCheckInfoClientS2M : public THeaderS
    {
      THeaderCheckInfoClientS2M();
    };
	  //-------------------------------------------------
	  struct THeaderInfoSlaveM2C : public THeaderM
	  {
      THeaderInfoSlaveM2C();
      TIP_Port ip_port_slave;
	  };
	  //-------------------------------------------------
	  struct THeaderConnectToSlaveC2S : public THeaderC
	  {
      THeaderConnectToSlaveC2S();
	  };
	  //-------------------------------------------------
	  struct THeaderClientConnectS2M : public THeaderS
	  {
      THeaderClientConnectS2M();
	  };
	  //-------------------------------------------------
	  struct THeaderCheckClientConnectM2S : public THeaderM
	  {
      THeaderCheckClientConnectM2S();
	  };
	  //-------------------------------------------------
	  struct THeaderCheckConnectToSlaveS2C : public THeaderS
	  {
      THeaderCheckConnectToSlaveS2C();
	  };
	  //-------------------------------------------------
#ifdef WIN32
#pragma pack(pop)
#endif
  }
}
#endif
