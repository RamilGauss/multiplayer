/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#ifndef HistoryPacketTCPH
#define HistoryPacketTCPH

#include <map>

#include "ContainerRise.h"
#include "ShareMisc.h"


class THistoryPacketTCP
{
public:
  struct TResult
  {
    TResult(){complete=false;}
    void Set(char* b, int s){buffer=b;size=s;complete=true;}
    bool complete;
    char* buffer;
    int size;
  };

  typedef enum{
    eSearchBegin,
    eSearchSize,
    eSearchEnd,
  }eStatePacket;

  int sizePacket;// предполагаемый размер пакета
  TContainerRise c;    
  eStatePacket   state;
  THistoryPacketTCP();
  void Clear();

  void Analiz(int& beginPos, TResult& res, int readSize, char* buffer);
protected:
  int SearchBegin(int readSize, char* buffer, int beginPos);
  int SearchSize(int readSize, char* buffer, int beginPos);
  int SearchEnd(int readSize, char* buffer, TResult& res, int beginPos);

};


#endif