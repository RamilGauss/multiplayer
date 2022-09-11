/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerContextDownConnection_Slave.h"

#include <boost/foreach.hpp>

#include "Logger.h"
#include "ContainerContextSc.h"
#include "Base.h"
#include "BL_Debug.h"

using namespace nsMMOEngine;

TManagerContextDownConnection_Slave::TManagerContextDownConnection_Slave()
{

}
//---------------------------------------------------------------------------------------
TManagerContextDownConnection_Slave::~TManagerContextDownConnection_Slave()
{

}
//---------------------------------------------------------------------------------------
void TManagerContextDownConnection_Slave::AddSessionEvent(unsigned int id_session)
{
	mMapSessionLoadProcent.insert(TMapUintUchar::value_type(id_session,0));
}
//---------------------------------------------------------------------------------------
void TManagerContextDownConnection_Slave::DeleteSessionEvent(unsigned int id_session)
{
	mMapSessionLoadProcent.erase(id_session);
}
//---------------------------------------------------------------------------------------
bool TManagerContextDownConnection_Slave::FindMinimumLoad(unsigned int& id_session,
																													unsigned char& load_procent)
{
	if(mMapSessionLoadProcent.size()==0)
		return false;

	mVecDesc.clear();
	BOOST_FOREACH(TMapUintUchar::value_type& it, mMapSessionLoadProcent)
		mVecDesc.push_back(TDesc(it.second,it.first));

	sort(mVecDesc.begin(), mVecDesc.end());
	id_session   = mVecDesc[0].id_session;
	load_procent = mVecDesc[0].load_procent;
	return true;
}
//---------------------------------------------------------------------------------------
void TManagerContextDownConnection_Slave::SetLoadBySession(unsigned int id_session,
																													 unsigned char load_procent)
{
	TMapUintUcharIt fit = mMapSessionLoadProcent.find(id_session);
	if(fit==mMapSessionLoadProcent.end())
	{
		BL_FIX_BUG();
		return;
	}
	fit->second = load_procent;
}
//---------------------------------------------------------------------------------------
