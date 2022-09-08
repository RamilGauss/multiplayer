#include "LoaderModelDX.h"
#include "LoggerDX.h"
#include "BL_Debug.h"
#include "glib\gmem.h"

using namespace nsStruct3D;

TLoaderModelDX::TLoaderModelDX()
{

}
//--------------------------------------------------------------------------------
TLoaderModelDX::~TLoaderModelDX()
{

}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::Load(LPCWSTR strFilenameData)
{
  USES_CONVERSION;
  strcpy(pStrFilenameData ,W2A(strFilenameData));
  strcpy(pStrFilenameDataMainIni,pStrFilenameData);
  strcat(pStrFilenameDataMainIni,"\\main.ini");
  if(LoadMainFile()==false) 
  {
    GlobalLoggerDX.WriteF_time("Ќе удалось загрузить MainFile дл€ модели.\n");
    return false;
  }
  if(LoadFileResource()==false)
  {
    GlobalLoggerDX.WriteF_time("Ќе удалось загрузить ресурсы дл€ модели.\n");
    return false;
  }
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadMainFile()
{
  mFileIniMain.Close();
  mFileIniMain.Open(pStrFilenameDataMainIni);
  if(mFileIniMain.IsOpen()==false)
    return false;

  // считать данные:
  mCntEffectVisual = mFileIniMain.GetInteger("MAIN","CntEffectVisual",0);
  mCntEffectInLOD  = mFileIniMain.GetInteger("MAIN","CntEffectInLOD",0);
  mLOD             = mFileIniMain.GetDouble("MAIN","LOD",1);
  mID_unique       = mFileIniMain.GetInteger("MAIN","ID_unique",eUndefID);
  mCntGroup        = mFileIniMain.GetInteger("MAIN","CntGroup",0);
  mArrDefGroup     = new TDefGroup[mCntGroup];
  //----------------------------------------------------------------------------  
  // примитивы
  char* strFileNamePrimitive = mFileIniMain.GetValue("MAIN","StrPathFilePrimitive");
  if(strFileNamePrimitive)
  {
    strcpy(pStrPathPrimitive,pStrFilenameData);
    strcat(pStrPathPrimitive,"\\");
    strcat(pStrPathPrimitive,strFileNamePrimitive);
    g_free(strFileNamePrimitive);
  }
  //----------------------------------------------------------------------------  
  mFileIniMain.Close();
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadFileResource()
{
  mFileIniRes.Close();
  mFileIniRes.Open(pStrPathPrimitive);
  if(mFileIniRes.IsOpen()==false)
    return false;
  
  for(int i = 0 ; i < mCntGroup ; i++)
    if(LoadPart(i)==false) return false;

  mFileIniRes.Close();
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadPart(int i)
{
  char strNumPart[20];
  sprintf(strNumPart,"PART%d",i);
  
  mArrDefGroup[i].mIndex = mFileIniRes.GetInteger(strNumPart,"Index",0);
  //------------------------------------------------------------
  char * str = mFileIniRes.GetValue(strNumPart,"strPathShader");
  if(str)
  {
    strcpy(mArrDefGroup[i].strPathShader,pStrFilenameData);
    strcat(mArrDefGroup[i].strPathShader,"\\");
    strcat(mArrDefGroup[i].strPathShader,str);
    g_free(str);
  }
  else return false;
  //------------------------------------------------------------
  str = mFileIniRes.GetValue(strNumPart,"strTexture");
  if(str)
  {
    USES_CONVERSION;
    wcscpy(mArrDefGroup[i].strTexture,A2W(pStrFilenameData));
    wcscat(mArrDefGroup[i].strTexture,L"\\");
    wcscat(mArrDefGroup[i].strTexture,A2W(str));
    g_free(str);
  }
  else return false;
  //------------------------------------------------------------
  str = mFileIniRes.GetValue(strNumPart,"strName");
  if(str)
  {
    strcpy(mArrDefGroup[i].strName,str);
    g_free(str);
  }
  else return false;
  //------------------------------------------------------------
  if(LoadVector(strNumPart,"vAmbient",mArrDefGroup[i].vAmbient)==false) 
    return false;
  if(LoadVector(strNumPart,"vDiffuse",mArrDefGroup[i].vDiffuse)==false) 
    return false;
  if(LoadVector(strNumPart,"vSpecular",mArrDefGroup[i].vSpecular)==false) 
    return false;
  //vAmbient =1;1;1;
  //vDiffuse =1;1;1;
  //vSpecular=1;1;1;

  //nShininess=1
  //fAlpha=1.0
  //bSpecular=0
  //mTypeLOD=0
  //mflgNormal=1
  //cntIndexes=6
  //indexes=0;1;2;0;2;3;
  //cntVertex=4
  //vertex=0,0,0,0,1,0,0,0;0,0,0,0,1,0,0,0;0,0,0,0,1,0,0,0;0,0,0,0,1,0,0,0;
  return false/*true*/;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadVector(char* strNumPart,char* key,D3DXVECTOR3& vector)
{
  char* str = mFileIniRes.GetValue(strNumPart,key);
  if(str)
  {
    //vector.x = FindSemicolon();
    //vector.y = ;
    //vector.z = ;
    g_free(str);
    return true;
  }
  return false;
}
//--------------------------------------------------------------------------------