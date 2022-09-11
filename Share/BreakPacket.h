/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013, 2013, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
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

  const TBreakPacket& operator =( const TBreakPacket& b );

  std::list<TContainerPtr>* GetList(){return &mList;}

protected:

};


#endif
