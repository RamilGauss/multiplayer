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
