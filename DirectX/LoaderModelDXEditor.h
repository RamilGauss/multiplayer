#ifndef LoaderModelDXEditorH
#define LoaderModelDXEditorH

#include "ILoaderModelDX.h"

// для экспериментов (для WoT)

class TLoaderModelDXEditor : public ILoaderModelDX
{

public:
  TLoaderModelDXEditor();
  virtual ~TLoaderModelDXEditor();

  virtual bool Load(LPCWSTR strFilenameData);


protected:

};


#endif
