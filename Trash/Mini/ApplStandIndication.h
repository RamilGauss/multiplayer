/*
			04.10.2012
				Gauss
	Класс для управления и взаимодействия со стендом индикации.
	Прикладной уровень
	Является частью ПО "Магний"

	//*********************************************
	Объект сам найдет устройство
	//*********************************************
*/

#ifndef TransportStandIndication_H
#define TransportStandIndication_H

#include "StandIndication.h"
#include "CallBackRegistrator.h"

#include "PacketApplLevel_SI.h"

class TApplStandIndication
{
protected:
	volatile bool flgRecvPacket;
	TPacketApplLevel_SI mPacketRecv;
	TStandIndication mSI;
	
	TSaveOnHDD mLog;
	
protected:	
	enum
	{
		eSet,               // команда
		eEcho,              // квитанция на eSet
		eGetPrd,            // запрос
		eAnswerPrd,
		eGetState,          // запрос
		eAnswerState,
		eGetStateSystemCool,// запрос
		eAnswerStateSystemCool,
    eGetVersion,
    eAnswerVersion,
	};

public:
		
	TApplStandIndication();
	virtual ~TApplStandIndication();
	
	// отправка запросов и команд на СИ
	bool Set( nsStandIndication::TPacketSet& pIn);//команда sync
	bool GetPRD(nsStandIndication::TPacketAnswerPrd& pOut);						// запрос sync
	bool GetState(nsStandIndication::TPacketAnswerState& pOut);					// запрос sync
	bool GetStateSystemCool(nsStandIndication::TPacketAnswerStateSystemCool& pOut);// запрос sync
	
	bool IsConnect(){return mSI.IsActive();}
	
  // Gauss 01.03.2013
  // для тестирования
  bool TryConnect();// проверить есть ли связь и ,если нет, то попытаться соединиться
	
protected:
		
	bool Send(        void* packet, int sizePacket, int type);
	bool SendRequest( void* pOut,   int sizeOut,    int type);
	
	void Init();
	void Done();
	
	friend void FromTransport_Answer(void* pData, int size);

	void Answer(void* pData, int size);
	
};

#endif

