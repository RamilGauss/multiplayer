/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#ifndef GraphicEngineGUI_PrivateH
#define GraphicEngineGUI_PrivateH

#include "IButton.h"
#include "IEditBox.h"
#include "IStatic.h"
#include "ISlider.h"
#include "ICheckBox.h"
#include "IComboBox.h"
#include "IListBox.h"
#include "IRadioButton.h"
#include <map>
#include <string>
#include <vector>
#include "MakerGUI.h"


#ifdef WIN32
  #include "DXUT.h"
  #include "DXUTgui.h"
#else
#endif

class TGraphicEngineGUI;

class TGraphicEngineGUI_Private
{

#ifdef WIN32
  CDXUTDialog
#endif
  mPrivateDialog;// ������ ��������� �� ��������� ������� ������� IControl, IButton, etc.

  TGraphicEngineGUI* pMasterGUI;

public:

  TGraphicEngineGUI_Private(TGraphicEngineGUI* _pMasterGUI);
  virtual ~TGraphicEngineGUI_Private();

public:
  // GUI
  bool Load(const char* sFullPathXML);
  void Connect(const char* nameMember, nsTypeGUI::tEvent event, int key);
  void* Get(const char* name);


public:
  // GE
  void Init(void* ptr);
  void ResizeEvent(int Width, int Height);
  void Render(float elapsedtime);
  void GUIEvent(unsigned int nEvent, int nControlID);
#ifdef WIN32
  void SetCallback(void* func);
  bool MsgProc(unsigned int hWnd, 
               unsigned int uMsg, 
               unsigned int wParam, 
               unsigned int lParam);
#else

#endif

private:
  typedef std::multimap<nsTypeGUI::tEvent,int> TMultiMapObject;
  struct TObjectGUI
  {
    unsigned int id;
    TMultiMapObject mapEvent;
    TMakerGUI::TPlaceGUI x;
    TMakerGUI::TPlaceGUI y;
  };

  typedef std::map<std::string,TObjectGUI*> TMapNameObject;
  typedef std::vector<TObjectGUI*> TVectorObject;
  typedef std::map<void*,void*> TMapPtrPtr;

  TMapNameObject mMapNameObjectGUI;
  TVectorObject  mVectorObjectGUI;
  TMapPtrPtr     mMapPrivatePublicGUI;// ��� ������ �� ���������, � public ��������� ��������� �� private

  int GetPlaceCoord(TMakerGUI::TPlaceGUI* v,int len);
};


#endif
