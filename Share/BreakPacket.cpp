/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 



#include "BreakPacket.h"
#include <algorithm>

using namespace std;

TBreakPacket::TBreakPacket()
{

}
//-----------------------------------------------------------------
TBreakPacket::~TBreakPacket()
{
  DeleteCollect();
}
//-----------------------------------------------------------------
void TBreakPacket::PushBack(char* p,int size)
{
  TContainerPtr cPtr;
  cPtr.SetData(p, size);

  mList.push_back(cPtr);
}
//-----------------------------------------------------------------
void TBreakPacket::PushFront(char* p,int size)
{
  TContainerPtr cPtr;
  cPtr.SetData(p, size);

  mList.push_front(cPtr);
}
//-----------------------------------------------------------------
void TBreakPacket::Collect()
{
  if(mList.size()<=1) return;// Gauss для одно пакетного не надо делать сборку (оптимизация) 03.06.2013

  DeleteCollect();
  int size = GetSize();

  mCollect.SetData(NULL, size);// выделить память
  char* pCollect = (char*)mCollect.GetPtr();

  TListC_PtrIt bit = mList.begin();
  TListC_PtrIt eit = mList.end();
  while(bit!=eit)
  {
    int sizePart  =        bit->GetSize();
    char* ptrPart = (char*)bit->GetPtr(); 
    memcpy(pCollect, ptrPart, sizePart);
    pCollect += sizePart;

    bit++;
  }
}
//-----------------------------------------------------------------
void* TBreakPacket::GetCollectPtr()
{
  if(mList.size()==1)//Gauss для одно пакетного не надо делать сборку (оптимизация) 03.06.2013
  {
    return mList.begin()->GetPtr();
  }
  else 
    if(mList.size()==0)
      return NULL;

  return mCollect.GetPtr();
}
//-----------------------------------------------------------------
int TBreakPacket::GetSize()
{
  if(mCollect.GetPtr())
    return mCollect.GetSize();
  int size = 0;

  TListC_PtrIt bit = mList.begin();
  TListC_PtrIt eit = mList.end();
  while(bit!=eit)
  {
    size += bit->GetSize();
    bit++;
  }
  return size;
}
//-----------------------------------------------------------------
int TBreakPacket::GetCountPart()
{
  return mList.size();
}
//-----------------------------------------------------------------
void TBreakPacket::DeleteCollect()
{
  mCollect.Done();
}
//-----------------------------------------------------------------
void TBreakPacket::UnlinkPart()
{
  mList.clear();
}
//-----------------------------------------------------------------	
const TBreakPacket& TBreakPacket::operator =( const TBreakPacket& b )
{
	mList = b.mList;
	return *this;
}
//-----------------------------------------------------------------