#ifndef ManagerModelH
#define ManagerModelH


class TManagerModel
{

public:
  TManagerModel();
  ~TManagerModel();

  // ������ ���������� (���������, ������� ������������ � ObjectDX)

  HRESULT Load(IDirect3DDevice9* pd3dDevice, 
    const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
    void* pUserContext );



};

extern TManagerModel GlobalManagerModel;

#endif