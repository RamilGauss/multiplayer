#ifndef LoaderObjectDXH
#define LoaderObjectDXH

#include "StructDirectX.h"
#include <wtypes.h>

class TLoaderModelDX
{

public:
  TLoaderModelDX();
  ~TLoaderModelDX();

  bool Load(LPCWSTR strFilenameData);

protected:

};


#endif