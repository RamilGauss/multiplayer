/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 
#include "LoaderTree.h"
#include "MakerXML.h"

namespace TreeSection
{
  const char* SectionMatrix = "matrix";
  const char* SectionRow    = "row";
  const char* SectionJoint  = "joint";
  const char* SectionName   = "name";
  const char* SectionNumUse = "numUse";
  const char* SectionChild  = "child";
}

using namespace std;
using namespace TreeSection;
using namespace nsStruct3D;

TLoaderTree::TLoaderTree()
{
  pLoadedTree = NULL;
  TMakerXML makerXML;
  mXML = makerXML.New();
}
//-----------------------------------------------------------------------------
TLoaderTree::~TLoaderTree()
{
  delete pLoadedTree;
	TMakerXML makerXML;
	makerXML.Delete(mXML);
}
//-----------------------------------------------------------------------------
bool TLoaderTree::Load(char* sPath)
{
  delete pLoadedTree;
  pLoadedTree = new TTreeJoint::TLoadedJoint;
  CHECK_RET(mXML->Load(sPath))
  string sRoot = mXML->GetNameSection(0);
  CHECK_RET(mXML->EnterSection(sRoot.data(),0))
  //--------------------------------------------
  LoadMatrix4x4(SectionMatrix,0,&(pLoadedTree->world));

  // ��������� Joint
  // ��� �����
  string str = mXML->ReadSection(SectionName,0);
  if(str.length())
    pLoadedTree->root = str.data();
  else return false;
  //------------------------------------------------------------------
  // ��������� �����
  int cntJoint = mXML->GetCountSection(SectionJoint);
  for(int i = 0 ; i < cntJoint ; i++)
    CHECK_RET(LoadJoint(i)) 

  mXML->ResetPos();
  return true;
}
//---------------------------------------------------------------------------------------
TTreeJoint::TLoadedJoint* TLoaderTree::TakeTree()
{
  TTreeJoint::TLoadedJoint* p = pLoadedTree;
  pLoadedTree = NULL;
  return p;
}
//--------------------------------------------------------------------------------
bool TLoaderTree::LoadMatrix4x4(const char* name,int num, TMatrix16/*D3DXMATRIXA16*/* pM)
{
  CHECK_RET(mXML->EnterSection(name,num))
  for(int k = 0 ; k < 4 ; k++)
  {
    float v4[4];
    CHECK_RET(mXML->ReadFloat4(SectionRow,k,&v4[0]))
    for(int m = 0 ; m < 4 ; m++)
      pM->m[k][m] = v4[m];
  }
  mXML->LeaveSection();
  return true;
}
//--------------------------------------------------------------------------------
bool TLoaderTree::LoadJoint(int i)
{
  CHECK_RET(mXML->EnterSection(SectionJoint,i))

  TTreeJoint::TPart* pPart = new TTreeJoint::TPart;
  // name
  string str = mXML->ReadSection(SectionName,0);
  if(str.length())
    pPart->name = str;
  else return false;
  // numUse
  int numUse;
  CHECK_RET(mXML->ReadInt(SectionNumUse,0,numUse))
  pPart->numUse = numUse;
  // ��������� �����
  int cntChild = mXML->GetCountSection(SectionChild);
  // 05.03.2013 if(cntChild==0) return false;
  for(int j = 0 ; j < cntChild; j++)
  {
    CHECK_RET(mXML->EnterSection(SectionChild,j))
    //---------------------------------------------------------
    TTreeJoint::TChild* pChild = new TTreeJoint::TChild;
    str = mXML->ReadSection(SectionName,0);
    if(str.length())
      pChild->name = str.data();
    else return false;
    //----------------------------------------------    
    CHECK_RET(LoadMatrix4x4(SectionMatrix,0,&(pChild->matrix)))
    pPart->vectorChild.push_back(pChild);
    //--------------------------------------------------------
    mXML->LeaveSection();
  }

  pLoadedTree->vectorPart.push_back(pPart);
  
  mXML->LeaveSection();
  return true;
}
//--------------------------------------------------------------------------------
