#ifndef LoaderModelDXTestH
#define LoaderModelDXTestH

#include "ILoaderModelDX.h"

// для экспериментов (для WoT)

class TLoaderModelDXTest : public ILoaderModelDX
{

public:
  TLoaderModelDXTest();
  virtual ~TLoaderModelDXTest();

  virtual bool Load(LPCWSTR strFilenameData);


protected:

};


#endif
