/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "ILoaderModelGE.h"
#include "MakerXML.h"
#include "Logger.h"
#include <atlconv.h>

using namespace nsStruct3D;
using namespace std;

const char* sPathShader = "/shader";
const char* sPathTexture = "/texture";
const char* sPathMainFileConfig = "/main.xml";

const char* SectionLOD = "lod";
const char* SectionPart = "part";

const char* SectionPathShader = "strPathShader";
const char* SectionTechnique  = "strTechnique";
const char* SectionTexture    = "texture";
const char* SectionName       = "name";
const char* SectionNumUse     = "numUse";
const char* SectionAmbient    = "vAmbient";
const char* SectionDiffuse    = "vDiffuse";
const char* SectionSpecular   = "vSpecular";
const char* SectionShininess  = "nShininess";
const char* SectionbSpecular  = "bSpecular";
const char* SectionTypeLOD    = "mTypeLOD";
const char* SectionflgNormal  = "mflgNormal";
const char* SectionPrimitives = "primitives";

#pragma warning(disable:4800)

ILoaderModelGE::ILoaderModelGE()
{
  TMakerXML makerXML;
  mXML = makerXML.New();

  pStrPathShader[0]       = '\0';
  pStrFilenameData[0]     = '\0';
  pStrPathPrimitive[0]    = '\0';
  pStrFilenameDataMain[0] = '\0';
}
//-----------------------------------------------------------
ILoaderModelGE::~ILoaderModelGE()
{
  Done();
  delete mXML;
}
//-----------------------------------------------------------
int ILoaderModelGE::GetCountGroup()
{
  return mVectorGroup.size();
}
//-----------------------------------------------------------
void ILoaderModelGE::Done()
{
  int cnt = mVectorGroup.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    delete mVectorGroup[i];
  }
  mVectorGroup.clear();
}
//-----------------------------------------------------------
float ILoaderModelGE::GetLOD()
{
  return mLOD; 
}
//-----------------------------------------------------------
void ILoaderModelGE::UpPath(char* pPath)
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
bool ILoaderModelGE::Load(wchar_t* strFilenameData)
{
  USES_CONVERSION;

  strcpy(pStrPathShader,W2A(strFilenameData));
  UpPath(&pStrPathShader[0]);
  UpPath(&pStrPathShader[0]);
  UpPath(&pStrPathShader[0]);
  UpPath(&pStrPathShader[0]);
  strcat(pStrPathShader,sPathShader);

  strcpy(pStrFilenameData ,W2A(strFilenameData));
  UpPath(&pStrFilenameData[0]);
  strcpy(pStrFilenameDataMain,pStrFilenameData);
  strcat(pStrFilenameDataMain,sPathMainFileConfig);

  strcpy(pStrPathTexture,W2A(strFilenameData));
  UpPath(&pStrPathTexture[0]);
  UpPath(&pStrPathTexture[0]);
  UpPath(&pStrPathTexture[0]);
  UpPath(&pStrPathTexture[0]);
  strcat(pStrPathTexture,sPathTexture);

  if(LoadMainFile()==false) 
  {
    GetLogger()->Get("GE")->WriteF_time("Не удалось загрузить ресурсы для модели.\n");
    return false;
  }
  return true;
}
//--------------------------------------------------------------------------------
bool ILoaderModelGE::LoadMainFile()
{
  mXML->Load(pStrFilenameDataMain);
  if(mXML->IsOpen()==false)
    return false;

  // кол-во записей
  string sRoot = mXML->GetNameSection(0);
  if(sRoot.length()==0) return false;
  if(mXML->EnterSection(sRoot.data(),0)==false) return false;
  //--------------------------------------
  // считать данные:
  if(mXML->ReadFloat(SectionLOD,0,mLOD)==false) return false;
  int cntGroup = mXML->GetCountSection(SectionPart);// кол-во частей
  Done();
  for(int i = 0 ; i < cntGroup ; i++)
  {
    mVectorGroup.push_back(new TDefGroup);
    if(LoadPart(i)==false) 
      return false;
  }
  //--------------------------------------
  mXML->LeaveSection();
  return true;
}
//--------------------------------------------------------------------------------
bool ILoaderModelGE::LoadPart(int i)
{
  if(mXML->EnterSection(SectionPart,i)==false) return false;
  //-----------------------------------------------
  string str = mXML->ReadSection(SectionPathShader,0);
  if(str.length())
  {
    mVectorGroup[i]->strPathShader = pStrPathShader;
    mVectorGroup[i]->strPathShader += "\\";
    mVectorGroup[i]->strPathShader += str.data();
  }
  else return false;
  //------------------------------------------------------------
  str = mXML->ReadSection(SectionTechnique,0);
  if(str.length())
    mVectorGroup[i]->strTechnique = str.data();
  else return false;
  //------------------------------------------------------------
  str = mXML->ReadSection(SectionTexture,0);
  if(str.length())
  {
    USES_CONVERSION;
    mVectorGroup[i]->strTexture = A2W(pStrPathTexture);
    mVectorGroup[i]->strTexture += L"\\";
    mVectorGroup[i]->strTexture += A2W(str.data());
  }
  else return false;
  //------------------------------------------------------------
  str = mXML->ReadSection(SectionName,0);
  if(str.length())
  {
    mVectorGroup[i]->strName = str.data();
  }
  else return false;
  //------------------------------------------------------------
  int numUse;
  if(mXML->ReadInt(SectionNumUse,0,numUse)==false)
    return false;
  mVectorGroup[i]->mNumUse = numUse;
  //------------------------------------------------------------
  if(LoadVector(SectionAmbient,mVectorGroup[i]->vAmbient)==false) 
    return false;
  if(LoadVector(SectionDiffuse,mVectorGroup[i]->vDiffuse)==false) 
    return false;
  if(LoadVector(SectionSpecular,mVectorGroup[i]->vSpecular)==false) 
    return false;
  //------------------------------------------------------------
  int val;
  if(mXML->ReadInt(SectionShininess,0,val)==false) return false;
  mVectorGroup[i]->nShininess = val;
  if(mXML->ReadInt(SectionbSpecular,0,val)==false) return false;
  mVectorGroup[i]->bSpecular  = bool(val);
  if(mXML->ReadInt(SectionTypeLOD,0,val)==false) return false;
  mVectorGroup[i]->mTypeLOD   = bool(val);
  if(mXML->ReadInt(SectionflgNormal,0,val)==false) return false;
  mVectorGroup[i]->mflgNormal = bool(val);
  //-----------------------------------------------------------------------
  if(SetupMesh(mVectorGroup[i])==false)
    return false;

  mXML->LeaveSection();
  return true;
}
//--------------------------------------------------------------------------------
bool ILoaderModelGE::LoadVector4(const char* key,TVector4/*D3DXVECTOR4*/& vector4)
{
  float v4[4];
  if(mXML->ReadFloat4(key,0,&v4[0])==false) return false;

  vector4.x = v4[0];
  vector4.y = v4[1];
  vector4.z = v4[2];
  vector4.w = v4[3];
  return true;
}
//--------------------------------------------------------------------------------
bool ILoaderModelGE::LoadVector(const char* key,TVector3/*D3DXVECTOR3*/& vector)
{
  float v3[3];
  if(mXML->ReadFloat3(key,0,&v3[0])==false) return false;

  vector.x = v3[0];
  vector.y = v3[1];
  vector.z = v3[2];
  return true;
}
//--------------------------------------------------------------------------------
bool ILoaderModelGE::SetupMesh(TDefGroup *mArrDefGroup)
{
  char strPathPrimitives[MAX_PATH];
  string str = mXML->ReadSection(SectionPrimitives,0);
  if(str.length())
  {
    strcpy(strPathPrimitives,pStrFilenameData);
    strcat(strPathPrimitives,"\\");
    strcat(strPathPrimitives,str.data());
  }
  else 
    return false;

  mArrDefGroup->pMesh = LoadMesh(strPathPrimitives);
  return (mArrDefGroup->pMesh!=NULL);
}
//--------------------------------------------------------------------------------
