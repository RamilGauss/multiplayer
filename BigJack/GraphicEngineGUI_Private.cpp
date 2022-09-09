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

#include "GraphicEngineGUI_Private.h"
#include <stddef.h>

#ifdef WIN32
  #include "DXUT.h"
  #include "DXUTgui.h"
#endif

#include "MakerXML.h"
#include "IXML.h"
#include "BL_Debug.h"
#include "MakerGUI.h"
#include "GraphicEngineGUI.h"


#include "ButtonWin.h"

using namespace std;
using namespace nsTypeGUI;

TGraphicEngineGUI_Private::TGraphicEngineGUI_Private(TGraphicEngineGUI* _pMasterGUI)
{
  pMasterGUI = _pMasterGUI;
}
//--------------------------------------------------------------------------------
TGraphicEngineGUI_Private::~TGraphicEngineGUI_Private()
{
  int cnt = mVectorObjectGUI.size();
  for(int i = 0 ; i < cnt ; i++)
    delete mVectorObjectGUI[i];

  TMapPtrPtr::iterator bit = mMapPrivatePublicGUI.begin();
  TMapPtrPtr::iterator eit = mMapPrivatePublicGUI.end();
  while(bit!=eit)
  {
    IBaseGUI* pBaseGUI = static_cast<IBaseGUI*>(bit->second); 
    delete pBaseGUI;

    bit++;
  }
}
//--------------------------------------------------------------------------------
bool TGraphicEngineGUI_Private::Load(const char* sFullPathXML)
{
  TMakerGUI maker;
  CHECK_RET(maker.Load(sFullPathXML));
  int cnt = maker.GetCount();
  for(int i = 0 ; i < cnt ; i++ )
  {
    const TMakerGUI::TDefGUI* pDefGUI = maker.Get(i);
    TObjectGUI* pObject = new TObjectGUI;
    pObject->id         = mVectorObjectGUI.size();
    pObject->x          = pDefGUI->place[0];
    pObject->y          = pDefGUI->place[1];

    mMapNameObjectGUI.insert(TMapNameObject::value_type(pDefGUI->name,pObject));
    mVectorObjectGUI.push_back(pObject);
    mMapPrivatePublicGUI.insert(TMapPtrPtr::value_type(pDefGUI->pPrivateGUI,pDefGUI->pPublicGUI));

#ifdef WIN32
    CDXUTControl* pControl = (CDXUTControl*)pDefGUI->pPrivateGUI;
    pControl->SetID(pObject->id);
    BL_ASSERT(mPrivateDialog.AddControl(pControl)==S_OK);
#endif
  }

  return true;
}
//--------------------------------------------------------------------------------
void TGraphicEngineGUI_Private::Connect(const char* nameMember, tEvent event, int key)
{
  // ищем объект
  TMapNameObject::iterator fit = mMapNameObjectGUI.find(string(nameMember));
  if(fit==mMapNameObjectGUI.end())
  {
    BL_FIX_BUG();
    return;
  }
  TObjectGUI *pObj = fit->second;
  pObj->mapEvent.insert(TMultiMapObject::value_type(event,key));
}
//--------------------------------------------------------------------------------
void* TGraphicEngineGUI_Private::Get(const char* name)
{
  void* publicGUI = NULL;
  // по имени найти ID
  TMapNameObject::iterator fit = mMapNameObjectGUI.find(string(name));
  if(fit!=mMapNameObjectGUI.end())
  {
    unsigned int id = fit->second->id;
#ifdef WIN32
    void* privateGUI = mPrivateDialog.GetControl(id);
#else
#endif
    TMapPtrPtr::iterator fitPublicGUI = mMapPrivatePublicGUI.find(privateGUI);
    if(fitPublicGUI!=mMapPrivatePublicGUI.end())
      publicGUI = fitPublicGUI->second;
  }
  return publicGUI;
}
//--------------------------------------------------------------------------------
void TGraphicEngineGUI_Private::ResizeEvent(int Width, int Height)
{
  // в соответствии с настройками поменять положение элементов на форме.
  int cnt = mVectorObjectGUI.size();
  for( int i = 0 ; i < cnt ; i++ )
  {
    unsigned int id = mVectorObjectGUI[i]->id;
    int x = GetPlaceCoord(&mVectorObjectGUI[i]->x,Width);
    int y = GetPlaceCoord(&mVectorObjectGUI[i]->y,Height);
#ifdef WIN32
    void* privateGUI = mPrivateDialog.GetControl(id);
#else
#endif
    TMapPtrPtr::iterator fitPublicGUI = mMapPrivatePublicGUI.find(privateGUI);
    if(fitPublicGUI!=mMapPrivatePublicGUI.end())
    {
      void* publicGUI = fitPublicGUI->second;
      IBaseGUI* pBaseGUI = static_cast<IBaseGUI*>(publicGUI); 
      pBaseGUI->SetLocation(x,y);
    }
  }
}
//--------------------------------------------------------------------------------
void TGraphicEngineGUI_Private::Render(float elapsedtime)
{
  mPrivateDialog.OnRender(elapsedtime);
}
//--------------------------------------------------------------------------------
void TGraphicEngineGUI_Private::GUIEvent(unsigned int nEvent, int nControlID)
{
  // рассылка уведомлений
  // по ID найти имя компонента, по имени найти событие. 
  // Если события совпадают, то найти и вызвать адрес функции.
  TObjectGUI* pObj = mVectorObjectGUI[nControlID];

  TMultiMapObject::iterator fit = pObj->mapEvent.find((tEvent)nEvent);
  if(fit!=pObj->mapEvent.end())
  {
    TMultiMapObject::iterator lastElement = pObj->mapEvent.upper_bound((tEvent)nEvent);

    while(fit!=lastElement)
    {
      int key = fit->second;
      pMasterGUI->GUIEvent(key);
      fit++;
    }
  }
}
//--------------------------------------------------------------------------------
#ifdef WIN32
void TGraphicEngineGUI_Private::SetCallback(void* func)
{
  mPrivateDialog.SetCallback((PCALLBACKDXUTGUIEVENT)func, this);
}
//--------------------------------------------------------------------------------
bool TGraphicEngineGUI_Private::MsgProc(unsigned int _hWnd, 
                                unsigned int _uMsg, 
                                unsigned int _wParam, 
                                unsigned int _lParam)
{
  HWND hWnd = (HWND)_hWnd;
  UINT uMsg = (UINT)_uMsg;
  WPARAM wParam = (WPARAM)_wParam;
  LPARAM lParam = (LPARAM)_lParam;

  return mPrivateDialog.MsgProc(hWnd, uMsg, wParam, lParam);
}
//--------------------------------------------------------------------------------
#else
#endif
//--------------------------------------------------------------------------------
void TGraphicEngineGUI_Private::Init(void* ptr)
{
#ifdef WIN32
  mPrivateDialog.Init((CDXUTDialogResourceManager*)ptr);
#else
#endif
}
//--------------------------------------------------------------------------------
int TGraphicEngineGUI_Private::GetPlaceCoord(TMakerGUI::TPlaceGUI* v,int len)
{
  int res = 0;
  switch(v->type)
  {
    case TMakerGUI::min:
      switch(v->policy)
      {
        case TMakerGUI::fixed:
          res = v->value;
          break;
        case TMakerGUI::preffered:
          res = int((v->value*len)/100.0f);
          break;
      }
      break;
    case TMakerGUI::max:
      switch(v->policy)
      {
      case TMakerGUI::fixed:
        res = len - v->value;
        break;
      case TMakerGUI::preffered:
        res = len - int((v->value*len)/100.0f);
        break;
      }
      break;
  }
  return res;
}
//--------------------------------------------------------------------------------
