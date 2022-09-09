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

#include "MakerGUI.h"

#ifdef WIN32
  #include "DXUT.h"
  #include "ButtonWin.h"
  #include "CheckBoxWin.h"
  #include "ComboBoxWin.h"
  #include "EditBoxWin.h"
  #include "ListBoxWin.h"
  #include "StaticWin.h"
  #include "SliderWin.h"
  #include "RadioButtonWin.h"
#else
#endif

#include <string>
#include "common_defs.h"
#include "MakerXML.h"

using namespace nsTypeGUI;
using namespace std;

const char* strClass     = "class";
const char* strName      = "name";
const char* strGeometry  = "geometry";
const char* strWidth     = "w";
const char* strHeight    = "h";
const char* strPlace     = "place";
const char* strBind      = "bind";
const char* strType      = "type";
const char* strPolicy    = "policy";
const char* strValue     = "value";
const char* strProperty  = "property";
const char* strNode      = "node";
//----------------------------------------------------------------------
// параметры GUI
const char* strParamText = "text";

#ifdef WIN32
#define CREATE_GUI(classname) \
  pBaseGUI = new classname##Win; \
  pCurBaseGUI_Private = ((classname##Win*)pBaseGUI)->GetPrivate();
#else
#endif

//----------------------------------------------------------------------
struct TDefClassGUI
{
  string className;
  tTypeGUI type;
};
//----------------------------------------------------------------------
TDefClassGUI defClassGUI[] =
{
  {"Button",     tButton},
  {"CheckBox",   tCheckBox},
  {"ComboBox",   tComboBox},
  {"EditBox",    tEditBox},
  {"ListBox",    tListBox},
  {"RadioButton",tRadioButton},
  {"Slider",     tSlider},
  {"Static",     tStatic},
};
//----------------------------------------------------------------------
//----------------------------------------------------------------------
TMakerGUI::TMakerGUI()
{
  TMakerXML maker;
  mXML = maker.New();

  int cnt = ARRAY_SIZE(defClassGUI);
  for(int i = 0 ; i < cnt ; i++)
    mMap.insert(TMapClassNameTypeGUI::value_type(defClassGUI[i].className,defClassGUI[i].type));
}
//----------------------------------------------------------------------
TMakerGUI::~TMakerGUI()
{
  delete mXML;
}
//----------------------------------------------------------------------
IBaseGUI* TMakerGUI::New(tTypeGUI typeGUI)
{
  IBaseGUI* pBaseGUI = NULL;
  switch(typeGUI)
  {
    case tButton:
      CREATE_GUI(TButton)
      break;
    case tCheckBox:
      CREATE_GUI(TCheckBox);
      break;    
    case tComboBox:
      CREATE_GUI(TComboBox)
      break;
    case tEditBox:
      CREATE_GUI(TEditBox)
      break;
    case tListBox:
      CREATE_GUI(TListBox)
      break;
    case tRadioButton:
      CREATE_GUI(TRadioButton)
      break;
    case tSlider:
      CREATE_GUI(TSlider)
      break;
    case tStatic:
      CREATE_GUI(TStatic)
      break;
  }
  return pBaseGUI;
}
//----------------------------------------------------------------------
IBaseGUI* TMakerGUI::New(string nameClassGUI)
{
  TMapClassNameTypeGUI::iterator fit = mMap.find(nameClassGUI);
  IBaseGUI* pBaseGUI = NULL;
  if(fit!=mMap.end())
  {
    tTypeGUI typeGUI = fit->second;
    pBaseGUI = New(typeGUI);
  }
  return pBaseGUI;
}
//----------------------------------------------------------------------
bool TMakerGUI::Load(const char* sFullPathXML)
{
  pCurBaseGUI = NULL;
  pCurBaseGUI_Private = NULL;
  if(LoadGUI(sFullPathXML)==false)
  {
    delete pCurBaseGUI;// не удалось загрузить
    return false;
  }
  return true;
}
//----------------------------------------------------------------------
bool TMakerGUI::LoadGUI(const char* sFullPathXML)
{
  CHECK_RET(mXML->Load(sFullPathXML))

  string sRoot = mXML->GetNameSection(0);
  CHECK_RET(mXML->EnterSection(sRoot.data(),0))
    //-------------------------------------------------
  int cntNode = mXML->GetCountSection(strNode);
  for(int i = 0 ; i < cntNode ; i++ )
  {
    CHECK_RET(mXML->EnterSection(strNode,i))
      // содержимое нода отправить на анализ
      CHECK_RET(AddDef(i))
    CHECK_RET(mXML->LeaveSection())
  }
  return true;
}
//----------------------------------------------------------------------
int TMakerGUI::GetCount()
{
  return mVectorDefGUI.size();
}
//----------------------------------------------------------------------
const TMakerGUI::TDefGUI* TMakerGUI::Get(int i) const
{
  return &mVectorDefGUI[i];
}
//----------------------------------------------------------------------
bool TMakerGUI::AddDef(int i)
{
  TDefGUI vDef;
  // обязательные параметры: class и имя
  //--------------------------------------------------
  string sClass = mXML->ReadSection(strClass,0);
  CHECK_RET(sClass.data())
  IBaseGUI* pBaseGUI = New(sClass);
  pCurBaseGUI = pBaseGUI;
  CHECK_RET(pBaseGUI)
  vDef.pPublicGUI  = pBaseGUI;
  vDef.pPrivateGUI = pCurBaseGUI_Private;
  //--------------------------------------------------
  string sName = mXML->ReadSection(strName,0);
  CHECK_RET(sName.data())
  vDef.name = sName;
  //--------------------------------------------------
  // описание политики и положения
  // Ширина и высота
  CHECK_RET(mXML->EnterSection(strGeometry,0))
    int width = 0,height = 0;
    mXML->ReadInt(strWidth,0,width);
    mXML->ReadInt(strHeight,0,height);
    pBaseGUI->SetSize(width,height);
  CHECK_RET(mXML->LeaveSection())
  //--------------------------------------------------
  // x
  CHECK_RET(ReadPlace(vDef,0))
  // y
  CHECK_RET(ReadPlace(vDef,1))
  //--------------------------------------------------
  if(mXML->GetCountSection(strProperty)!=0)// определены ли доп. свойства
  {
    CHECK_RET(mXML->EnterSection(strProperty,0))
      SetupSpecificPropertyGUI(pBaseGUI);
    CHECK_RET(mXML->LeaveSection())
  }
  //--------------------------------------------------
  mVectorDefGUI.push_back(vDef);
  return true;
}
//----------------------------------------------------------------------
bool TMakerGUI::ReadPlace(TDefGUI &vDef, int i)
{
  CHECK_RET(mXML->EnterSection(strPlace,0))
  //-----------------------------------------  
    CHECK_RET(mXML->EnterSection(strBind,i))
    //-----------------------------------------  
    int type=0,policy=0,value=0;
    mXML->ReadInt(strType,0,type);    vDef.place[i].type   = (tTypeBindGUI)type;
    mXML->ReadInt(strPolicy,0,policy);vDef.place[i].policy = (tPolicyGUI)policy;
    mXML->ReadInt(strValue,0,value);  vDef.place[i].value  = value;
    //-----------------------------------------  
    CHECK_RET(mXML->LeaveSection())
  //-----------------------------------------
  CHECK_RET(mXML->LeaveSection())
  return true;
}
//----------------------------------------------------------------------
void TMakerGUI::SetupSpecificPropertyGUI(IBaseGUI* pBaseGUI)
{
  int cnt = mXML->GetCountSection();
  for(int i = 0 ; i < cnt ; i++ )
  {
    string nameParam = mXML->GetNameSection(i);
    string param = mXML->ReadSection(nameParam.data(),0);
    SetParam(nameParam.data(),param.data(),pBaseGUI);
  }
}
//----------------------------------------------------------------------
void TMakerGUI::SetParam(const char* nameParam,const char* param,IBaseGUI* pBaseGUI)
{
  // позже расширить функционал 
  switch(pBaseGUI->GetType())
  {
    case tButton:
    {
      IButton* pButton = (IButton*)pBaseGUI;
      if(strcmp(nameParam,strParamText)==0)
        pButton->SetText(param);
      break;
    }
    case tCheckBox:
      break;    
    case tComboBox:
      break;
    case tEditBox:
      break;
    case tListBox:
      break;
    case tRadioButton:
      break;
    case tSlider:
      break;
    case tStatic:
      break;
  }
}
//----------------------------------------------------------------------
