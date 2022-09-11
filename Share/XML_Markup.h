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

#ifndef XML_MarkupH
#define XML_MarkupH

#include "IXML.h"
#include "Markup.h"

#define A2UTF8  CMarkup::AToUTF8
#define UTF82A  CMarkup::UTF8ToA

/*
  реализация XML-парсера на CMarkUp.
*/

class TXML_Markup : public IXML
{
protected:
  CMarkup mMarkup;
  std::string mStrPath;

public:
  TXML_Markup(){};
  virtual ~TXML_Markup(){};

  virtual bool Load(const char* sPath);
  virtual bool IsOpen();
  // операции с секциями
  
  // навигация
  virtual void ResetPos();
  virtual int  GetCountSection(const char* name = NULL);
  virtual std::string GetNameSection(int index);
  virtual bool EnterSection(const char* name, int num);
  virtual bool LeaveSection();
  // изменение кол-ва
  virtual bool AddSectionAndEnter(const char* name);
  virtual bool AddSectionAndEnter(const char *name, int numAttr, TAttrInfo *pAttribs);
  virtual bool AddSection(const char* name);
  virtual bool AddSection(const char *name, int numAttr, TAttrInfo *pAttribs);
  virtual bool RemoveSection(const char* name, int num);

  // изменение содержимого
  virtual bool AddXMLDeclaration(const char* strDecl);

  virtual bool WriteSectionAttr(const char* name, int num, const char* nameAttr, std::string buffer);
  virtual bool WriteSection(const char* name, int num, std::string buffer);
  
  virtual bool WriteSectionAttr(int index, const char* nameAttr, std::string buffer);
  virtual bool WriteSection(int index, std::string buffer);
  // чтение
  virtual std::string ReadSectionAttr(const char* name, int num, const char* nameAttr );
  virtual std::string ReadSection(const char* name, int num);
  
  virtual std::string ReadSectionAttr(int index, const char* nameAttr );
  virtual std::string ReadSection(int index);

  virtual bool Save(const char* sPath = NULL);
};

#endif