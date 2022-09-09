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

#ifndef GraphicEngineGUIH
#define GraphicEngineGUIH

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
#include "SrcEvent.h"
#include "classTools.h"

#define GET_ELEMENT_DECL(NAME) \
I##NAME* Get##NAME(const char* name);

class TGraphicEngineGUI_Private;

class TGraphicEngineGUI : public TSrcEvent
{
  NO_COPY_OBJECT(TGraphicEngineGUI)

  friend class IGraphicEngine;
  TGraphicEngineGUI_Private* pPrivateGUI;
  TGraphicEngineGUI_Private* GetPrivate(){return pPrivateGUI;}

  void InitByGraphicEngine(void* ptr);// вызовет 

public:

  TGraphicEngineGUI();
  virtual ~TGraphicEngineGUI();

  virtual void Init() = 0;
  //----------------------------------------------------------------------
  GET_ELEMENT_DECL(Button)
  GET_ELEMENT_DECL(EditBox)
  GET_ELEMENT_DECL(Static)
  GET_ELEMENT_DECL(Slider)
  GET_ELEMENT_DECL(CheckBox)
  GET_ELEMENT_DECL(ComboBox)
  GET_ELEMENT_DECL(ListBox)
  GET_ELEMENT_DECL(RadioButton)
  //----------------------------------------------------------------------
  void GUIEvent(int key);
protected:
  // внутренняя инициализация
  bool Load(const char* sFullPathXML);

  // nameMember - имя компонента, например bOk - кнопка "Ок"
  // tEvent     - тип события GUI-компонента
  // key        - параметр для игрового движка (разработчик обработает его)
  void Connect(const char* nameMember, nsTypeGUI::tEvent event, int key);

  virtual const char* GetNameForm();
};


#endif
