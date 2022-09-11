/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_ISCENARIO_H
#define MELISSA_ISCENARIO_H

#include "SrcEvent.h"

#include "DescRecvSession.h"
#include "CallBackRegistrator.h"

namespace nsMelissa
{
  class IContextScenario;
	class TManagerSession;
  class IScenario
  {
		TCallBackRegistrator1<unsigned int> mCallBackNeedContext;
    TCallBackRegistrator1<IScenario*> mCallBackEnd;
    unsigned char mType;
  protected:
    IContextScenario* mCurContext;
  public:
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
		// �������� F: void C::Func(IScenario* pSc);
    template <typename F, class C>
    void RegisterOnNeedContext(F f, C pObject)// �������� ������� ��������� ������
    {mCallBackNeedContext.Register(f,pObject);}
    template <typename F, class C>
		void RegisterOnEnd(F f, C pObject)// �������� ������� ��������� �������� (��������� �� ������� ���������)
		{mCallBackEnd.Register(f,pObject);}
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
  };
}

#endif
