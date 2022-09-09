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

#include "ApplicationProtocolPacketAnswer.h"

// ushort type, 
// uchar code (см. enum{eFight..), 
// ushort codeMap - код карты только если code==eFight
// uchar - кол-во танков в команде
// массив длинной CountTankInCommand*sizeof(DefTank)

TA_In_Fight::TA_In_Fight(): TBasePacket()
{
  mType=APPL_TYPE_A_IN_FIGHT;
  mSize = eLenHeader;
  mData = (char*)malloc(mSize);
  setType();

  setCountTank(0);// иначе будет полный пиздец
}
//-------------------------------------------------------------------------------------------
unsigned char TA_In_Fight::getCode()
{
  return *getPointerCode();
} 
//-------------------------------------------------------------------------------------------
void TA_In_Fight::setCode(unsigned char val)
{
  *getPointerCode() = val;
}
//-------------------------------------------------------------------------------------------
unsigned char* TA_In_Fight::getPointerCode()
{
  return (unsigned char*)mData+sizeof(mType);
} 
//-------------------------------------------------------------------------------------------
unsigned short TA_In_Fight::getCodeMap()
{
  return *getPointerCodeMap();
}
//-------------------------------------------------------------------------------------------
void TA_In_Fight::setCodeMap(unsigned short val)
{
  *getPointerCodeMap()=val;
}
//-------------------------------------------------------------------------------------------
unsigned short* TA_In_Fight::getPointerCodeMap()
{
  return (unsigned short*)(mData+sizeof(mType)+sizeof(unsigned char));
}
//-------------------------------------------------------------------------------------------
unsigned char TA_In_Fight::getCountTank()
{
  return *getPointerCountTank();
}
//-------------------------------------------------------------------------------------------
unsigned char* TA_In_Fight::getPointerCountTank()
{
  return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned short));
}
//-------------------------------------------------------------------------------------------
void TA_In_Fight::setCountTank(unsigned char cnt)
{
  *getPointerCountTank() = cnt;
}
//-------------------------------------------------------------------------------------------
unsigned char* TA_In_Fight::getPointerTankProperty(int i)
{
  return FindTank(i)+sizeof(unsigned short)/*плюс размер "длина"*/;
}
//-----------------------------------------------------------------------------
unsigned short TA_In_Fight::getSizeTankProperty(int i)
{
  unsigned int size = *((unsigned short*)FindTank(i));
  return size;
}
//-----------------------------------------------------------------------------
void TA_In_Fight::addTankProperty(unsigned char* pData, unsigned short size)
{
  // выделить память больше на размер свойств танка + размер поля "длина" 
  mData = (char*)mo_realloc(mData,mSize,mSize+size+sizeof(unsigned short));
  // новый размер
  mSize+=size+sizeof(unsigned short);

  unsigned char cnt = getCountTank();
  unsigned char* pLastTank = FindTank(cnt);
  //----------------------------------------
  memcpy(pLastTank+sizeof(unsigned short),pData,size);
  unsigned short* pSize = (unsigned short*)pLastTank;
  *pSize = size;
  //----------------------------------------
  cnt++;
  setCountTank(cnt);
}
//-----------------------------------------------------------------------------
unsigned char* TA_In_Fight::FindTank(int i)
{
  int iFound = 0;

  unsigned short* pSize;
  unsigned short size;
  unsigned char* p = (unsigned char*)mData+eLenHeader;

  pSize = (unsigned short*)p;
  size = *pSize;

  while(iFound!=i)
  {
    p += sizeof(size)+size;  

    pSize = (unsigned short*)p;
    size = *pSize;
    iFound++;
  }

  return p;
}
//-----------------------------------------------------------------------------
