/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IPhysicEngineH
#define IPhysicEngineH

#include <list>

#include "IBaseObjectPrediction.h"
#include "SrcEvent.h"

#define STR_NAME_ROBERT "PE"

// предсказатель
// Robert - физический движок
class IPhysicEngine : public TSrcEvent
{
public:

  IPhysicEngine(){};
  virtual ~IPhysicEngine(){};

  virtual void AddObject(IBaseObjectPrediction* pObject) = 0;
  virtual void Clear() = 0;


  virtual void InitState() = 0;

  virtual void Calc() = 0;

public:
  std::list<IBaseObjectPrediction*> mListObject;
  // список разрушенных или поврежденных объектов.
  std::list<IBaseObjectPrediction*> mListDamageObject;

  struct TEvent
  {

  };
  std::list<TEvent*> mListFreshEvent;

};


#endif

