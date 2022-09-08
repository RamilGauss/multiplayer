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
    GlobalLoggerDX.WriteF_time("Не удалось загрузить MainFile для модели.\n");
    return false;
  }
  //LoadFileResource();
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadMainFile()
{
  mFileIniMain.Open(pStrFilenameDataMainIni);
  if(mFileIniMain.IsOpen()==false)
  {
    BL_FIX_BUG();
    return false;
  }
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
