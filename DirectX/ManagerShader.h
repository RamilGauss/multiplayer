#ifndef ManagerShaderH
#define ManagerShaderH


#include <d3dx9effect.h>

class TManagerShader
{

  ID3DXEffect* pEffect;

public:
  TManagerShader();
  ~TManagerShader();


  void Create();
  void Reset();
  void LostDevice();
  void DestroyDevice();

};

#endif
