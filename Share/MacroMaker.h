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
the "TornadoEngine" Source Code.  If not, please request a copy in writing at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef DEF_MACRO_MAKER_H
#define DEF_MACRO_MAKER_H

#include "MacroMaker_Prepare.h"

//------------------------------------------------------------------------------------------------------------
// ������ ��� �������������
//------------------------------------------------------------------------------------------------------------
// � ����������� ������������
#define MACRO_MAKER_H_USE_P(NameInterface, NameRealize, param_for_decl) \
MACRO_MAKER_H(NameInterface, NameRealize, param_for_decl)

#define MACRO_MAKER_H_EXPORT_USE_P(decl, NameInterface, NameRealize, param_for_decl) \
MACRO_MAKER_H_EXPORT(decl, NameInterface, NameRealize, param_for_decl)

#define MACRO_MAKER_CPP_USE_P(NameInterface, NameRealize, ClassRealize, param_for_decl, param_for_def) \
MACRO_MAKER_CPP(NameInterface, NameRealize, ClassRealize, param_for_decl, param_for_def)
//------------------------------------------------------------------------------------------------------------
// ��� ����������
#define MACRO_MAKER_H_USE(NameInterface, NameRealize)                 MACRO_MAKER_H(NameInterface, NameRealize,)
#define MACRO_MAKER_H_EXPORT_USE(decl, NameInterface, NameRealize)    MACRO_MAKER_H_EXPORT(decl, NameInterface, NameRealize,)
#define MACRO_MAKER_CPP_USE(NameInterface, NameRealize, ClassRealize) MACRO_MAKER_CPP(NameInterface, NameRealize, ClassRealize,,)
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// ���� ��� ���������� ��������� � ������ ����������
#define MACRO_MAKER_H_USE_P_SAME(NameInterface, param_for_decl) \
MACRO_MAKER_H_USE_P(NameInterface, NameInterface, param_for_decl)

#define MACRO_MAKER_H_EXPORT_USE_P_SAME(decl, NameInterface, param_for_decl) \
MACRO_MAKER_H_EXPORT_USE_P(decl, NameInterface, NameInterface, param_for_decl)

#define MACRO_MAKER_CPP_USE_P_SAME(NameInterface, ClassRealize, param_for_decl, param_for_def) \
MACRO_MAKER_CPP_USE_P(NameInterface, NameInterface, ClassRealize, param_for_decl, param_for_def)
//------------------------------------------------------------------------------------------------------------
// ��� ����������
#define MACRO_MAKER_H_USE_SAME(NameInterface)                 MACRO_MAKER_H_USE(NameInterface, NameInterface)
#define MACRO_MAKER_H_EXPORT_USE_SAME(decl, NameInterface)    MACRO_MAKER_H_EXPORT_USE(decl, NameInterface, NameInterface)
#define MACRO_MAKER_CPP_USE_SAME(NameInterface, ClassRealize) MACRO_MAKER_CPP_USE(NameInterface, NameInterface, ClassRealize)
//------------------------------------------------------------------------------------------------------------

#endif