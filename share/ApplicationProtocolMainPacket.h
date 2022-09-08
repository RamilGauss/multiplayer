#ifndef ApplicationProtocolMainPacketH
#define ApplicationProtocolMainPacketH

#include "ApplicationProtocolPacket.h"

#if defined( WIN32 )
#pragma pack(push, 1)
#endif


//APPL_TYPE_S_FIGHT_COORD_BULLET
class TS_Fight_Coord_Bullet : public TBasePacket
{
  // ushort type, unsigned short
public:  
  struct TLocalTank
  {
    // центра танка
    unsigned int xC;
    unsigned int yC;
    
    // перед танка
    unsigned int xF;// forward
    unsigned int yF;

    // скаляр скорости
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
    unsigned int x0;
    unsigned int y0;
    // вектор скорости, координаты через 1 секунду
    unsigned int x1;
    unsigned int y1;
  };
protected:
  unsigned short mCntTanks;
  TLocalTank* mArrLocalTank;
  unsigned short mCntBullet;
  TLocalBullet* mArrBullet;

public:
  TS_Fight_Coord_Bullet()
  {mType=APPL_TYPE_S_FIGHT_COORD_BULLET;mCntTanks=0;mCntBullet=0;mArrLocalTank=NULL;mArrBullet=NULL;};
  //-------------------------------------------------------------------
  void setCntTank(unsigned int val)
  {
    int new_size = sizeof(mType)                  + 
                   sizeof(mCntTanks)              +
                   val*sizeof(TLocalTank)         + 
                   sizeof(mCntBullet)             +
                   mCntBullet*sizeof(TLocalBullet);
    char* volatily_buffer = (char*)malloc(new_size);
    mCntTanks = val;
    Resize(volatily_buffer);
    mSize = new_size;
  }
  unsigned short getCntTank(){return mCntTanks;};
  unsigned short* getPointerCntTank(){return &mCntTanks;};
  //-------------------------------------------------------------------
  void setCntBullet(unsigned int val)
  {
    int new_size = sizeof(mType)                  + 
                   sizeof(mCntTanks)              +
                   mCntTanks*sizeof(TLocalTank)   + 
                   sizeof(mCntBullet)             +
                   val*sizeof(TLocalBullet);
    char* volatily_buffer = (char*)malloc(new_size);
    
    mCntBullet = val;
    Resize(volatily_buffer);
    mSize = new_size;
  }
  unsigned short getCntBullet(){return mCntBullet;};
  unsigned short* getPointerCntBullet(){return &mCntBullet;};
  //-------------------------------------------------------------------
  void Resize(char* volatily_buffer)
  {
    char* p = volatily_buffer;
    memcpy(p,&mType,       sizeof(mType));                   
    p+=sizeof(mType);
    
    memcpy(p,&mCntTanks,   sizeof(mCntTanks));               
    p+=sizeof(mCntTanks);
    
    if(mArrLocalTank)
      memcpy(p,mArrLocalTank,mCntTanks*sizeof(TLocalTank));    
    mArrLocalTank = (TLocalTank*)p;
    p+=mCntTanks*sizeof(TLocalTank);
    
    memcpy(p,&mCntTanks,   sizeof(mCntBullet));              
    p+=sizeof(mCntBullet);
    
    if(mArrBullet)
      memcpy(p,mArrBullet,mCntBullet*sizeof(TLocalBullet)); 
    mArrBullet = (TLocalBullet*)p;
    p+=mCntBullet*sizeof(TLocalBullet);
    
    free(mData);
    mData = volatily_buffer;
  }
  //-------------------------------------------------------------------
  TLocalTank* getArrTank()  {return mArrLocalTank;};
  void   setArrTank(char* pArr){memcpy(mArrLocalTank,pArr,mCntTanks*sizeof(TLocalTank));}
  
  TLocalBullet* getArrBullet(){return mArrBullet;};
  void setArrBullet(char* pArr){memcpy(mArrBullet,pArr,mCntBullet*sizeof(TLocalBullet));};
};

#if defined( WIN32 )
#pragma pack(pop)
#endif


#endif