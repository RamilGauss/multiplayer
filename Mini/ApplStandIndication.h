/*
			04.10.2012
				Gauss
	����� ��� ���������� � �������������� �� ������� ���������.
	���������� �������
	�������� ������ �� "������"

	//*********************************************
	������ ��� ������ ����������
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
		eSet,               // �������
		eEcho,              // ��������� �� eSet
		eGetPrd,            // ������
		eAnswerPrd,
		eGetState,          // ������
		eAnswerState,
		eGetStateSystemCool,// ������
		eAnswerStateSystemCool,
    eGetVersion,
    eAnswerVersion,
	};

public:
		
	TApplStandIndication();
	virtual ~TApplStandIndication();
	
	// �������� �������� � ������ �� ��
	bool Set( nsStandIndication::TPacketSet& pIn);//������� sync
	bool GetPRD(nsStandIndication::TPacketAnswerPrd& pOut);						// ������ sync
	bool GetState(nsStandIndication::TPacketAnswerState& pOut);					// ������ sync
	bool GetStateSystemCool(nsStandIndication::TPacketAnswerStateSystemCool& pOut);// ������ sync
	
	bool IsConnect(){return mSI.IsActive();}
	
  // Gauss 01.03.2013
  // ��� ������������
  bool TryConnect();// ��������� ���� �� ����� � ,���� ���, �� ���������� �����������
	
protected:
		
	bool Send(        void* packet, int sizePacket, int type);
	bool SendRequest( void* pOut,   int sizeOut,    int type);
	
	void Init();
	void Done();
	
	friend void FromTransport_Answer(void* pData, int size);

	void Answer(void* pData, int size);
	
};

#endif

