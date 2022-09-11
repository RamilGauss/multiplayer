/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContainerContextSc.h"
#include "MasterPrivate.h"

using namespace nsMelissa;

TMasterPrivate::TMasterPrivate()
{

}
//--------------------------------------------------------------------
TContainerContextSc* TMasterPrivate::GetC()
{
  return &mC;
}
//--------------------------------------------------------------------
int TMasterPrivate::GetCountClient()
{
  return 0;
}
//--------------------------------------------------------------------
bool TMasterPrivate::SearchClient(unsigned int id_client)
{
  return false;
}
//--------------------------------------------------------------------
