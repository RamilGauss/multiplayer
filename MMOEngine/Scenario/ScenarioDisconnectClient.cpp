/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

// Begin � End �� ���������,
// �.�. �������� ���� �������� ��������, ������� ��� �� �� ����� �� ���������
// � ��� �� �������������

#include "ScenarioDisconnectClient.h"
#include "Logger.h"
#include "ManagerSession.h"

using namespace std;
using namespace nsMMOEngine;

TScenarioDisconnectClient::TScenarioDisconnectClient()
{

}
//---------------------------------------------------------------
void TScenarioDisconnectClient::Recv(TDescRecvSession* pDesc)
{
  THeaderDisconnectClient* pH = (THeaderDisconnectClient*)pDesc->data;
  switch(pH->subType)
  {
    case eFromMaster:
      RecvFromMaster(pDesc);
      break;
    case eFromSlave:
      RecvFromSlave(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//---------------------------------------------------------------
void TScenarioDisconnectClient::Work()
{

}
//---------------------------------------------------------------
void TScenarioDisconnectClient::DisconnectFromSlave(unsigned int id_client)
{
  // ������������ ������
  TBreakPacket bp;// ��������� ��� ����� ������
  THeaderFromSlave h;
  h.id_client = id_client;
  bp.PushFront((char*)&h, sizeof(h));
  // �������� ����� ��� ������� �����������
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//---------------------------------------------------------------
void TScenarioDisconnectClient::DisconnectFromMaster(vector<unsigned int>& vecID_client)
{
  // ������������ ������
  TBreakPacket bp;// ��������� ��� ����� ������
  bp.PushFront((char*)&vecID_client[0],vecID_client.size()*sizeof(unsigned int));
  THeaderFromMaster h;
  h.countID = vecID_client.size();
  bp.PushFront((char*)&h, sizeof(h));
  // �������� ����� ��� ������� �����������
  Context()->GetMS()->Send(Context()->GetID_Session(), bp);
}
//---------------------------------------------------------------
void TScenarioDisconnectClient::RecvFromMaster(TDescRecvSession* pDesc)
{
  THeaderFromMaster* pH = (THeaderFromMaster*)pDesc->data;

  unsigned int* pArrID = (unsigned int*)(pDesc->data + sizeof(THeaderFromMaster));
  for(int i = 0 ; i < pH->countID ; i++ )
  {
    // ����� �������� ��� ��� ID
    NeedContextByClientKey(pArrID[i]);
  }
}
//---------------------------------------------------------------
void TScenarioDisconnectClient::RecvFromSlave(TDescRecvSession* pDesc)
{
  THeaderFromSlave* pH = (THeaderFromSlave*)pDesc->data;

  NeedContextByClientKey(pH->id_client);
}
//---------------------------------------------------------------
