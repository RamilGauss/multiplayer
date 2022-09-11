/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_SLAVE_PRIVATE_H
#define MELISSA_SLAVE_PRIVATE_H

#include <set>

#include "ContainerContextSc.h"
#include <vector>

namespace nsMelissa
{
	class TSlavePrivate
	{
    enum{eDefCountClient=100};

    unsigned int mID_SessionDown;

    unsigned char mLoadProcent;

    typedef std::set<unsigned int> TSetUint;
    TSetUint mSetClientKey;

    int mCountFreeClient;
    int mCountGroupClient;

    TContainerContextSc mC;

  public:	
    typedef enum{
      eReady,// ������������ 
      eConnect,// �������������� 
    }tState;

		TSlavePrivate();

    void SetID_Session(unsigned int v);
    unsigned int GetID_Session();

    void SetLoadProcent(unsigned char v);
    unsigned char GetLoadProcent();

    tState GetState();
    void SetState(tState state);
  
    void GetVectorID_client(std::vector<unsigned int>& vID_client);

    void AddClient(std::list<unsigned int>& l_id_client);
    void RemoveClient(std::list<unsigned int>& l_id_client);
    // ��� Add � Remove ( count = -10, ��������)
    void AddDeltaGroupClient(int count);
    void AddDeltaFreeClient(int count);

    // ������ ���������� ���-�� ��������, ������� ������ ��������� Slave
    int CalcCountClientForEmpty();
    
    int GetLoadProcentFreeClient();
    int GetLoadProcentGroupClient();

    // ������� �������� �������� �� ������
    int GetCountClient(std::list<unsigned int>& l_id_client);

    TContainerContextSc* GetC();
  protected:
    tState mState;
	};
}  
#endif