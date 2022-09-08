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

  bool LoadVertex(char* strNumPart, TEffectDX::VERTEX* vertex, int cnt);
  bool LoadIndexes(char* strNumPart, DWORD *indexes, int cnt);


protected:
  char* FindSemicolon(char* in_buffer);
  float FindFloat_Semicolon(char** buffer,bool* ok);
  int FindInt_Semicolon(char** buffer,bool* ok);
};


#endif