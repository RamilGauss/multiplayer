/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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


#ifndef NetTransportH
#define NetTransportH

#include "CallBackRegistrator.h"
#include "UdpDevice.h"
#include "glib/gthread.h"
#include "TransportProtocolPacket.h"
#include "hArray.h"
#include "GCS.h"
#include "BL_Debug.h"
#include "INET_Transport.h"
#include <set>

// Melissa - транспорт
// Активный вариант транспорта
// содержит поток для проверки получения пакетов по сети

class TNetTransport : public INET_Transport
{
protected:
  // для сервера, в клиенте такая ситуация маловероятна
  // при отправке известному клиенту
  // и при получении от неизвестного клиента (см. GetInfoConnect(...))
  GCS gcsArrFresh;
  void lockArrConnect(){Lock(&gcsArrFresh);};
  void unlockArrConnect(){Unlock(&gcsArrFresh);};

	GCS gcsSendRcv;
	void lockSendRcv(){Lock(&gcsSendRcv);};
	void unlockSendRcv(){Unlock(&gcsSendRcv);};


  TCallBackRegistrator mCallBackRecvPacket;// указатель на ip, port, пакет и размер данных по указателю, данные - {4б|2б|sizeб-6б}
  TCallBackRegistrator mCallBackRecvStream;// указатель на ip, port, пакет и размер данных по указателю, данные - {4б|2б|sizeб-6б}
  TCallBackRegistrator mCallBackDisconnect;// указатель на ip, port с кем произошел разрыв связи

	UdpDevice mUDP;	

	bool flgActive;
	bool flgNeedStop;

	enum{eSizeBuffer     = 65535,
       eCntTry         = 15,
       eTimeLivePacket = 60,// мс
			 eTimeout        = 60,// частота обновления движка, мс
       eWaitThread     = eCntTry*eTimeLivePacket,// мс
       eSizeBufferForRecv = 30000000, // байт
       eSizeBufferForSend = 30000000, // байт
	};
	char mBuffer[eSizeBuffer];
	
	GThread* thread;

public:

	TNetTransport(char* pPathLog=NULL);
	virtual ~TNetTransport();
	virtual void InitLog(char* pPathLog);

  virtual bool Open(unsigned short port,int numNetWork=0);

	virtual void Write(InfoData* data, bool check = true, bool add_in_queque = true);
	// чтение - зарегистрируйся
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type);
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type);

	virtual void Start();
	virtual void Stop();

  // синхронная функция
  virtual bool Synchro(unsigned int ip, unsigned short port); // вызов только для клиента
protected:
  bool SendSynchro(unsigned int ip, unsigned short port, int cntTry);

  // передать объект 
  void Lock(void* pLocker);
  void Unlock(void* pLocker);

protected:
  void notifyRcvPacket(void * data,int size){mCallBackRecvPacket.Notify(data,size);};
  void notifyRcvStream(void * data,int size){mCallBackRecvStream.Notify(data,size);};
  void notifyDisconnect(TIP_Port* data){mCallBackDisconnect.Notify(data,sizeof(TIP_Port));};

	friend void* ThreadTransport(void*p);
	void Engine();

protected:
	
  class TDescPacket  : public TObject
  {
  protected:
    InfoData mInfoData;
    //----------------------
    void* mReadyPacket;
    int   mSizePacket;
    bool  isQueue;// зависимость от очереди, целевой сокет примет пакеты в определенном порядке.
    //----------------------
  public:  
    void SetData(InfoData* pInfoD )
    {
      mInfoData = *pInfoD;
      delete[]mReadyPacket;
      mSizePacket = mInfoData.size+sizeof(TPrefixTransport);
      mReadyPacket = new char[mSizePacket];
      memcpy((char*)mReadyPacket+sizeof(TPrefixTransport),pInfoD->packet, mInfoData.size);
      ((TPrefixTransport*)mReadyPacket)->ip_port_dst.ip   = mInfoData.ip_dst;
      ((TPrefixTransport*)mReadyPacket)->ip_port_dst.port = mInfoData.port_dst;
			((TPrefixTransport*)mReadyPacket)->ip_port_src.ip   = mInfoData.ip_src;
			((TPrefixTransport*)mReadyPacket)->ip_port_src.port = mInfoData.port_src;
      ((TPrefixTransport*)mReadyPacket)->cntTry           = -1;
    }
    void SetType(unsigned char type)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->type = type;
    }
    void SetCnIn(unsigned short cn)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->cn_in = cn;
    }
    void SetCnOut(unsigned short cn)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->cn_out = cn;
    }
    void SetTime(guint32 time_ms)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->time_ms = time_ms;
    }
    void SetCntTry(unsigned char  cntTry)
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      prefix->cntTry = cntTry;
    }

    unsigned short GetCnIn()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->cn_in;
    }
    unsigned short GetCnOut()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->cn_out;
    }
    guint32 GetTime()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->time_ms;
    }
    unsigned char GetCntTry()
    {
      if(mReadyPacket==NULL) {BL_FIX_BUG();return 0;}
      TPrefixTransport* prefix = (TPrefixTransport*)mReadyPacket;
      return prefix->cntTry;
    }

    void* GetData(int & size, unsigned int& ip_dst, unsigned short& port_dst)
    {
      if(mReadyPacket==NULL) BL_FIX_BUG();
      size = mSizePacket;
      ip_dst   = mInfoData.ip_dst;
      port_dst = mInfoData.port_dst;
      return mReadyPacket;
    }

    unsigned int GetIP_dst() const
    {
      if(mReadyPacket==NULL) BL_FIX_BUG();
      return mInfoData.ip_dst;
    }

    unsigned short GetPort_dst() const
    {
      if(mReadyPacket==NULL) BL_FIX_BUG();
      return mInfoData.port_dst;
    }

    void SetIsQueue(bool v){isQueue=v;}
    bool GetIsQueue()const{return isQueue;}
    TDescPacket(){isQueue=true;/*infoData=NULL;*/mReadyPacket=NULL;mSizePacket=0;}
    ~TDescPacket()
    {
      //delete infoData;
      delete[]mReadyPacket;
    }
  };

  // пакеты, ожидающие квитанцию
	TArrayObject mArrWaitCheck;
  // пакеты, ожидающие отправку
  TArrayObject mArrWaitSend;
	
	void* FindInList();

protected:
	void AnalizPacket(unsigned int ip,unsigned short port,int size);
  void FindAndCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port);
  void NotifyRcvPacket(int size);
  void NotifyRcvStream(int size);
  void SendCheck(TPrefixTransport* prefix,unsigned int ip,unsigned short port);

  // сколько ждать в чтении
	int GetTimeout();// мс
	void SendUnchecked();

  bool Send(TDescPacket* pDefPacket);
  void Disconnect(TIP_Port* ip_port);//TDescPacket* pDefPacket);

protected:
	bool Write(void *p, int size, unsigned int ip, unsigned short port);

	TSaveOnHDD mLogRcvSend;

	TSaveOnHDD mLogEvent;

	void WriteLog(void *p, int size, unsigned int ip, unsigned short port);
	void ReadLog(int size, unsigned int ip, unsigned short port);
	void LogTransportInfo(TPrefixTransport* p,int size);

protected:
  class  InfoConnect : public TObject
  {
    // информация по сокету
    public:
      InfoConnect(){init();}
      unsigned int   ip;
      unsigned short port;
			// для Stream
			unsigned short cn_in_s;     // циклический номер для определения свежести при Rcv
			unsigned short cn_out_s;    // циклический номер при Send
			// для Packet
			unsigned short cn_in_p;     // циклический номер для определения свежести при Rcv
			unsigned short cn_out_p;    // циклический номер при Send

      typedef std::set<unsigned short> TSetUshort;
      TSetUshort setIndexWaitRecv;// для определения свежести пакетов
      void init()
      {
        cn_in_s  = 0;
        cn_out_s = 0;
        cn_in_p  = 0;
        cn_out_p = 0;
        setIndexWaitRecv.clear();
      }
  };

  // информация по сокету. какой текущий номер пакета идет на прием и на отправку
  TArrayObject mArrConnect;

  bool IsPacketFresh();
  bool IsStreamFresh();

  bool A_more_B(unsigned short A, unsigned short B);
  
	static int SortFreshInfoConnect(const void* p1, const void* p2);
  static int SortFreshDefPacket(const void* p1, const void* p2);

  // если найдет объект - вернет указатель на него
  // иначе создаст новый
	InfoConnect* GetInfoConnect(unsigned int ip,unsigned short port);

	bool FindInArrWaitCheckQ(TDescPacket* pDefPacket);

  // в массиве pArr найти и удалить ячейки, которые найдутся по pDefPacket
  void SearchAndDelete(TArrayObject* pArr, TIP_Port* ip_port);//TDescPacket* pDefPacket);

  void SearchAndDeleteConnect(TIP_Port* ip_port);//unsigned int ip,unsigned short port);

  void SetupBufferForSocket();
};


#endif