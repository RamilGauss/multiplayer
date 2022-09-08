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

};


#endif