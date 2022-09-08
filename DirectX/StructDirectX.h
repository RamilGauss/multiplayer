#ifndef StructDirectXH
#define StructDirectXH

#include "TypeDef.h"

namespace nsStructDirectX
{

#if defined( TD_WINDOWS )
#pragma pack(push, 1)


struct TStateObjectDX
{
  unsigned char cnt;
  unsigned char* pState;
  TStateObjectDX()
  {
    cnt = 0;
    pState = NULL;
  }
  ~TStateObjectDX()
  {
    clear();
  }
  void clear()
  {
    cnt=0;
    delete[] pState;
    pState = NULL;
  }
  void setCnt(unsigned char val)
  {
    clear();
    cnt = val;
    pState = new unsigned char(cnt);
  }
};
//-----------------------------------------------------------------
struct TCoord3
{
  float x;
  float y;
  float z;
};
//-----------------------------------------------------------------
struct TOrient3
{
  float vx;
  float vy;
  float vz;
};
//-----------------------------------------------------------------
class TObjectDX
{
public:
  TObjectDX();
  ~TObjectDX();

};
//-----------------------------------------------------------------

#pragma pack(pop)
#endif

}

#endif