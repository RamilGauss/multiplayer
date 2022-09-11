/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#include "ManagerScenario.h"

#include <boost/foreach.hpp>

#include "IScenario.h"
#include "MakerScenario.h"

#include "BL_Debug.h"

using namespace std;
using namespace nsMelissa;

TManagerScenario::TManagerScenario(TManagerSession* pMS)
{
  pActiveScenario = NULL;
	mManagerSession = pMS;
}
//---------------------------------------------------------------------
TManagerScenario::~TManagerScenario()
{
  Done();
}
//---------------------------------------------------------------------
IScenario* TManagerScenario::Add(unsigned int ID_Implementation, string& name)
{
  IScenario* pScenario = Get(name);
  if(pScenario) 
  {
    BL_FIX_BUG();//???
    return pScenario;
  }
  pScenario = mMakerScenario.New(ID_Implementation);
  pScenario->Init(this, mManagerSession);
  mMapStrScenario.insert(TMapStrPtr::value_type(name, pScenario));
  return pScenario;
}
//---------------------------------------------------------------------
IScenario* TManagerScenario::Get(string& name)
{
  TMapStrPtrIt fit = mMapStrScenario.find(name);
  if(fit==mMapStrScenario.end())
    return NULL;
  return fit->second;
}
//---------------------------------------------------------------------
void TManagerScenario::Remove(string& name)
{
  IScenario* pScenario = Get(name);
  if(pScenario)
  {
    if(pScenario==pActiveScenario)
      Disactivate();

    mMapStrScenario.erase(name);
    mMakerScenario.Delete(pScenario);
  }
}
//---------------------------------------------------------------------
void TManagerScenario::Done()
{
	BOOST_FOREACH( TMapStrPtr::value_type& bit, mMapStrScenario )
    delete bit.second;

  mMapStrScenario.clear();
}
//---------------------------------------------------------------------
void TManagerScenario::Activate(IScenario* pNeedActivation)
{
  if(pActiveScenario)// если есть активный, то поместить в очередь на активацию
  {
    // даже если сейчас активен тот же сценарий, поместить в очередь
    mListWaitActivation.push_back(pNeedActivation);
    return;
  }
  pActiveScenario = pNeedActivation;
}
//---------------------------------------------------------------------
void TManagerScenario::Disactivate()
{
  BL_ASSERT(pActiveScenario);
  // следующий сценарий
  if(mListWaitActivation.size())
  {
    pActiveScenario = mListWaitActivation.front();
    mListWaitActivation.pop_front();
  }
  else
    pActiveScenario = NULL;
}
//---------------------------------------------------------------------
void TManagerScenario::Work()
{
  if(pActiveScenario)
    pActiveScenario->Work();
}
//---------------------------------------------------------------------
IScenario* TManagerScenario::GetActive()
{
  return pActiveScenario;
}
//---------------------------------------------------------------------
