/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef List_SingleLinkH
#define List_SingleLinkH

#include "BL_IOStream.h"
//--------------------------------------------------------------------------
// ������� ������������ ������
template< typename TItem >
class TSLL_ItemT
{
public:
  TItem* mNext;
  TSLL_ItemT(void) { mNext = 0; }

  // ��������� ������� ������
  TItem* last() const;
};
//--------------------------------------------------------------------------
// ����������� ������, ��������������� �� ����������� ���������� ��������� � ������ ������
template< class TItem >
class TSLL_ListT
{
protected:
  TItem* mFirst;
public:

  TSLL_ListT(void) { mFirst = 0; }

  // ��� �������� ������� - �������� ������
  //~TSLL_ListT()           { deleteAll(); }

  // ������ - ����
  bool empty() const       { return mFirst == 0; }
  
  // ���������� ��������� � ������
  int size() const;

  // ������ ������� ������
  TItem* first() const     { return mFirst; }
  
  // ��������� ������� ������
  TItem* last() const      { return mFirst ? mFirst->last() : NULL; }

  // ������� ��� �������� ������
  void deleteAll();

  // �������� ������� � ������ ������
  void prepend( TItem* item );

  // �������� � ������ ������ �������� �� ������� ������
  void prepend( TSLL_ListT* list );
  
  // ��������� ������ ������� �� ������
  // ���������: ����������� �������
  TItem* removeFirst();

  // ��������� �� ������ ������� <item>
  void removeOne( TItem* item );

  // ��������� ��� �������� �� ������
  void removeAll();

  // ��������� ������ � �����
  bool save( TIOStream& od ) const;

  // ��������� ������ �� ������
  bool load( TIOStream& od );
};
//--------------------------------------------------------------------------
// ����������� ������, ��������������� �� ����������� ���������� ��������� 
// ��� � ������ ������, ��� � � �����
template< class TItem >
class TSLL_ListET : public TSLL_ListT<TItem>
{
  typedef TSLL_ListT<TItem> inherited;
protected:
  TItem* mLast;
public:

  TSLL_ListET(void)   { mLast = 0; }

  // ��������� ������� ������
  TItem* last()  const        { return mLast; }

  // ������� ��� �������� ������
  void deleteAll();

  // �������� ������� � ������ ������
  void prepend( TItem* item );

  // �������� ������� � ����� ������
  void append( TItem* item );

  // �������� ������ � ����� ������
  void appendList( TItem* item );
  void appendList( TSLL_ListET<TItem>& list );

  // ��������� ������ ������� �� ������
  // ���������: ����������� �������
  TItem* removeFirst();

  // ��������� �� ������ ������� <item>
  void removeOne( TItem* item );

  // ��������� ��� �������� �� ������
  void removeAll();

  // ��������� ������ � �����
  //bool save( ROutData od ) const; -  ��������� ������������� ������

  // ��������� ������ �� ������
  bool load( TIOStream& od );
};
//--------------------------------------------------------------------------

#include "List_SingleLink_p.h"

#endif
