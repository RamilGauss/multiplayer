#ifndef StructDirectXH
#define StructDirectXH

#include "TypeDef.h"

namespace nsStructDirectX
{

#if defined( TD_WINDOWS )
#pragma pack(push, 1)

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

#pragma pack(pop)
#endif

}

#endif