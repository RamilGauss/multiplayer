#ifndef PredictionH
#define PredictionH

#include "Tank.h"
#include "ObjectPrediction.h"
#include <list>

// предсказатель
class TPrediction
{

protected:


public:

  TPrediction();
  ~TPrediction();

  void SetListTank();
  void LoadMap(unsigned short id_map);
  void InitState();
  void SetState();

  void Calc();

  void SetOrientAim(TTank*pTank, nsServerStruct::TPacketServer* pDefPacket);
  void SetKeyEvent(TTank*pTank, nsServerStruct::TPacketServer* pDefPacket);


public:
  // список разрушенных или поврежденных объектов.
  std::list<TObjectPrediction*> mListDamageObject;

  struct TEvent
  {

  };
  std::list<TEvent*> mListFreshEvent;

};


#endif

