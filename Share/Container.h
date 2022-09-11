/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#ifndef ContainerH
#define ContainerH

#include "TypeDef.h"

/*
   опирующий контейнер
  копировать внутрь контейнера данные,
  объект сам освободит
*/

class SHARE_EI TContainer
{
protected:
  char* pData;
  int size;
public:
  TContainer();
  virtual ~TContainer();
  // освободить пам€ть
  virtual void Done();
  // копировать внутрь данные
  virtual void SetData(char* p, int s);
  // получить доступ к данным
  virtual char* GetData(int &s);
  // более коротка€ верси€ дл€ получени€ доступа
  virtual void* GetPtr()const;
  virtual int GetSize()const;
  // что бы не освобождать пам€ть с помощью Done()
  virtual void Unlink();

  static void Free(char* p){delete[]p;}
};


#endif

