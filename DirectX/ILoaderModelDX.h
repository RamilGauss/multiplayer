#ifndef ILoaderObjectDXH
#define ILoaderObjectDXH

#include "Struct3D.h"
#include <wtypes.h>
#include "EffectDX.h"
#include <d3dx9math.h>
#include "BL_ConfigFile.h"

class ILoaderModelDX
{
public:

  struct TDefGroup
  {
    unsigned char      mIndex;
    char   strPathShader[MAX_PATH];// относительный путь
    WCHAR  strTexture[MAX_PATH];
    char   strName[MAX_PATH];
    DWORD* indexes;
    TEffectDX::VERTEX* vertex;
    DWORD  sizeIndexes;// расчет по файлу
    DWORD  sizeVertex;// расчет по файлу

    D3DXVECTOR3 vAmbient;
    D3DXVECTOR3 vDiffuse;
    D3DXVECTOR3 vSpecular;

    int   nShininess;
    float fAlpha;

    bool bSpecular;

    unsigned char mTypeLOD;
    bool          mflgNormal;

    TDefGroup()
    {
      mIndex = 0;
      strTexture[0]    = '\0';
      strPathShader[0] = '\0';
      strName[0]       = '\0';
      indexes = NULL;
      vertex = NULL;
      sizeVertex = 0;
      sizeIndexes = 0;

      vAmbient  = D3DXVECTOR3(0,0,0);
      vDiffuse  = D3DXVECTOR3(0,0,0);
      vSpecular = D3DXVECTOR3(0,0,0);

      nShininess = 0;
      fAlpha     = 0.0f;

      bSpecular  = false;

      mTypeLOD   = 0;
      mflgNormal = true;
    }
  };
public:
  int mCntEffectVisual;// расчет после загрузки
  int mCntEffectInLOD;
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

protected:

  TBL_ConfigFile mFileIniMain;
  TBL_ConfigFile mFileIniRes;

  char pStrFilenameData[MAX_PATH];
  char pStrPathPrimitive[MAX_PATH];
  char pStrFilenameDataMainIni[MAX_PATH];
};


#endif