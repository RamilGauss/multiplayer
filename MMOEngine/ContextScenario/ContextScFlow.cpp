/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScFlow.h"
#include "ManagerSession.h"
#include <boost/foreach.hpp>

using namespace nsMMOEngine;

TContextScFlow::TContextScFlow()
{

}  
//------------------------------------------------------------
TContextScFlow::~TContextScFlow()
{
  Done();
}  
//------------------------------------------------------------
void TContextScFlow::Done()
{
  BOOST_FOREACH(TSavePacket* pSP,mListSave)
    delete pSP;
}
//------------------------------------------------------------
void TContextScFlow::Send( TBreakPacket& bp, bool check)
{
  GetMS()->Send( GetID_Session(), bp, check);
}  
//------------------------------------------------------------
void TContextScFlow::SaveBreakPacket(TBreakPacket& bp, bool check)
{
  TSavePacket* pSP = new TSavePacket;
  // ������� �����
  bp.Collect();
  // ��������� ��� ��������� ��������� �����
  char* p  = (char*)bp.GetCollectPtr();
  int size = bp.GetSize();
  // ���������� �� ���������� ������
  bp.UnlinkCollect();
  // ������ �� �������� ������ ������ � ���������
  pSP->c.Entrust(p, size);
  // ������ ����� ������ �������� � ���������� �������� � break packet
  pSP->bp.PushFront(p, size);
  pSP->check = check;

  mListSave.push_back(pSP);
}
//-------------------------------------------------------------------------
void TContextScFlow::SendAndRemoveFirst()
{
  if(mListSave.size()==0)
  {
    // �� �������
    BL_FIX_BUG();
    return;
  }

  TSavePacket* pSP = mListSave.front();
  mListSave.pop_front();
  Send(pSP->bp, pSP->check);
  delete pSP;
}
//-------------------------------------------------------------------------
