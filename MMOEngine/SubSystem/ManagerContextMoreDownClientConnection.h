/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MANAGER_CONTEXT_MORE_DOWN_CLIENT_CONNECTION_H
#define MANAGER_CONTEXT_MORE_DOWN_CLIENT_CONNECTION_H

#include <map>
#include "DelegateManagerContextSc.h"

namespace nsMMOEngine
{
  class TContainerContextSc;
	class TManagerContextMoreDownClientConnection : public TDelegateManagerContextSc
  {
    typedef std::map<unsigned int, TContainerContextSc*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;

    typedef std::map<unsigned int, unsigned int> TMapUintUint;
    typedef TMapUintUint::iterator TMapUintUintIt;
    
    TMapUintPtr  mMapKeyContext;
    TMapUintUint mMapKeySession;
    
  public:
    TManagerContextMoreDownClientConnection();
    ~TManagerContextMoreDownClientConnection();
    
    // ���������
    bool FindSessionByClientKey(unsigned int id, unsigned int& id_session);
    TContainerContextSc* FindContextByClientKey(unsigned int id);
    // ��������� ������    
    bool SetSessionByClientKey(unsigned int id_client, 
                               unsigned int id_session);
    // ����������/��������
    TContainerContextSc* AddContext(unsigned int id_client, 
                                    unsigned int id_session);
    // ��������
    void DeleteByKey(unsigned int key);
	private:
    void Clear();
  };
}

#endif

