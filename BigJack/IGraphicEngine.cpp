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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "IGraphicEngine.h"
#include "HiTimer.h"
#include "GraphicEngineGUI_Private.h"

using namespace std;

//------------------------------------------------------------------------
int IGraphicEngine::GetCountGUI()
{
  return mListGUI.size();
}
//------------------------------------------------------------------------
void IGraphicEngine::AddGUI(TGraphicEngineGUI* pForm)
{
  mListGUI.push_back(pForm);
  void* pObjectForGUI = GetObjectForInitGUI();
  pForm->GetPrivate()->Init(pObjectForGUI);

  void* funcGUI = GetFuncEventGUI();
  pForm->GetPrivate()->SetCallback(funcGUI);
}
//------------------------------------------------------------------------
void IGraphicEngine::ClearGUI()
{
  mListGUI.clear();
}
//------------------------------------------------------------------------
void IGraphicEngine::ResetGUI(int Width, int Height)
{
  TListGUI::iterator bit = mListGUI.begin();
  TListGUI::iterator eit = mListGUI.end();
  while(bit!=eit)
  {
    (*bit)->GetPrivate()->ResizeEvent(Width, Height);
    bit++;
  }
}
//------------------------------------------------------------------------
void IGraphicEngine::RenderGUI()
{
  float elapsedtime = (float)ht_GetMSCount();
  TListGUI::iterator bit = mListGUI.begin();
  TListGUI::iterator eit = mListGUI.end();
  while(bit!=eit)
  {
    (*bit)->GetPrivate()->Render(elapsedtime);
    bit++;
  }
}
//------------------------------------------------------------------------
void IGraphicEngine::GUIEvent(unsigned int nEvent, int nControlID, void* pGUI)
{
  TListGUI::iterator bit = mListGUI.begin();
  TListGUI::iterator eit = mListGUI.end();
  while(bit!=eit)
  {
    if(pGUI==(*bit))
    {
      (*bit)->GetPrivate()->GUIEvent(nEvent,nControlID);
      break;
    }
    bit++;
  }
}
//------------------------------------------------------------------------
#ifdef WIN32
bool IGraphicEngine::MsgProcGUI(unsigned int hWnd, 
                                unsigned int uMsg, 
                                unsigned int wParam, 
                                unsigned int lParam)
{
  TListGUI::iterator bit = mListGUI.begin();
  TListGUI::iterator eit = mListGUI.end();
  while(bit!=eit)
  {
    if((*bit)->GetPrivate()->MsgProc(hWnd,uMsg,wParam,lParam)==false)
      return false;
    bit++;
  }
  return true;
}
#else
#endif
//------------------------------------------------------------------------
