/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_GROUP_CLIENT_PRIVATE_H
#define MELISSA_GROUP_CLIENT_PRIVATE_H

#include <set>

namespace nsMelissa
{
  class TClientForMasterPrivate;
	class TGroupClientPrivate
	{
    typedef std::set<TClientForMasterPrivate*> TSetUintPtr;
    TSetUintPtr mSetClient;

    unsigned int mID;
  public:	
    typedef enum{
      eReady,// ������������ 
      eConnect,// �������������� 
    }tState;

		TGroupClientPrivate();

    void SetID(unsigned int id);
    unsigned int GetID();

    void SetClient(std::set<TClientForMasterPrivate*>& setClient);
    std::set<TClientForMasterPrivate*>& GetClient();

    void SetState(tState state);
    tState GetState();
  protected:
    tState mState;
	};
}  
#endif