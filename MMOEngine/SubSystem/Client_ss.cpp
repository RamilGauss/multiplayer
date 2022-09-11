/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Client_ss.h"

#include "ContainerContextSc.h"

using namespace nsMMOEngine;

TClient_ss::TClient_ss()
{
  mIDClient = (unsigned int)-1;
}
//--------------------------------------------------------------------
TContainerContextSc* TClient_ss::GetC()
{
  return &mC;
}
//--------------------------------------------------------------------
void TClient_ss::SetID_SessionMaster(unsigned int id_session)
{
  mID_SessionMaster = id_session;
}
//--------------------------------------------------------------------
unsigned int TClient_ss::GetID_SessionMaster()
{
  return mID_SessionMaster;
}
//--------------------------------------------------------------------
void TClient_ss::SetIDClient(unsigned int id_client)
{
  mIDClient = id_client;
}
//--------------------------------------------------------------------
unsigned int TClient_ss::GetIDClient()
{
  return mIDClient;
}
//--------------------------------------------------------------------
