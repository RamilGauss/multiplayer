/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ClientGameH
#define ClientGameH

#include <vector>
#include <string>
#include <map>

#include "IGame.h"
#include "WrapperMakerTransport.h"

/*
1. ������ ��������� ������� ���-�� ����������� (� ������������ �� ����� ���������������).
2. ���� ���������� ��������, �������� � ���������������� ������� ����� ������������.
3. �������� ������� "�������� �������" SrcEvent � "�����������" DstEvent.

*/
struct TDescThread;

class TClientGame : public IGame
{
protected:
  IClientDeveloperTool::TComponentClient mCClient;

  typedef std::vector<TDescThread> TVectorDT;
  TVectorDT   mOtherThreadVecModule;

  enum{
    eSleepNE      = 30,
    eWaitFeedBack = 30,
  };
public:
  TClientGame();
  virtual ~TClientGame();
protected:
  virtual bool Init(int variant_use, 
                    const char* sNameDLL, 
                    std::vector<std::string>& arg);
  virtual void Done();
  virtual bool Work();

  virtual void MakeVectorModule();
  virtual void CollectEvent();

  bool HandleGraphicEngineEvent();
  bool HandleNetEngineEvent();
  bool HandleTimerFirstEvent();
  bool HandleTimerLastEvent();

  virtual void HandleEvent(nsEvent::TEvent* pEvent);

  void Render();

  void StartThreadModule();
  void StopThreadModule();
};
#endif
