/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
#include "LoggerDX.h"
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
  mLOD             = (float)mFileIniMain.GetDouble("MAIN","LOD",1);
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
  mArrDefGroup[i].fAlpha     = (float)mFileIniRes.GetDouble(strNumPart,"fAlpha");
  mArrDefGroup[i].bSpecular  = mFileIniRes.GetBool(strNumPart,"bSpecular",false);
  mArrDefGroup[i].mTypeLOD   = mFileIniRes.GetBool(strNumPart,"mTypeLOD",false);
  mArrDefGroup[i].mflgNormal = mFileIniRes.GetBool(strNumPart,"mflgNormal",true);
  //-----------------------------------------------------------------------
  if(LoadMesh(strNumPart,&mArrDefGroup[i])==false)
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
  float res = (float)atof(cpyBuffer);
  *buffer = buffer1+1;
  *ok = true;
  return res; 
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadMesh(char* strNumPart,TDefGroup *mArrDefGroup)
{
  HRESULT hr;
  char strPathPrimitives[MAX_PATH];
  char* str = mFileIniRes.GetValue(strNumPart,"primitives");
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
  mMeshLoader.ZeroMesh();// отв€затьс€

  return true;
//  HRESULT hr;
//  LPD3DXMESH pMesh = NULL;
//  LPD3DXBUFFER bufEffectInstances = NULL;
//  LPD3DXBUFFER bufAdjacency = NULL;
//  DWORD dwMaterials = 0;
//
//  V( D3DXLoadMeshFromX( strPathPrimitives,
//                        D3DXMESH_SYSTEMMEM,
//                        m_pd3dDevice,
//                        NULL,//&bufAdjacency,
//                        NULL,
//                        NULL,//&bufEffectInstances,
//                        NULL,//&dwMaterials,
//                        &pMesh
//                        ) );
//  mArrDefGroup->cntVertex  = 3;//###pMesh->GetNumVertices();
//  mArrDefGroup->vertex  = new TEffectDX::VERTEX[mArrDefGroup->cntVertex];
//  mArrDefGroup->cntIndexes = 3;//###3*pMesh->GetNumFaces();
//  mArrDefGroup->indexes = new DWORD[mArrDefGroup->cntIndexes];
//#if 0
//  //###
//  mArrDefGroup->indexes[0]=0;
//  mArrDefGroup->indexes[1]=1;
//  mArrDefGroup->indexes[2]=2;
//  //###
//#endif
//#if 1
//  DWORD size = pMesh->GetNumBytesPerVertex();
//  // копировать из меша
//#pragma pack(push, 1)
//  struct TPosText
//  {
//    D3DXVECTOR3 position;
//    D3DXVECTOR2 texcoord;
//  };
//#pragma pack(pop)
//  TPosText* pVertex;
//  V( pMesh->LockVertexBuffer( 0 , ( void** )&pVertex ) );
//  for(int i = 0 ; i < mArrDefGroup->cntVertex ; i++ )
//  {
//    mArrDefGroup->vertex[i].position = pVertex[i].position;
//    mArrDefGroup->vertex[i].normal   = D3DXVECTOR3(0,0,1);
//    mArrDefGroup->vertex[i].texcoord = pVertex[i].texcoord;
//  }
//  //memcpy( mArrDefGroup->vertex, pVertex, /*mArrDefGroup->cntVertex **/ sizeof( TEffectDX::VERTEX ) );
//  pMesh->UnlockVertexBuffer();
//
//  // Copy the index data
//  DWORD* pIndex;
//  V( pMesh->LockIndexBuffer( 0 , ( void** )&pIndex ) );
//  memcpy( mArrDefGroup->indexes, pIndex, mArrDefGroup->cntIndexes * sizeof( DWORD ) );
//  pMesh->UnlockIndexBuffer();
//
//#endif
//  // уничтожить меш и другие ресурсы
//  SAFE_RELEASE(pMesh);
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
