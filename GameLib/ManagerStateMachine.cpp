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
the "TornadoEngine" Source Code.  If not, please request a copy in writing at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "ManagerStateMachine.h"
#include "MakerXML.h"
#include "IXML.h"
#include <string>

using namespace std;

const char* sState   = "state";
const char* sName    = "name";
const char* sDesc    = "desc";
const char* sKey     = "key";
const char* sValue   = "value";
const char* sComment = "comment";

TManagerStateMachine::TManagerStateMachine()
{
  TMakerXML maker;
  pXML = maker.New();
}
//---------------------------------------------------------------------------------
TManagerStateMachine::~TManagerStateMachine()
{
  Done();
}
//---------------------------------------------------------------------------------
/*
  Формат xml
  <state>
    <name>A</name>
    <desc>
      <key>0</key>
      <value>1</value>
      <comment>adasdad</comment>
    </desc>
    <desc>
      <key>0</key>
      <value>1</value>
      <comment>asdasdad</comment>
    </desc>
  </state>
*/
bool TManagerStateMachine::Load(const char* sPath,unsigned int& id)
{
  TSM_UintUint* pSM = new TSM_UintUint;
  mVecSM.push_back(pSM);

  CHECK_RET(pXML->Load(sPath))
  string sRoot = pXML->GetNameSection(0);
  CHECK_RET(pXML->EnterSection(sRoot.data(),0))
    
    int cntState = pXML->GetCountSection(sState);
    for(int i = 0 ; i < cntState ; i++)
    {
      string nameState;
      string sectionComment;
      unsigned int k, v;
      TMapUintPtr* m = new TMapUintPtr;
      CHECK_RET(pXML->EnterSection(sState,i))
        nameState = pXML->ReadSection(sName,0);// имя состояния
        int cntDesc = pXML->GetCountSection(sDesc);
        for(int j = 0 ; j < cntDesc ; j++)
        {
          CHECK_RET(pXML->EnterSection(sDesc,j))
            CHECK_RET(pXML->ReadUint(sKey,  0,k))
            CHECK_RET(pXML->ReadUint(sValue,0,v))
            sectionComment = pXML->ReadSection(sComment,0);
          CHECK_RET(pXML->LeaveSection())
          m->insert(TMapUintPtr::value_type(k,TDesc(v,sectionComment)));
        }
      CHECK_RET(pXML->LeaveSection())
      pSM->AddState(nameState,NULL,m);
      if(i==0)
        pSM->Init(nameState.data());
    }
  CHECK_RET(pXML->LeaveSection())
  //--------------------------------------------------
  id = mVecSM.size()-1;
  return true;
}
//---------------------------------------------------------------------------------
bool TManagerStateMachine::GetValue(unsigned int id, unsigned int k, unsigned int& v)
{
  if(mVecSM.size()<=id) return false;

  TMapUintPtr::iterator fit = mVecSM.at(id)->GetDataCurrent()->find(k);
  if(fit!=mVecSM.at(id)->GetDataCurrent()->end())
  {
    v = fit->second.v;
    return true; 
  }
  return false;
}
//---------------------------------------------------------------------------------
void TManagerStateMachine::Done()
{
  int cnt = mVecSM.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    delete mVecSM.at(i);
  }
  mVecSM.clear();

  TMakerXML maker;
  maker.Delete(pXML);
  pXML = NULL;
}
//---------------------------------------------------------------------------------
bool TManagerStateMachine::SetState(const char* sNewStateName, unsigned int id )
{
  if(id==eAll)
  {
    bool res = true;
    int cnt = mVecSM.size();
    for(int i = 0 ; i < cnt ; i++ )
    {
      res &= mVecSM.at(i)->Init(sNewStateName);
    }
    return res;
  }
  return mVecSM.at(id)->Init(sNewStateName);
}
//---------------------------------------------------------------------------------
bool TManagerStateMachine::GetComment(unsigned int id, unsigned int k, string& comment)
{
  TMapUintPtr::iterator fit = mVecSM.at(id)->GetDataCurrent()->find(k);
  if(fit!=mVecSM.at(id)->GetDataCurrent()->end())
  {
    comment = fit->second.comment;
    return true; 
  }
  return false;
}
//---------------------------------------------------------------------------------
