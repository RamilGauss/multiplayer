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


#ifndef NetTransportUDPH
#define NetTransportUDPH

#include <set>

#include "glib/gthread.h"

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "hArray.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "INetTransport.h"
#include "BasePacketNetTransport.h"
#include "TransportProtocolPacket.h"

// содержит поток для проверки получения пакетов по сети
// ограничение по максимальному размеру пакета - 1400 байт.
// не может контролировать трафик
// плюсы класса в том что он построен только на UDP

// Thread safe - Send поддерживает.

class TNetTransport_UDP : public INetTransport
{
protected:
  // для сервера, в клиенте такая ситуация маловероятна
  // при отправке известному клиенту
  // и при получении от неизвестного клиента (см. GetInfoConnect(...))
	GCS gcsSendRcv;
	void lockSendRcv()  {Lock(&gcsSendRcv);};
	void unlockSendRcv(){Unlock(&gcsSendRcv);};

	TCallBackRegistrator mCallBackRecv;      // TDescRecv		
	TCallBackRegistrator mCallBackDisconnect;// TIP_Port

	UdpDevice mUDP;	

	volatile bool flgActive;
	volatile bool flgNeedStop;

	// пакеты, ожидающие квитанцию
	//TArrayObject mArrWaitCheck;
	// информация по сокету, какой текущий номер пакета идет на прием и на отправку
	//TArrayObject mArrConnect;

  typedef std::map<TIP_Port,nsNetTransportStruct::TInfoConnect*> TMapIP_Ptr;
  typedef TMapIP_Ptr::iterator TMapIP_Ptr_It;

  TMapIP_Ptr mMapInfoConnect;

  typedef std::multimap<TIP_Port,TBasePacketNetTransport*> TMultiMapIP_Ptr;
  typedef TMultiMapIP_Ptr::iterator TMultiMapIP_Ptr_It;

  TMultiMapIP_Ptr mMultiMapWaitCheck;

	enum
  {
    eSizeBuffer        = 65535,
    eCntTry            = 10,
    eTimeLivePacket    = 120, //если в течение такого времени не будет квитанции, считается пакет не доставлен, мс
    eWaitThread        = eTimeLivePacket*eCntTry,
    eTimeRefreshEngine = 60,  // частота обновления движка, мс
    
    // размер системных буферов
    eSizeBufferForRecv = 30000000, // байт
    eSizeBufferForSend = 30000000, // байт
    eFeedBack          = 20,// ждать пока (де)активизируется двигатель, мс
	};
	typedef enum
	{
		ePacket  = 'P',
		eStream  = 'S',
		eSynchro = 'C',
		eCheck   = 'K',
	}eTypeHeadPacket;
  enum
  {
    eWaitSynchro=5,// сек
  };

	char mBuffer[eSizeBuffer];
	GThread* thread;

public:

	TNetTransport_UDP();
	virtual ~TNetTransport_UDP();

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);
	virtual void Send(unsigned int ip, unsigned short port, 
                    TBreakPacket& packet,
                    bool check = true);

	// чтение - зарегистрируйся
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type);
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type);

	virtual void Start();
	virtual void Stop();
	virtual bool IsActive();

  // синхронизирующая функция
  virtual bool Connect(unsigned int ip, unsigned short port); // вызов только для клиента

protected:
  bool SendSynchro(unsigned int ip, unsigned short port, int cntTry);

  // передать объект блокировки
  void Lock(void* pLocker);
  void Unlock(void* pLocker);

protected:
  void NotifyDisconnect(TIP_Port* data){mCallBackDisconnect.Notify(data,sizeof(TIP_Port));};

	friend void* ThreadTransport(void*p);
	void Engine();

protected:
	void AnalizPacket(unsigned int ip,unsigned short port,int size);
  void FindAndCheck(nsNetTransportStruct::THeader* prefix,
                    unsigned int ip,unsigned short port);
  void NotifyRecv(eTypeRecv type, int size);
  void SendCheck(nsNetTransportStruct::THeader* prefix,
                 unsigned int ip,unsigned short port);

  // сколько ждать в чтении
	int GetTimeout();// мс
	void SendUnchecked();

  bool Send(TBasePacketNetTransport* pDefPacket);

protected:
  bool Write(void *p, int size, unsigned int ip, unsigned short port);

  void WriteLog(void *p, int size, unsigned int ip, unsigned short port);
	void ReadLog(int size, unsigned int ip, unsigned short port);
	void LogTransportInfo(nsNetTransportStruct::THeader* p,int size);

protected:

  bool IsPacketFresh();
  bool IsStreamFresh();

  bool A_more_B(unsigned short A, unsigned short B);
  
  // если найдет объект - вернет указатель на него
  // иначе создаст новый
	nsNetTransportStruct::TInfoConnect* GetInfoConnect(TIP_Port& v);

  // в массиве pArr найти и удалить ячейки, которые найдутся по pDefPacket
  void SearchAndDeleteInMapWaitCheck(std::set<TIP_Port>& setIP_port);
  void SetupBufferForSocket();

};


#endif