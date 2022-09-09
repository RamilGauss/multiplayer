/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef List_DualLinkH
#define List_DualLinkH

//--------------------------------------------------------------------------
// ������� ����������� ������
template< typename TItem >
class TDLL_ItemT
{
public:
  TItem* mPrev;
  TItem* mNext;
  TDLL_ItemT(void) { mPrev = mNext = 0; }
  
  TItem* first() const;   // ��������� ������� ������
  TItem* last() const;    // ��������� ������� ������

};
//--------------------------------------------------------------------------
// ����������  ������, ��������������� �� ����������� ����������/�������� ��������� 
// � ������������ ������� ������
template< class TItem >
class TDLL_ListT
{
  TItem* mFirst;
  TItem* mLast;
public:

  TDLL_ListT(void) { mFirst = mLast = 0; }

  // ��� �������� ������� - �������� ������
  //~TSLL_ListT()           { deleteAll(); }

  // ���������� ��������� � ������
  int size() const;

  // ������ - ����
  bool empty() const       { return mFirst == 0; }

  // ��������� ������� ������
  TItem* first() const     { return mFirst; }

  // ��������� ������� ������
  TItem* last() const      { return mLast; }

  // ������� ��� �������� ������
  void deleteAll();

  // �������� ������� � ������ ������
  void prepend( TItem* item );

  // �������� ������� � ����� ������
  void append( TItem* item );

  // ��������� ������ ������� �� ������
  // ���������: ����������� �������
  TItem* removeFirst();

  // ��������� ��������� ������� �� ������
  // ���������: ����������� �������
  TItem* removeLast();

  // ��������� �� ������ ������� <item>
  void removeOne( TItem* item );

  // ��������� ��� �������� �� ������
  void removeAll();

  // �������� ������� <item> � ������ ����� ��������� �������� <pos>
  void insertAfter( TItem* pos, TItem* item );

  // �������� ������� <item> � ������ ����� �������� ��������� <pos>
  void insertBefore( TItem* pos, TItem* item );

  // �������� ������� � ������ � ������������ � �������� ����������
  void insert( TItem* item );

  // ��������������� ������� �������� � ������ � ������������ � �������� ����������
  void checkSortPos( TItem* item );
};
//--------------------------------------------------------------------------

#include "List_DualLink_p.h"

#endif // List_DualLinkH
