#ifndef LoaderModelDXEditorH
#define LoaderModelDXEditorH

#include "ILoaderModelDX.h"

// ��� ������������� (��� WoT)

class TLoaderModelDXEditor : public ILoaderModelDX
{

public:
  TLoaderModelDXEditor();
  virtual ~TLoaderModelDXEditor();

  virtual bool Load(LPCWSTR strFilenameData);


protected:

};


#endif
