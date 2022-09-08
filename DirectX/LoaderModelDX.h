#ifndef LoaderObjectDXH
#define LoaderObjectDXH

#include "Struct3D.h"
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