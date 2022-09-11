/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SCENARIO_FLOW_H
#define MELISSA_SCENARIO_FLOW_H

#include "IScenario.h"
#include "ContextScFlow.h"
#include "MakerScenario.h"

namespace nsMelissa
{
  class TScenarioFlow : public IScenario
  {
    enum{eUp,eDown};
    struct THeaderFlow : public IScenario::TBaseHeader
    {
      THeaderFlow(){type = TMakerScenario::eFlow;}
    };
    //-------------------------------------------------
    struct THeaderSendUp : public THeaderFlow
    {
      THeaderSendUp(){subType = eUp;}
    };
    //-------------------------------------------------
    struct THeaderSendDown : public THeaderFlow
    {
      THeaderSendDown(){subType = eDown;}
    };
    //-------------------------------------------------
  public:
    TScenarioFlow();
    virtual ~TScenarioFlow();

    void SendUp(TBreakPacket bp, bool check);
    void SendDown(TBreakPacket bp, bool check);

    virtual void Recv(TDescRecvSession* pDesc);
  protected:
    virtual void DelayBegin();
  private:
    TContextScFlow* Context(){return (TContextScFlow*)mCurContext;}
    // дл€ Send
    void HandlePacket(TBreakPacket& bp, bool check);
    // дл€ Recv
    template <class TypeSrc>
    void Recv(TDescRecvSession* pDesc)
    {
      // создать событие
      TypeSrc* pEvent = new TypeSrc;
      // отцепитьс€ от пам€ти, в которой содержитс€ пакет
      pDesc->c.Unlink();
      // отдать пам€ть под контроль событи€
      pEvent->c.Entrust(pDesc->data, pDesc->sizeData);
      // откуда пришел пакет - сесси€
      pEvent->id_session = pDesc->id_session;
      // добавить событие без копировани€ и указать истинное врем€ создани€ событи€ в транспорте
      Context()->GetSE()->AddEventWithoutCopy(pEvent, sizeof(TypeSrc), pDesc->time_ms);
    }
  };
  //-------------------------------------------------------------------
}
#endif