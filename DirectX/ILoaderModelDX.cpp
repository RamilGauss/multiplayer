#include "ILoaderModelDX.h"




ILoaderModelDX::ILoaderModelDX( IDirect3DDevice9* _m_pd3dDevice )
{
  m_pd3dDevice = _m_pd3dDevice;

  pStrPathShader[0]          = '\0';
  pStrFilenameData[0]        = '\0';
  pStrPathPrimitive[0]       = '\0';
  pStrFilenameDataMainIni[0] = '\0';

  mID_unique   = eUndefID;
  mCntGroup    = 0;
  mArrDefGroup = NULL;
}
//-----------------------------------------------------------
ILoaderModelDX::~ILoaderModelDX()
{
  Done();
}
//-----------------------------------------------------------
int ILoaderModelDX::GetCountSubset()
{
  return mCntGroup;
}
//-----------------------------------------------------------
char* ILoaderModelDX::GetStrPathShader(int iGroup)
{
  return mArrDefGroup[iGroup].strPathShader;
}
//-----------------------------------------------------------
//DWORD* ILoaderModelDX::GetPinterIndexes(int iGroup, int &size)
//{
//  size = mArrDefGroup[iGroup].cntIndexes;
//  return mArrDefGroup[iGroup].indexes;
//}
////-----------------------------------------------------------
//TEffectDX::VERTEX* ILoaderModelDX::GetPinterVertex(int iGroup, int &size)
//{
//  size = mArrDefGroup[iGroup].cntVertex;
//  return mArrDefGroup[iGroup].vertex;
//}
////-----------------------------------------------------------
WCHAR* ILoaderModelDX::GetTexture(int iGroup, int &cnt)
{
  return mArrDefGroup[iGroup].strTexture;
}
//-----------------------------------------------------------
void ILoaderModelDX::Done()
{
  mCntGroup = 0;
  delete mArrDefGroup;
  mArrDefGroup = NULL;
}
//-----------------------------------------------------------
unsigned short ILoaderModelDX::GetID_Unique()
{
  return mID_unique;
}
//-----------------------------------------------------------
float ILoaderModelDX::GetLOD()
{
  return mLOD; 
}
//-----------------------------------------------------------
void ILoaderModelDX::UpPath(char* pPath)
{
  int len = strlen(pPath);
  for(int i = len -1 ; i >= 0 ; i--)
  {
    if(pPath[i]=='\\')
    {
      pPath[i] = '\0';
      return;
    }
  }
}
//-----------------------------------------------------------
