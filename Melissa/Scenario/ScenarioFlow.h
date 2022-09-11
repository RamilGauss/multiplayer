/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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
    // ��� Send
    void HandlePacket(TBreakPacket& bp, bool check);
    // ��� Recv
    template <class TypeSrc>
    void Recv(TDescRecvSession* pDesc)
    {
      // ������� �������
      TypeSrc* pEvent = new TypeSrc;
      // ���������� �� ������, � ������� ���������� �����
      pDesc->c.Unlink();
      // ������ ������ ��� �������� �������
      pEvent->c.Entrust(pDesc->data, pDesc->sizeData);
      // ������ ������ ����� - ������
      pEvent->id_session = pDesc->id_session;
      // �������� ������� ��� ����������� � ������� �������� ����� �������� ������� � ����������
      Context()->GetSE()->AddEventWithoutCopy(pEvent, sizeof(TypeSrc), pDesc->time_ms);
    }
  };
  //-------------------------------------------------------------------
}
#endif