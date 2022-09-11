/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef DESC_RECV_SESSION_H
#define DESC_RECV_SESSION_H

#include "INetTransport.h"
#include "Base.h"
#include "Container.h"
#include "HiTimer.h"

namespace nsMMOEngine
{
	struct TDescRecvSession : public INetTransport::TDescRecv
	{
    TContainer c;

    unsigned int time_ms;
		unsigned int id_session;
		TDescRecvSession()
		{
      id_session = INVALID_HANDLE_SESSION;
			time_ms  = ht_GetMSCount();
		}
    void Assign(TDescRecvSession* p)
    {
			time_ms  = p->time_ms;
      id_session       = p->id_session;
      ip_port  = p->ip_port;
      type     = p->type;
      // ����������� ������
      c.SetData(p->data, p->sizeData);
      sizeData = p->sizeData;
      data     = (char*)c.GetPtr();
    }
	};
}
#endif