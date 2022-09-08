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

  guint32 mTimeBeginCountDown;// ������� �������� ������
  guint32 mTimeAfterCountDown;// ����� ��������� ��������� ������� (������� ���������� ��� ���)
  guint32 mNow_MS;

  bool flgEventReconnect;
  bool flgIsLoadingMap;
  GThread* threadLoadMap;

  int mState;
  enum{
       eLoad             = 0, // ������� null-stream
       eCountDown        = 1, // ������� countdown-stream
       eFight            = 2, // ������� ���� �� ���
  };

  enum{
    eTimeCountDown = 30000,// ��
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
  // ������ �������:
  // �������� � ����� ������
  //std::list<TStateObject> mListHistoryAction;// �� �������� ������ ���������������� �������� � �������� ��������������� ������
  std::list<TAction*> mListFreshAction;  // ������ ������� �� ��������, ������� ���������

public:
  TRoom();
  ~TRoom();

  void AddTank(TTank* pTank);

	void SetTransport(TransportProtocolTank* pTransport);//����� ���, �������� ����������� ������ �� �������

	bool Work(); // ���������� ���������� � ��������� ����� ��������

  void SetIDMap(unsigned short val);
  unsigned short GetIDMap(){return mID_map;};

  TTank* GetTank(int i){return (TTank*)mArrTank.Get(i);};

  void MakeGroup();
  void PreparePrediction();// ��������� ������������� ��� ��������� ��������� ��������, ��������, �������
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
  
  // �������� ��������� ������ �������� �����
  bool CheckIsLoadComplete();

  void AnalizPacket();// ����� �������� �������������� ����� (����� ��������)
  void CalcPrediction();

  friend void* ThreadLoadMap(void* p);
  void ThreadLoadMap();

  void SendInitCoordTank(nsServerStruct::TClient* pClient);
  void SendScore(nsServerStruct::TClient* pClient);
  void SendStateObject(nsServerStruct::TClient* pClient);

  void InitPositionTank();

};

#endif