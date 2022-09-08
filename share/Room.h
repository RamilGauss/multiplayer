#ifndef RoomH
#define RoomH
#include "ServerStruct.h"
#include "TransportProtocolTank.h"
#include "ManagerMap.h"
#include "Prediction.h"
#include <list>
#include "glib/gthread.h"

class TRoom
{

  TPrediction mPrediction;

  TransportProtocolTank* mTransport;

  TArrayObject mArrTank;

  guint32 mTimeBeginCountDown;// начался обратный отсчет
  guint32 mTimeAfterCountDown;// после окончания обратного отсчета (начался собственно сам бой)
  guint32 mNow_MS;

  bool flgEventReconnect;
  bool flgIsLoadingMap;
  GThread* threadLoadMap;

  int mState;
  enum{
       eLoad             = 0, // отсылка null-stream
       eCountDown        = 1, // отсылка countdown-stream
       eFight            = 2, // отсылка инфы по бою
  };

  enum{
    eTimeCountDown = 30000,// мс
  };

  unsigned short mID_map;

  //struct TStateObject
  //{
  //  unsigned short id;
  //  unsigned int maskState;
  //};
  unsigned char score0;
  unsigned char score1;
  //---------------------------------------------------
  struct TAction
  {
    TTank* pTank;
    nsServerStruct::TPacketServer* pDefPacket;
  };
  //---------------------------------------------------
  // список событий:
  // работаем в одном потоке
  //std::list<TStateObject> mListHistoryAction;// на отправку только присоединившимся клиентам в качестве корректирующего пакета
  std::list<TAction*> mListFreshAction;  // свежие события от клиентов, требуют обработки

public:
  TRoom();
  ~TRoom();

  void AddTank(TTank* pTank);

	void SetTransport(TransportProtocolTank* pTransport);//старт боя, передача возможности вещать на клиента

	bool Work(); // рассчитать координаты и разослать стрим клиентам

  void SetIDMap(unsigned short val);
  unsigned short GetIDMap(){return mID_map;};

  TTank* GetTank(int i){return (TTank*)mArrTank.Get(i);};

  void MakeGroup();
  void PreparePrediction();// настроить предсказатель для получения координат объектов, коллизий, событий
  void LoadMap();

  void SetPacket(nsServerStruct::TPacketServer* pPacket,TTank* ptank);

  void Done();

protected:

  void WriteTransportStream(nsServerStruct::TClient* pClient,TBasePacket *packet);
  void WriteTransportAnswer(nsServerStruct::TClient* pClient,TBasePacket *packet);

  void WorkByState();
  void WorkLoadMap();
  void WorkCountDown();
  void WorkFight();

  bool CheckEndFight();
  
  // проверка состояния потока загрузки карты
  bool CheckIsLoadComplete();

  void AnalizPacket();// здесь отослать корректирующий пакет (можно партиями)
  void CalcPrediction();

  friend void* ThreadLoadMap(void* p);
  void ThreadLoadMap();

  void SendInitCoordTank(nsServerStruct::TClient* pClient);
  void SendScore(nsServerStruct::TClient* pClient);
  void SendStateObject(nsServerStruct::TClient* pClient);

  void InitPositionTank();

};

#endif