/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BasePacketNetTransportH
#define BasePacketNetTransportH

#include "BasePacket.h"
#include "TransportProtocolPacket.h"
#include "TypeDef.h"

class TBasePacketNetTransport : public TBasePacket
{
public:
	TBasePacketNetTransport();
	virtual ~TBasePacketNetTransport();

	virtual int GetType();

  GetSet(nsNetTransportStruct::THeader,PrefixTransport)

  void SetHeader(nsNetTransportStruct::TShortDescPacket* pInfoD );

  void SetType(unsigned char type);
  void SetCnIn(unsigned short cn);
  void SetCnOut(unsigned short cn);
  void SetTime(unsigned int time_ms);
  void SetCntTry(unsigned char  cntTry);

	unsigned short GetCnIn();
  unsigned short GetCnOut();
  unsigned int GetTime();
  unsigned char GetCntTry();
  void* GetData(int & size, unsigned int& ip_dst, unsigned short& port_dst);
  unsigned int GetIP_dst() const;
  unsigned short GetPort_dst() const;

protected:

	virtual void Init();

	nsNetTransportStruct::THeader* GetPrefix() const
  {return (nsNetTransportStruct::THeader*)mC.GetPtr();}

};


#endif