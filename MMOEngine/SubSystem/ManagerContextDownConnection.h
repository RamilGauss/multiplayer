/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MANAGER_CONTEXT_DOWN_CONNECTION_H
#define MANAGER_CONTEXT_DOWN_CONNECTION_H

#include <map>
#include <set>
#include "DelegateManagerContextSc.h"

namespace nsMMOEngine
{
  class TContainerContextSc;
  class TManagerContextDownConnection : public TDelegateManagerContextSc
  {
    typedef std::map<unsigned int, TContainerContextSc*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;

    typedef std::set<unsigned int> TSetUint;
    typedef TSetUint::iterator TSetUintIt;

    typedef std::map<unsigned int, TSetUint > TMapUintSetUint;
    typedef TMapUintSetUint::iterator TMapUintSetUintIt;
    
    TMapUintPtr     mMapSessionContext;
    TMapUintSetUint mMapSessionKey;
    
  public:
    TManagerContextDownConnection();
    virtual ~TManagerContextDownConnection();
    
    // ���������
    TContainerContextSc* FindContextBySession(unsigned int id_session);
    
    int GetCountSession();
    bool GetSessionByIndex( int index, unsigned int& id_session);

    int GetCountClientKey(unsigned int id_session_master);
    bool GetClientKeyByIndex(unsigned int id_session_master, 
                             int index, 
                             unsigned int& id_client);

    // ����������/��������
    TContainerContextSc* AddContext(unsigned int id_session);
    bool AddClientKey(unsigned int id_session, 
                      unsigned int id_client);

    // ��������
    void DeleteByClientKey(unsigned int id_session, unsigned int id_client);
    void DeleteContextBySession(unsigned int id_session);
	protected:
		virtual void AddSessionEvent(unsigned int id_session){}
		virtual void DeleteSessionEvent(unsigned int id_session){}
	private:
    void Clear();
  };
}

#endif

