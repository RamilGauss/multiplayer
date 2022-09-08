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

  strcpy(pStrPathShader,W2A(strFilenameData));
  UpPath(&pStrPathShader[0]);
  strcat(pStrPathShader,"\\shader");

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
    strcpy(mArrDefGroup[i].strPathShader,pStrPathShader);
    strcat(mArrDefGroup[i].strPathShader,"\\");
    strcat(mArrDefGroup[i].strPathShader,str);
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  str = mFileIniRes.GetValue(strNumPart,"strTechnique");
  if(str)
  {
    strcpy(mArrDefGroup[i].strTechnique,str);
    g_free(str);
    str = NULL;
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
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  str = mFileIniRes.GetValue(strNumPart,"strName");
  if(str)
  {
    strcpy(mArrDefGroup[i].strName,str);
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  if(LoadVector(strNumPart,"vAmbient",mArrDefGroup[i].vAmbient)==false) 
    return false;
  if(LoadVector(strNumPart,"vDiffuse",mArrDefGroup[i].vDiffuse)==false) 
    return false;
  if(LoadVector(strNumPart,"vSpecular",mArrDefGroup[i].vSpecular)==false) 
    return false;

  mArrDefGroup[i].nShininess = mFileIniRes.GetInteger(strNumPart,"nShininess",0);
  mArrDefGroup[i].fAlpha     = mFileIniRes.GetDouble(strNumPart,"fAlpha");
  mArrDefGroup[i].bSpecular  = mFileIniRes.GetInteger(strNumPart,"bSpecular",0);
  mArrDefGroup[i].mTypeLOD   = mFileIniRes.GetInteger(strNumPart,"mTypeLOD",0);
  mArrDefGroup[i].mflgNormal = mFileIniRes.GetInteger(strNumPart,"mflgNormal",1);
  //-----------------------------------------------------------------------
  mArrDefGroup[i].cntIndexes = mFileIniRes.GetInteger(strNumPart,"cntIndexes",0);
  mArrDefGroup[i].cntVertex  = mFileIniRes.GetInteger(strNumPart,"cntVertex",0);
  if(mArrDefGroup[i].cntIndexes==0||mArrDefGroup[i].cntVertex==0) return false;
  mArrDefGroup[i].indexes = new DWORD[mArrDefGroup[i].cntIndexes];
  mArrDefGroup[i].vertex  = new TEffectDX::VERTEX[mArrDefGroup[i].cntVertex];
  if(LoadVertex(strNumPart,mArrDefGroup[i].vertex,mArrDefGroup[i].cntVertex)==false)
    return false;
  if(LoadIndexes(strNumPart,mArrDefGroup[i].indexes,mArrDefGroup[i].cntIndexes)==false)
    return false;
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadVector(char* strNumPart,char* key,D3DXVECTOR3& vector)
{
  char* str = mFileIniRes.GetValue(strNumPart,key);
  if(str)
  {
    char* buffer = str;
    bool ok;
    vector.x = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    vector.y = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    vector.z = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    g_free(str);
    return true;
  }
  return false;
}
//--------------------------------------------------------------------------------
char* TLoaderModelDX::FindSemicolon(char* buffer)
{
  for(int i = 0 ; true ; i++ )
  {
    if(buffer[i]=='\0') return NULL;
    if(buffer[i]==';') return &buffer[i];
  }
  return NULL;
}
//--------------------------------------------------------------------------------
float TLoaderModelDX::FindFloat_Semicolon(char** buffer,bool* ok)
{
  char cpyBuffer[50];
  char* buffer1;
  char* buffer0 = *buffer;
  int size;
  //---------------------------------------------------
  buffer1 = FindSemicolon(buffer0);
  if(buffer1==NULL)
  {
    *buffer = NULL;
    *ok = false;
    return 0.0f;
  }
  size = buffer1-buffer0;
  memcpy(cpyBuffer,buffer0,size);
  cpyBuffer[size] = '\0';
  float res = atof(cpyBuffer);
  *buffer = buffer1+1;
  *ok = true;
  return res; 
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadVertex(char* strNumPart, TEffectDX::VERTEX* vertex, int cnt)
{
  char *str = mFileIniRes.GetValue(strNumPart,"vertex");

  char* buffer = str;
  for(int i = 0 ; i < cnt ; i++ )
  {
    bool ok;
    vertex[i].position.x = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    vertex[i].position.y = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    vertex[i].position.z = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    //-----------------------------------------------------------------
    vertex[i].normal.x = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    vertex[i].normal.y = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    vertex[i].normal.z = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    //-----------------------------------------------------------------
    vertex[i].texcoord.x = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
    vertex[i].texcoord.y = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
  }
  g_free(str);
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadIndexes(char* strNumPart, DWORD* indexes, int cnt)
{
  char *str = mFileIniRes.GetValue(strNumPart,"indexes");
  char* buffer = str;
  for(int i = 0 ; i < cnt ; i++ )
  {
    bool ok;
    indexes[i] = FindInt_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str); return false;}
  }
  g_free(str);
  return true;
}
//--------------------------------------------------------------------------------
int TLoaderModelDX::FindInt_Semicolon(char** buffer,bool* ok)
{
  char cpyBuffer[50];
  char* buffer1;
  char* buffer0 = *buffer;
  int size;
  //---------------------------------------------------
  buffer1 = FindSemicolon(buffer0);
  if(buffer1==NULL)
  {
    *buffer = NULL;
    *ok = false;
    return 0;
  }
  size = buffer1-buffer0;
  memcpy(cpyBuffer,buffer0,size);
  cpyBuffer[size] = '\0';
  int res = atoi(cpyBuffer);
  *buffer = buffer1+1;
  *ok = true;
  return res; 
}
//--------------------------------------------------------------------------------
