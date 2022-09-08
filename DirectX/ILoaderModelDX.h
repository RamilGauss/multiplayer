#ifndef ILoaderObjectDXH
#define ILoaderObjectDXH

#include "Struct3D.h"
#include <wtypes.h>
#include "EffectDX.h"
#include <d3dx9math.h>
#include "BL_ConfigFile.h"
#include <d3d9.h>

class ILoaderModelDX
{
protected:
  IDirect3DDevice9* m_pd3dDevice;
public:

  struct TDefGroup
  {
    unsigned char      mIndex;
    char   strPathShader[MAX_PATH];// ������������� ����
    char   strTechnique[MAX_PATH];
    WCHAR  strTexture[MAX_PATH];
    char   strName[MAX_PATH];

    ID3DXMesh* pMesh;
    //DWORD* indexes;
    //TEffectDX::VERTEX* vertex;
    //DWORD  cntIndexes;// ������ �� �����
    //DWORD  cntVertex;// ������ �� �����

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
      strTechnique[0]  = '\0';
      strName[0]       = '\0';
      pMesh            = NULL;
      //indexes = NULL;
      //vertex = NULL;
      //cntVertex = 0;
      //cntIndexes = 0;

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
  int mCntEffectVisual;// ������ ����� ��������
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

  ILoaderModelDX( IDirect3DDevice9* m_pd3dDevice );
  virtual ~ILoaderModelDX();

  TDefGroup* GetArrDefGroup(){return mArrDefGroup;}
  // ����� �������� ���������� ����������
  float GetLOD();
  unsigned short GetID_Unique();// ���������� ��� ������� ������, ���������� � ����� �����
  int GetCountSubset();
  //DWORD* GetPinterIndexes(int iGroup, int &size);
  //TEffectDX::VERTEX* GetPinterVertex(int iGroup, int &size);
  WCHAR* GetTexture(int iGroup, int &size);
  char* GetStrPathShader(int iGroup);

public:
  // virtual
  virtual bool Load(LPCWSTR strFilenameData) = 0;

protected:
  
  void Done();

  void UpPath(char* sPath);

protected:

  TBL_ConfigFile mFileIniMain;
  TBL_ConfigFile mFileIniRes;

  char pStrPathShader[MAX_PATH];

  char pStrFilenameData[MAX_PATH];
  char pStrPathPrimitive[MAX_PATH];
  char pStrFilenameDataMainIni[MAX_PATH];
};


#endif