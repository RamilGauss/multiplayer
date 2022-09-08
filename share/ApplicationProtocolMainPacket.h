/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ApplicationProtocolMainPacketH
#define ApplicationProtocolMainPacketH

#include "ApplicationProtocolPacket.h"
#include "memory_operation.h"

#if defined( WIN32 )
#pragma pack(push, 1)
#endif


//APPL_TYPE_G_S_FIGHT_COORD_BULLET
class TS_Fight_Coord_Bullet : public TBasePacket
{
  // ushort type, 
  // uchar cntTank
  // uchar cntBullet
  // float * 7 + 1 - DefTank
  // float * 6 - DefBullet
public:  
  struct TLocalTank
  {
    unsigned short id;
    // центра танка
    float x;
    float y;
    float z;
    // перед танка
    float xv;
    float yv;
    float zv;
    // скал€р скорости
    float v;
    // флаг нажатых клавиш
    unsigned char UP    : 2;
    unsigned char DOWN  : 2;
    unsigned char LEFT  : 2;
    unsigned char RIGHT : 2;
  };
  struct TLocalBullet
  {
    // начало полета
    float x;
    float y;
    float z;
    // вектор скорости, координаты через 1 секунду
    float xv;
    float yv;
    float zv;
  };
  enum{
    eSizeHeader=sizeof(unsigned short)+sizeof(unsigned char)+sizeof(unsigned char),
  };
public:
  TS_Fight_Coord_Bullet()
  {
    mType=APPL_TYPE_G_S_FIGHT_COORD_BULLET;
    mSize = eSizeHeader;
    mData = (char*)malloc(mSize);
    setType();
    setCntTank(0);
    setCntBullet(0);
  }
  //-------------------------------------------------------------------
  void setCntTank(unsigned int val)// всегда val>val_old
  {
    int size_paste = val*sizeof(TLocalTank);
    int size_bound = eSizeHeader;
    mData = (char*)mo_realloc_bound(mData,mSize,size_bound,size_paste);
    mSize += size_paste;

    unsigned char* pCnt = getPointerCntTank();
    *pCnt = val;
  }
  unsigned char getCntTank(){return *getPointerCntTank();};
  unsigned char* getPointerCntTank(){return (unsigned char*)(mData+sizeof(mType));};
  //-------------------------------------------------------------------
  void setCntBullet(unsigned int val)// всегда val>val_old
  {
    int size_paste = val*sizeof(TLocalBullet);
    int size_bound = eSizeHeader+getCntTank()*sizeof(TLocalTank);
    mData = (char*)mo_realloc_bound(mData,mSize,size_bound,size_paste);
    mSize += size_paste;

    unsigned char* pCnt = getPointerCntBullet();
    *pCnt = val;
  }
  unsigned char getCntBullet(){return *getPointerCntBullet();}
  unsigned char* getPointerCntBullet(){return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char));}
  //-------------------------------------------------------------------
  void setTank_ID(int i,float id){*getPointerTank_ID(i)=id;}
  unsigned short getTank_ID(int i){return *getPointerTank_ID(i);}
  unsigned short* getPointerTank_ID(int i){return (unsigned short*)(mData+eSizeHeader+i*sizeof(TLocalTank));}//0
  //-------------------------------------------------------------------
  void setTank_X(int i,float x){*getPointerTank_X(i)=x;}
  float getTank_X(int i){return *getPointerTank_X(i);}
  float* getPointerTank_X(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short));}//0
  //-------------------------------------------------------------------
  void setTank_Y(int i,float y){*getPointerTank_Y(i)=y;}
  float getTank_Y(int i){return *getPointerTank_Y(i);}
  float* getPointerTank_Y(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                     sizeof(float));}//1
  //-------------------------------------------------------------------
  void setTank_Z(int i,float z){*getPointerTank_Z(i)=z;}
  float getTank_Z(int i){return *getPointerTank_Z(i);}
  float* getPointerTank_Z(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                     sizeof(float)+sizeof(float));}//2
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  void setTank_XV(int i,float x){*getPointerTank_XV(i)=x;}
  float getTank_XV(int i){return *getPointerTank_XV(i);}
  float* getPointerTank_XV(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                      sizeof(float)+sizeof(float)+sizeof(float));}//3
  //-------------------------------------------------------------------
  void setTank_YV(int i,float y){*getPointerTank_YV(i)=y;}
  float getTank_YV(int i){return *getPointerTank_YV(i);}
  float* getPointerTank_YV(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                      sizeof(float)+sizeof(float)+sizeof(float)+//4
                                                                      sizeof(float));}
  //-------------------------------------------------------------------
  void setTank_ZV(int i,float z){*getPointerTank_ZV(i)=z;}
  float getTank_ZV(int i){return *getPointerTank_ZV(i);}
  float* getPointerTank_ZV(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                      sizeof(float)+sizeof(float)+sizeof(float)+//5
                                                                      sizeof(float)+sizeof(float));}
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  void setTank_V(int i,float v){*getPointerTank_V(i)=v;}
  float getTank_V(int i){return *getPointerTank_V(i);}
  float* getPointerTank_V(int i){return (float*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                     sizeof(float)+sizeof(float)+sizeof(float)+//6
                                                                     sizeof(float)+sizeof(float)+sizeof(float));}
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  void setTank_State(int i,unsigned int state){*getPointerTank_State(i)=state;}
  unsigned int getTank_State(int i){return *getPointerTank_State(i);}
  unsigned int* getPointerTank_State(int i){return (unsigned int*)(mData+eSizeHeader+i*sizeof(TLocalTank)+sizeof(unsigned short)+
                                                                                       sizeof(float)+sizeof(float)+sizeof(float)+//7
                                                                                       sizeof(float)+sizeof(float)+sizeof(float)+
                                                                                       sizeof(float));}
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  void setBullet_X(int i,float x){*getPointerBullet_X(i)=x;}
  float getBullet_X(int i){return *getPointerBullet_X(i);}
  float* getPointerBullet_X(int i){return (float*)(mData+eSizeHeader+sizeof(TLocalTank)*getCntTank()+i*sizeof(TLocalBullet));}//0
  //-------------------------------------------------------------------
  void setBullet_Y(int i,float y){*getPointerBullet_Y(i)=y;}
  float getBullet_Y(int i){return *getPointerBullet_Y(i);}
  float* getPointerBullet_Y(int i){return (float*)(mData+eSizeHeader+sizeof(TLocalTank)*getCntTank()+i*sizeof(TLocalBullet)+
                                                                     sizeof(float));}//1
  //-------------------------------------------------------------------
  void setBullet_Z(int i,float z){*getPointerBullet_Z(i)=z;}
  float getBullet_Z(int i){return *getPointerBullet_Z(i);}
  float* getPointerBullet_Z(int i){return (float*)(mData+eSizeHeader+sizeof(TLocalTank)*getCntTank()+i*sizeof(TLocalBullet)+
                                                                     sizeof(float)+sizeof(float));}//2
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------
  void setBullet_XV(int i,float x){*getPointerBullet_XV(i)=x;}
  float getBullet_XV(int i){return *getPointerBullet_XV(i);}
  float* getPointerBullet_XV(int i){return (float*)(mData+eSizeHeader+sizeof(TLocalTank)*getCntTank()+i*sizeof(TLocalBullet)+
                                                                      sizeof(float)+sizeof(float)+sizeof(float));}//3
  //-------------------------------------------------------------------
  void setBullet_YV(int i,float y){*getPointerBullet_YV(i)=y;}
  float getBullet_YV(int i){return *getPointerBullet_YV(i);}
  float* getPointerBullet_YV(int i){return (float*)(mData+eSizeHeader+sizeof(TLocalTank)*getCntTank()+i*sizeof(TLocalBullet)+
                                                                      sizeof(float)+sizeof(float)+sizeof(float)+//4
                                                                      sizeof(float));}
  //-------------------------------------------------------------------
  void setBullet_ZV(int i,float z){*getPointerBullet_ZV(i)=z;}
  float getBullet_ZV(int i){return *getPointerBullet_ZV(i);}
  float* getPointerBullet_ZV(int i){return (float*)(mData+eSizeHeader+sizeof(TLocalTank)*getCntTank()+i*sizeof(TLocalBullet)+
                                                                      sizeof(float)+sizeof(float)+sizeof(float)+//5
                                                                      sizeof(float)+sizeof(float));}
  //-------------------------------------------------------------------

};

#if defined( WIN32 )
#pragma pack(pop)
#endif


#endif