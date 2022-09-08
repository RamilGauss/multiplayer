#ifndef LoaderObjectDXH
#define LoaderObjectDXH

#include "StructDirectX.h"

class TLoaderObjectDX
{

public:
  TLoaderObjectDX();
  ~TLoaderObjectDX();

  nsStructDirectX::TObjectDX* Load(unsigned int id);

protected:

};


#endif