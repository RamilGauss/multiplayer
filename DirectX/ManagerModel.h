#ifndef ManagerModelH
#define ManagerModelH

#include "DXUT.h"
#include "hArray.h"
class TModelDX;

class TManagerModel
{

public:
  TManagerModel();
  ~TManagerModel();
  
  TModelDX* Find(unsigned int id);
  // список примитивов (оригиналы, которые используются в ObjectDX)

  void Load(IDirect3DDevice9* pd3dDevice, 
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext );

  void ResetDevice();
  void OnLostDevice();
  void OnDestroyDevice();

  TArrayObject mArrModel;

protected:
  bool LoadListPath();

  char** mArrPathModel;
  int   mCntPathModel;

  void Done();

};

extern TManagerModel GlobalManagerModel;

#endif