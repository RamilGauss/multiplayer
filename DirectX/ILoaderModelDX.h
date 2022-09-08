#ifndef ILoaderObjectDXH
#define ILoaderObjectDXH

#include "Struct3D.h"
#include <wtypes.h>
#include "EffectDX.h"

class ILoaderModelDX
{
public:
  struct TDefGroup
  {
    char*              strPathShader;// относительный путь
    DWORD*             indexes;
    DWORD              sizeIndexes;
    TEffectDX::VERTEX* vertex;
    DWORD              sizeVertex;
  };
protected:
  float mLOD;
  unsigned short mID_unique;
  int mCntGroup;
  TDefGroup* mArrDefGroup;

public:
  enum{
    eUndefID = -1,
  };

  ILoaderModelDX();
  virtual ~ILoaderModelDX();

  TDefGroup* GetArrDefGroup(){return mArrDefGroup;}
  // после загрузки становятся доступными
  float GetLOD();
  unsigned short GetID_Unique();// уникальный для данного набора, фигурирует в файле карты
  int GetCountSubset();
  DWORD* GetPinterIndexes(int iGroup, int &size);
  TEffectDX::VERTEX* GetPinterVertex(int iGroup, int &size);
  char* GetTexture(int iGroup, int &size);
  char* GetStrPathShader(int iGroup);


public:
  // virtual
  virtual bool Load(LPCWSTR strFilenameData) = 0;


protected:
  
  void Done();

};


#endif