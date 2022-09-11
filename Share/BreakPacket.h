/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BreakPacketH
#define BreakPacketH

#include <list>

#include "TypeDef.h"
#include "ContainerPtr.h"
#include "Container.h"

/*
  �������� �� ����� �����. ��������, ����� ����� ��������� �� ������ ������ 
  �������� ����� ���������, ������ ��������� ����� ������ (+ ������ ���������)� �����������
  ���� ������, ����� ����������� ������, � �� ��������� ����� ������������ ������ � �����.
*/

class SHARE_EI TBreakPacket
{
protected:
  typedef std::list<TContainerPtr> TListC_Ptr;
  typedef TListC_Ptr::iterator TListC_PtrIt;

  TListC_Ptr mList;

  TContainer mCollect;

public:
  TBreakPacket();
  virtual ~TBreakPacket();

  // �������� ����� ������
  void PushBack(char* p,int size);
  void PushFront(char* p,int size);
  // ������� ������� � ���� ����� (������������), 
  // ������ ����� �������� ��������� �� ��������� ����� ����� GetPtr
  // ���� ���-�� ������ ����� 1, �� ������ �� �����.
  void Collect();
  void* GetCollectPtr();
  int GetSize();
  // ���-�� ������
  int GetCountPart();

  // ���������� ������, ������� �������������� ��� ��������� �����
  void DeleteCollect();

  void UnlinkPart();

  void UnlinkCollect();

  const TBreakPacket& operator =( const TBreakPacket& b );

  std::list<TContainerPtr>* GetList(){return &mList;}
protected:
};


#endif

