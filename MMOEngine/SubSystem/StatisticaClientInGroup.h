/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef STATISTICA_CLIENT_IN_GROUP_H
#define STATISTICA_CLIENT_IN_GROUP_H

#include <map>
#include <set>
#include <vector>

namespace nsMMOEngine
{
  class TStatisticaClientInGroup
  {
		struct TDesc
		{
			unsigned int countClient;
			unsigned int id_session;

			TDesc(unsigned int cc,unsigned int is)
			{
				countClient = cc;
				id_session  = is;
			}
			bool operator <(TDesc& right)
			{
				if(countClient < right.countClient)
					return true;
				return false;
			}
		};

		typedef std::vector<TDesc> TVectorDesc;
		typedef TVectorDesc::iterator TVectorDescIt;

		TVectorDesc mVecDesc;

    typedef std::set<unsigned int> TSetUint;
    typedef TSetUint::iterator TSetUintIt;

    typedef std::map<unsigned int, TSetUint > TMapUintSetUint;
    typedef TMapUintSetUint::iterator TMapUintSetUintIt;
    
    TMapUintSetUint mMapSlaveSessionClientKey;

	public:
    TStatisticaClientInGroup();
    ~TStatisticaClientInGroup();
    
    // ���������
    bool FindSlaveSessionByMinimumClient(unsigned int &id_session_slave);
    bool FindCountClientBySlaveSession(unsigned int id_session_slave, int& countClient);
    // ����������
    void AddSlave(unsigned int id_session);
    // �������� � ������ �������
    void AddBySlaveSessionClientKey(unsigned int id_session_slave, unsigned int id_client);

    // ��������
    void DeleteBySlaveSession(unsigned int id_session_slave);
		void DeleteByClientKey(unsigned int id_session_slave, unsigned int key);
  private:
  };
}

#endif

