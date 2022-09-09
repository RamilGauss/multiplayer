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
#include "LoaderTree.h"
#include "glib/gmem.h"

TLoaderTree::TLoaderTree()
{
  pLoadedTree = NULL;
}
//-----------------------------------------------------------------------------
TLoaderTree::~TLoaderTree()
{
  delete pLoadedTree;
}
//-----------------------------------------------------------------------------
bool TLoaderTree::Load(char* sPath)
{
  delete pLoadedTree;
  pLoadedTree = new TTreeJoint::TLoadedJoint;
  TBL_ConfigFile mFileIniMain;
  mFileIniMain.Close();
  mFileIniMain.Open(sPath);
  if(mFileIniMain.IsOpen()==false)
    return false;

  TBL_ConfigFile* fileIni = &mFileIniMain;
  for(int k = 0 ; k < 4 ; k++)
  {
    char sWorld[100];
    sprintf(sWorld,"world%d",k);// матрица из 4 строк
    D3DXVECTOR4 vector4;
    if(LoadVector4(fileIni,"JOINT",sWorld,vector4)==false) return false;
    for(int m = 0 ; m < 4 ; m++)
      pLoadedTree->world.m[k][m] = vector4[m];
  }

  // загрузить Joint
  // им€ корн€
  int cntJoint = fileIni->GetInteger("JOINT","cnt",0);
  char* str = fileIni->GetValue("JOINT","root");
  if(str)
  {
    pLoadedTree->root = str;
    g_free(str);
    str = NULL;
  }
  else return false;
  //------------------------------------------------------------------
  // загрузить части
  for(int i = 0 ; i < cntJoint ; i++)
  {
    TTreeJoint::TPart* pPart = new TTreeJoint::TPart;
    char sj[100];
    sprintf(sj,"JOINT%d",i);
    char* str = fileIni->GetValue(sj,"strName");
    if(str)
    {
      pPart->name = str;
      g_free(str);
      str = NULL;
    }
    else return false;
    // загрузить детей
    int cntChild = fileIni->GetInteger(sj,"cntJoint",0);
    if(cntChild==0) return false;
    for(int j = 0 ; j < cntChild; j++)
    {
      TTreeJoint::TChild* pChild = new TTreeJoint::TChild;
      char sChild[100];
      sprintf(sChild,"nameJoint%d",j);// им€ ребенка
      str = fileIni->GetValue(sj,sChild);
      if(str)
      {
        pChild->name = str;
        g_free(str);
        str = NULL;
      }
      else return false;
      //----------------------------------------------    
      for(int k = 0 ; k < 4 ; k++)
      {
        char sMatrix[100];
        sprintf(sMatrix,"matrix%d_%d",j,k);// матрица из 4 строк
        D3DXVECTOR4 vector4;
        if(LoadVector4(fileIni,sj,sMatrix,vector4)==false) return false;
        for(int m = 0 ; m < 4 ; m++)
          pChild->matrix.m[k][m] = vector4[m];
      }
      pPart->vectorChild.push_back(pChild);
    }

    pLoadedTree->vectorPart.push_back(pPart);
  }
  return true;
}
//---------------------------------------------------------------------------------------
bool TLoaderTree::LoadVector4(TBL_ConfigFile* fileIni,char* strNumPart,char* key,D3DXVECTOR4& vector4)
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
float TLoaderTree::FindFloat_Semicolon(char** buffer,bool* ok)
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
char* TLoaderTree::FindSemicolon(char* buffer)
{
  for(int i = 0 ; true ; i++ )
  {
    if(buffer[i]=='\0') return NULL;
    if(buffer[i]==';') return &buffer[i];
  }
  return NULL;
}
//--------------------------------------------------------------------------------
TTreeJoint::TLoadedJoint* TLoaderTree::TakeTree()
{
  TTreeJoint::TLoadedJoint* p = pLoadedTree;
  pLoadedTree = NULL;
  return p;
}
//--------------------------------------------------------------------------------