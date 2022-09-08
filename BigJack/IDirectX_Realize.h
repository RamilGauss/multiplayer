#ifndef IDirectX_RealizeH
#define IDirectX_RealizeH

#include "DXUT.h"
#include <set>

class TManagerDirectX;

class IDirectX_Realize
{
protected:

  TManagerDirectX * pMDX;

public:
  IDirectX_Realize(TManagerDirectX* _pMDX){pMDX = _pMDX;}
  virtual ~IDirectX_Realize(){};

  virtual void Init(HWND hwnd = NULL ) = 0;
  virtual void Work() = 0;
  virtual int Done()  = 0;

protected:
};




#endif