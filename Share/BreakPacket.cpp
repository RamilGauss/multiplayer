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
#include <boost/foreach.hpp>

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
  DeleteCollect();
  int size = GetSize();

  mCollect.SetData(NULL, size);// выделить память
  char* pCollect = (char*)mCollect.GetPtr();

  // копируем все в одну область памяти
  BOOST_FOREACH( TContainerPtr& c, mList )
  {
    int sizePart  =        c.GetSize();
    char* ptrPart = (char*)c.GetPtr(); 
    memcpy(pCollect, ptrPart, sizePart);
    pCollect += sizePart;
  }
}
//-----------------------------------------------------------------
void* TBreakPacket::GetCollectPtr()
{
  return mCollect.GetPtr();
}
//-----------------------------------------------------------------
int TBreakPacket::GetSize()
{
  if(mCollect.GetPtr())
    return mCollect.GetSize();
  int size = 0;

  BOOST_FOREACH( TContainerPtr& c, mList )
    size += c.GetSize();

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
void TBreakPacket::UnlinkCollect()
{
  mCollect.Unlink();
}
//-----------------------------------------------------------------
