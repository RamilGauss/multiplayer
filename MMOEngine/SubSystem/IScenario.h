/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ISCENARIO_H
#define ISCENARIO_H

#include "SrcEvent.h"

#include "DescRecvSession.h"
#include "CallBackRegistrator.h"

#include "MapCallBack.h"

namespace nsMMOEngine
{
  class IContextScenario;
	class TManagerSession;
  class IScenario : public TMapCallBack
  {
		TCallBackRegistrator1<unsigned int> mCBNeedContext;
    TCallBackRegistrator1<IScenario*>   mCBEnd;
    // ������ �� �������� �� ����� �������
    TCallBackRegistrator1<unsigned int>              mCBContextByClientKey;

    unsigned char mType;
  protected:
    IContextScenario* mCurContext;
  public:
    // ���� CallBack
    enum
    {
      eContextBySession = 0,
      eEnd,
      eContextByClientKey,
      eCountCallBack,
    };

#if defined( WIN32 )
#pragma pack(push, 1)
#endif
    struct TBaseHeader{char type;char subType;};
#if defined( WIN32 )
#pragma pack(pop)
#endif
    IScenario();
    virtual ~IScenario();
    
    void SetContext(IContextScenario* pCSc);
    IContextScenario* GetContext();

		unsigned char GetType();
		void SetType(unsigned char type);

		virtual void Recv(TDescRecvSession* pDesc) = 0;
  protected:
    friend class IContextScenario;
    // ���� ������ ���� ������ �������� �����, �� ����� ����� ����� ����������� ���������� ���� �����
    virtual void DelayBegin();
    // ��������� ����� ������� � ��������
    virtual void Work();
  public:
    bool Begin();
    void End();
    // ������ �� ����� ������, ��� ����������������� �������� �������� � ������� SetContext()
    void NeedContext(unsigned int id_session);
    void NeedContextByClientKey(unsigned int id_client);
  };
}

#endif
