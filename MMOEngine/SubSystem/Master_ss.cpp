/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Master_ss.h"

#include <boost/foreach.hpp>

#include "Client_ss.h"

#include "ContainerContextSc.h"

using namespace nsMMOEngine;

TMaster_ss::TMaster_ss()
{

}
//--------------------------------------------------------------------
TMaster_ss::~TMaster_ss()
{
  BOOST_FOREACH(TMapUintPtr::value_type& bit, mMapKeyClient)
    delete bit.second;
  mMapKeyClient.clear();
}
//--------------------------------------------------------------------
TContainerContextSc* TMaster_ss::GetC()
{
  return &mC;
}
//--------------------------------------------------------------------
int TMaster_ss::GetCountClient()
{
  return mMapKeyClient.size();
}
//--------------------------------------------------------------------
TClient_ss* TMaster_ss::FindClient(unsigned int id_client)
{
  TMapUintPtrIt fit = mMapKeyClient.find(id_client);
  if(fit==mMapKeyClient.end())
    return NULL;

  return fit->second;
}
//--------------------------------------------------------------------
void TMaster_ss::AddClient(TClient_ss* pClient)
{
  mMapKeyClient.insert(TMapUintPtr::value_type(pClient->GetIDClient(),pClient));
}
//--------------------------------------------------------------------
unsigned int TMaster_ss::GetID_Session()
{
  return mID_SessionMasterSS;
}
//--------------------------------------------------------------------
void TMaster_ss::SetID_Session(unsigned int id)
{
  mID_SessionMasterSS = id;
}
//--------------------------------------------------------------------
void TMaster_ss::DeleteClient(unsigned int id_client)
{
  TClient_ss* pClient = FindClient(id_client);
  mMapKeyClient.erase(id_client);
  delete pClient;
}
//--------------------------------------------------------------------
