#include "ILoaderModelDX.h"




ILoaderModelDX::ILoaderModelDX()
{
  pStrFilenameData[0] = '\0';
  pStrPathPrimitive[0] = '\0';
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
DWORD* ILoaderModelDX::GetPinterIndexes(int iGroup, int &size)
{
  size = mArrDefGroup[iGroup].cntIndexes;
  return mArrDefGroup[iGroup].indexes;
}
//-----------------------------------------------------------
TEffectDX::VERTEX* ILoaderModelDX::GetPinterVertex(int iGroup, int &size)
{
  size = mArrDefGroup[iGroup].cntVertex;
  return mArrDefGroup[iGroup].vertex;
}
//-----------------------------------------------------------
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
