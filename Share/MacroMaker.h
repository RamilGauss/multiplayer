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

#ifndef DEF_MACRO_MAKER_H
#define DEF_MACRO_MAKER_H

// подготовка ( то, что для использования, смотри ниже )
#define MACRO_MAKER_H_DECL(DECL,Class,param_new_decl) \
class I##Class; \
class DECL TMaker##Class \
{ \
public: \
  I##Class* New(param_new_decl); \
  void Delete(I##Class*); \
};
//---------------------------------------------------------
#define MACRO_MAKER_H(Class,param_new_decl) \
MACRO_MAKER_H_DECL(,Class,param_new_decl)
//---------------------------------------------------------
#ifdef WIN32
  #define MACRO_MAKER_H_EXPORT(decl,Class,param_new_decl) \
  MACRO_MAKER_H_DECL(decl,Class,param_new_decl)
#else
  #define MACRO_MAKER_H_EXPORT(Class,param_new_decl) \
  MACRO_MAKER_H_DECL(,Class,param_new_decl)
#endif
//---------------------------------------------------------
//---------------------------------------------------------
#define MACRO_MAKER_CPP(Class,ReadyClass,param_new_decl,param_new_def) \
I##Class* TMaker##Class::New(param_new_decl) \
{ \
  return new ReadyClass(param_new_def); \
} \
void TMaker##Class::Delete(I##Class* ptr) \
{ \
  delete ptr;\
}
//---------------------------------------------------------
//---------------------------------------------------------
// готово для использования
#define MACRO_MAKER_H_USE_P(Class,param_new_decl) \
  MACRO_MAKER_H(Class,param_new_decl)
#define MACRO_MAKER_H_EXPORT_USE_P(decl,Class,param_new_decl) \
  MACRO_MAKER_H_EXPORT(decl,Class,param_new_decl)
#define MACRO_MAKER_CPP_USE_P(Class,ReadyClass,param_new_decl,param_new_def) \
  MACRO_MAKER_CPP(Class,ReadyClass,param_new_decl,param_new_def)

#define MACRO_MAKER_H_USE(Class) MACRO_MAKER_H(Class,)
#define MACRO_MAKER_H_EXPORT_USE(decl,Class) MACRO_MAKER_H_EXPORT(decl,Class,)
#define MACRO_MAKER_CPP_USE(Class,ReadyClass) MACRO_MAKER_CPP(Class,ReadyClass,,)


#endif