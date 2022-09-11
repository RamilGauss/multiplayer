/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SCENARIO_FLOW_H
#define SCENARIO_FLOW_H

#include "IScenario.h"
#include "ContextScFlow.h"
#include "MakerScenario.h"

namespace nsMMOEngine
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
      // ������ �� ����, ����� �������� ����� �������� �������, ������ ������ �� ������� ������ � ������
      if(pDesc->sizeData < sizeof(THeaderFlow))
        return;
      // ������� �������
      TypeSrc* pEvent = new TypeSrc;
      // ���������� �� ������, � ������� ���������� �����
      pDesc->c.Unlink();
      // ������ ������ ��� �������� �������
      pEvent->c.Entrust(pDesc->data, pDesc->sizeData);
			pEvent->data     = pDesc->data     + sizeof(THeaderFlow);
			pEvent->sizeData = pDesc->sizeData - sizeof(THeaderFlow);
      // ������ ������ ����� - ������
      pEvent->id_session = pDesc->id_session;
      // �������� ������� ��� ����������� � ������� �������� ����� �������� ������� � ����������
      Context()->GetSE()->AddEventWithoutCopy(pEvent, sizeof(TypeSrc), pDesc->time_ms);
    }
  };
  //-------------------------------------------------------------------
}
#endif