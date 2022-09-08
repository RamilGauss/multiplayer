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

  //---------------------------------------------------
  struct TAction
  {

  };
  //---------------------------------------------------
  struct TCorrectPacket
  {

  };
  //---------------------------------------------------
  // список событий:
  // работаем в одном потоке
  std::list<TAction> mListHistoryAction;// на отправку только присоединившимся клиентам в качестве корректирующего пакета
  std::list<TAction> mListFreshAction;  // свежие события от клиентов, требуют обработки
  std::list<TCorrectPacket> mListRequestSendCorrectPacket; // то что не успели отправить

public:
  TRoom();
  ~TRoom();

  void AddTank(TTank* pTank);

	void SetTransport(TransportProtocolTank* pTransport);//старт боя, передача возможности вещать на клиента

	void SetPacket(TTank* pTank, TBasePacket* packet);

	bool Work(); // рассчитать координаты и разослать стрим клиентам

  void SetIDMap(unsigned short val);
  unsigned short GetIDMap(){return mID_map;};

  TTank* GetTank(int i){return (TTank*)mArrTank.Get(i);};

  void MakeGroup();
  void PreparePrediction();// настроить предсказатель для получения координат объектов, коллизий, событий
  void LoadMap();
protected:

  void WriteTransportStream(nsServerStruct::TClient* pClient,TBasePacket *packet);
  void WriteTransportAnswer(nsServerStruct::TClient* pClient,TBasePacket *packet);

  void Done();

  void WorkByState();
  void WorkLoadMap();
  void WorkCountDown();
  void WorkFight();

  bool CheckEndFight();
  
  // проверка состояния потока загрузки карты
  bool CheckIsLoadComplete();

  void SendResultFight();

  void AnalizPacket();// здесь отослать корректирующий пакет (можно партиями)
  void SetDataInPrediction();
  void CalcPrediction();

  friend void* ThreadLoadMap(void* p);
  void ThreadLoadMap();

};

#endif