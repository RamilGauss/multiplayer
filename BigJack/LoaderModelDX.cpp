/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "LoaderModelDX.h"
#include "Logger.h"
#include "BL_Debug.h"
#include "glib/gmem.h"

using namespace nsStruct3D;

TLoaderModelDX::TLoaderModelDX(  IDirect3DDevice9* _m_pd3dDevice):
ILoaderModelDX(_m_pd3dDevice)
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
  UpPath(&pStrPathShader[0]);
  UpPath(&pStrPathShader[0]);
  UpPath(&pStrPathShader[0]);
  strcat(pStrPathShader,"\\shader");

  strcpy(pStrFilenameData ,W2A(strFilenameData));
  UpPath(&pStrFilenameData[0]);
  strcpy(pStrFilenameDataMainIni,pStrFilenameData);
  strcat(pStrFilenameDataMainIni,"\\main.ini");
  if(LoadMainFile()==false) 
  {
    GlobalLoggerDX.WriteF_time("Не удалось загрузить ресурсы для модели.\n");
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
  mLOD         = (float)mFileIniMain.GetDouble("MAIN","LOD",1);
  int cntGroup = mFileIniMain.GetInteger("MAIN","CntGroup",0);
  Done();
  for(int i = 0 ; i < cntGroup ; i++)
  {
    mVectorGroup.push_back(new TDefGroup);
    if(LoadPart(&mFileIniMain,i)==false) 
      return false;
  }
  mFileIniMain.Close();
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadPart(TBL_ConfigFile* fileIni, int i)
{
  char strNumPart[20];
  sprintf(strNumPart,"PART%d",i);

  //------------------------------------------------------------
  char * str = fileIni->GetValue(strNumPart,"strPathShader");
  if(str)
  {
    mVectorGroup[i]->strPathShader = pStrPathShader;
    mVectorGroup[i]->strPathShader += "\\";
    mVectorGroup[i]->strPathShader += str;
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  str = fileIni->GetValue(strNumPart,"strTechnique");
  if(str)
  {
    mVectorGroup[i]->strTechnique = str;
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  str = fileIni->GetValue(strNumPart,"strTexture");
  if(str)
  {
    USES_CONVERSION;
    mVectorGroup[i]->strTexture = A2W(pStrFilenameData);
    mVectorGroup[i]->strTexture += L"\\";
    mVectorGroup[i]->strTexture += A2W(str);
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  str = fileIni->GetValue(strNumPart,"strName");
  if(str)
  {
    mVectorGroup[i]->strName = str;
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------
  if(LoadVector(fileIni,strNumPart,"vAmbient",mVectorGroup[i]->vAmbient)==false) 
    return false;
  if(LoadVector(fileIni,strNumPart,"vDiffuse",mVectorGroup[i]->vDiffuse)==false) 
    return false;
  if(LoadVector(fileIni,strNumPart,"vSpecular",mVectorGroup[i]->vSpecular)==false) 
    return false;

  mVectorGroup[i]->nShininess = fileIni->GetInteger(strNumPart,"nShininess",0);
  mVectorGroup[i]->fAlpha     = (float)fileIni->GetDouble(strNumPart,"fAlpha");
  mVectorGroup[i]->bSpecular  = fileIni->GetBool(strNumPart,"bSpecular",false);
  mVectorGroup[i]->mTypeLOD   = fileIni->GetBool(strNumPart,"mTypeLOD",false);
  mVectorGroup[i]->mflgNormal = fileIni->GetBool(strNumPart,"mflgNormal",true);
  //-----------------------------------------------------------------------
  if(LoadMesh(fileIni,strNumPart,mVectorGroup[i])==false)
    return false;

  // загрузить Joint
  mVectorGroup[i]->mCntJoint = fileIni->GetInteger(strNumPart,"cntJoint",0);
  if(mVectorGroup[i]->mCntJoint>0)
    mVectorGroup[i]->pArrJoint = new TJointPart[mVectorGroup[i]->mCntJoint];
  for(int j = 0 ; j < mVectorGroup[i]->mCntJoint ; j++)
  {
    char partJoint[30];
    sprintf(partJoint,"nameJoint%d",j);
    TJointPart* pJoint = &(mVectorGroup[i]->pArrJoint[j]);
    str = fileIni->GetValue(strNumPart,partJoint);
    if(str)
    {
      pJoint->namePart = str;
      g_free(str);
      str = NULL;
    }
    else return false;

    for(int k = 0 ; k < 4 ; k++)
    {
      D3DXVECTOR4 vector4;
      char strNumJointPart[20];
      sprintf(strNumJointPart,"matrix%d_%d",j,k);

      if(LoadVector4(fileIni,strNumPart,strNumJointPart,vector4)==false) 
        return false;
      for(int m = 0 ; m < 4 ; m++)
        pJoint->matrix.m[k][m]=vector4[m];
    }
  }
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadVector4(TBL_ConfigFile* fileIni,char* strNumPart,char* key,D3DXVECTOR4& vector4)
{
  char* str = fileIni->GetValue(strNumPart,key);
  if(str)
  {
    char* buffer = str;
    bool ok;
    vector4.x = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    vector4.y = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    vector4.z = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    vector4.w = FindFloat_Semicolon(&buffer,&ok);
    if(ok==false) {g_free(str);return false;}
    g_free(str);
    return true;
  }
  return false;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadVector(TBL_ConfigFile* fileIni,char* strNumPart,char* key,D3DXVECTOR3& vector)
{
  char* str = fileIni->GetValue(strNumPart,key);
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
  float res = (float)atof(cpyBuffer);
  *buffer = buffer1+1;
  *ok = true;
  return res; 
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadMesh(TBL_ConfigFile* fileIni,char* strNumPart,TDefGroup *mArrDefGroup)
{
  HRESULT hr;
  char strPathPrimitives[MAX_PATH];
  char* str = fileIni->GetValue(strNumPart,"primitives");
  if(str)
  {
    strcpy(strPathPrimitives,pStrFilenameData);
    strcat(strPathPrimitives,"\\");
    strcat(strPathPrimitives,str);
    g_free(str);
  }
  else return false;

  USES_CONVERSION;
  V(mMeshLoader.Create(m_pd3dDevice,A2W(strPathPrimitives)));
  mArrDefGroup->pMesh = mMeshLoader.GetMesh();
  mMeshLoader.ZeroMesh();// отвязаться

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
