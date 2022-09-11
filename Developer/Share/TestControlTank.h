/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef TestControlTankH
#define TestControlTankH

#include "Container.h"
#include "GCS.h"

class TTankTower;

// ������� Developer ��������� ��� ���������� ������


class TTestControlTank
{
#if defined( WIN32 )
#pragma pack(push, 1)
#endif
	struct TDesc
	{
		float mSpeedRotateTurret;
		float mAngleTurret;

		float mSpeedRotateGun;
		float mAngleGun;
		TDesc()
		{
			mSpeedRotateTurret = 0;
			mAngleTurret    	 = 0;
			mSpeedRotateGun 	 = 0;
			mAngleGun       	 = 0;
		}
	};
#if defined( WIN32 )
#pragma pack(pop)
#endif

	TTankTower* mTank;

	unsigned int mTimeRecv_ms;
	unsigned int mTimeLastUpdate_ms;
	
	TDesc mDesc;

	GCS mMutexQtSend;

	void lockQtSend()  {mMutexQtSend.lock();}
	void unlockQtSend(){mMutexQtSend.unlock();}

public:
  TTestControlTank();

	// ��������� ������
	void SetTank(TTankTower* pTank);
	// �������� ��� ������������� ���������� � ���������� ��� �������
	void SetupParamForNow();
	
public:
	void Recv(void* data, int size);
	void Send();

	// ������� �� ������� ������ � �����
	// ������ ��������
	void SetSpeedRotateTurret(float v);
	void SetSpeedRotateGun(float v);
private:
	void UpdateAngle();
};


#endif