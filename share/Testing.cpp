/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "Testing.h"


#include <list>
#include "ApplicationProtocolPacketAnswer.h"

class TClass
{
public:
  TClass(int b)
  {
    a = b;
  }
  ~TClass()
  {
    a = -1;
  }
  int a;
};


void Testing(void* p, int* size)
{
  TA_Correct_Packet_State_Tank packet;
  // расчет координат возможен до загрузки карты, все данные хранятся у предсказателя
  int cnt = 2;
  packet.setCountTank(cnt);
  for(int i = 0 ; i < cnt ; i++)
  {
    packet.setID(i,i);
    packet.setX(i,0);
    packet.setY(i,1);  
    packet.setZ(i,2);
    packet.setXV(i,3);
    packet.setYV(i,4);
    packet.setZV(i,5);
  }

  return ;

  std::list<TClass*> l;
  TClass* pClass = new TClass(1);
  l.push_back(pClass);
  pClass = new TClass(2);
  l.push_back(pClass);


  std::list<TClass*>::iterator it = l.begin();
  std::list<TClass*>::iterator eit = l.end();
  while(it!=eit)
  {
    (*it)->a = 10;
    delete *it;
    it++;
  }

  l.clear();

}