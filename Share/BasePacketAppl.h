/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#ifndef BasePacketApplH
#define BasePacketApplH

#include "MarkUpContainer.h"

// готовые макросы:
/*
    GetSet(Type,NameParam)
    GetSetCount(TypeCnt, TypeElem, NameParam )
*/
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define GetByPtr(Type,NameParam) \
Type* p = (Type*)mC.Get(#NameParam);

#define GetElemByPtr(Type, NameParam, index) \
Type* p = (Type*)mC.Get(#NameParam, false, index);
//----------------------------------------------------------------------------------------
#define GetSet(Type,NameParam) \
void Get##NameParam(Type& v) \
{ \
  GetByPtr(Type,NameParam) \
  v = *p; \
} \
void Set##NameParam(Type& v) \
{ \
  GetByPtr(Type,NameParam) \
  *p = v; \
}
//----------------------------------------------------------------------------------------
#define GetSetCount(TypeCnt, TypeElem, NameParam ) \
void SetCount##NameParam(TypeCnt v) \
{ \
  GetByPtr(TypeCnt,NameParam) \
  *p = v; \
  mC.Update(); \
} \
void GetCount##NameParam(TypeCnt& v) \
{ \
  GetByPtr(TypeCnt,NameParam) \
  v = *p; \
} \
void Set##NameParam(TypeElem& v, TypeCnt index) \
{ \
  GetElemByPtr(TypeElem,NameParam,index) \
  *p = v; \
} \
void Get##NameParam(TypeElem& v, TypeCnt index) \
{ \
  GetElemByPtr(TypeElem,NameParam,index) \
  v = *p; \
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

class SHARE_EI TBasePacketAppl
{
protected:

  TMarkUpContainer mC;
 
public:

	TBasePacketAppl();
  virtual ~TBasePacketAppl();
  
  
  virtual bool Set(void* p, int size);
  virtual void* Get(int &size);
  
  virtual int GetType() = 0;
	
protected:

  virtual void Init() = 0;


};

#endif

