/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SrcEventH
#define SrcEventH

#include "TypeDef.h"
#include <string>

class TDstEvent;

/*
  �������� �������. �������� � ������ � TDstEvent
  ����������������,
  ��������� ������� AddEvent
*/


class SHARE_EI TSrcEvent
{
  int sSelfID;
  TDstEvent* pDstEvent;

public:

  TSrcEvent();
  virtual ~TSrcEvent();

  void SetSelfID(int selfID);

  void SetDstObject(TDstEvent* p);
  // ����� ��������� ������� ����� �������� 
  // �������� �������
  void AddEventCopy(void* data, int size);
  void AddEventCopy(void* data, int size, unsigned int time_create_ms);
  void AddEventWithoutCopy(void* data, int size);
  void AddEventWithoutCopy(void* data, int size, unsigned int time_create_ms);
};

#endif
