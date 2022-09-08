#ifndef LoaderObjectDXH
#define LoaderObjectDXH

#include "ILoaderModelDX.h"

class TLoaderModelDX : public ILoaderModelDX
{

public:
  TLoaderModelDX();
  virtual ~TLoaderModelDX();

  virtual bool Load(LPCWSTR strFilenameData);


protected:

  bool LoadMainFile();
  bool LoadFileResource();
  bool LoadPart(int i);
  bool LoadVector(char* strNumPart,char* key,D3DXVECTOR3& vector);
};


#endif