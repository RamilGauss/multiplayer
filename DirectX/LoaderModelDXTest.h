#ifndef LoaderModelDXTestH
#define LoaderModelDXTestH

#include "DefineUnicode.h"

#include "ILoaderModelDX.h"
#include "MeshLoader.h"

// для экспериментов (для WoT)

class TLoaderModelDXTest : public ILoaderModelDX
{

  CMeshLoader mMeshLoader;

public:
  TLoaderModelDXTest(IDirect3DDevice9* _m_pd3dDevice);
  virtual ~TLoaderModelDXTest();

  virtual bool Load(LPCWSTR strFilenameData);


protected:

  bool LoadMainFile();
  bool LoadFileResource();
  bool LoadPart(int i);
  bool LoadVector(char* strNumPart,char* key,D3DXVECTOR3& vector);

  bool LoadMesh(char* strNumPart, TDefGroup *mArrDefGroup);


protected:
  char* FindSemicolon(char* in_buffer);
  float FindFloat_Semicolon(char** buffer,bool* ok);
  int FindInt_Semicolon(char** buffer,bool* ok);
};


#endif