/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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

#include "MeshFileBj.h"
#include "SaveOnHDD.h"
#include "LoadFromHDD.h"
#include "IXML.h"

using namespace nsMeshStruct;


TMeshFileBj::TMeshFileBj()
{
  pReadData = NULL;
}
//-----------------------------------------------------------------------------------
TMeshFileBj::~TMeshFileBj()
{

}
//-----------------------------------------------------------------------------------
bool TMeshFileBj::Load(const char* strFilename, 
            VERTEX*&      pVertex, unsigned int& cntV,
            unsigned int*& pIndex, unsigned int& cntI)
{
  Done();
  /*
    4 байта - число точек (DWORD)
     точки по 32 байта
    все остальное - это индексы
  */
  TLoadFromHDD loader;
  if(loader.ReOpen((char*)strFilename)==false) return false;
  int size = loader.Size();

  pReadData = new char[size];
  loader.Read(pReadData,size);
  char* buffer = pReadData;

  // вершины
  cntV = *((unsigned int*)buffer);
  pVertex = new VERTEX[cntV];
  buffer += sizeof(cntV);
  memcpy(pVertex,buffer,cntV*sizeof(VERTEX));

  // индексы
  cntI = (size - sizeof(cntV)-cntV*sizeof(VERTEX))/sizeof(unsigned int);
  pIndex = new unsigned int[cntI];
  buffer += cntV*sizeof(VERTEX);
  memcpy(pIndex, buffer, cntI*sizeof(unsigned int));

  return true;
}
//-----------------------------------------------------------------------------------
bool TMeshFileBj::Save(const char* strFilename, 
                  VERTEX*       pVertex, unsigned int cntV,
                  unsigned int* pIndex,  unsigned int cntI) 
{
  TSaveOnHDD saver;
  CHECK_RET(saver.ReOpen((char*)strFilename));

  saver.Write(&cntV,sizeof(cntV));// кол-во точек
  // сохранить вершины
  saver.Write( pVertex, cntV*sizeof( VERTEX ) );
  saver.Write( pIndex, cntI*sizeof( unsigned int ) );
  return true;
}
//-----------------------------------------------------------------------------------
void TMeshFileBj::Done()
{
  delete[] pReadData;
  pReadData = NULL;
}
//-----------------------------------------------------------------------------------
