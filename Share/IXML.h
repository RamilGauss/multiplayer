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
the "Tanks" Source Code.  If not, please request a copy in writing at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef IXMLH
#define IXMLH

#include <stddef.h>
#include <string>

#define CHECK_RET(f) \
if(f==false) \
return false;

class IXML
{
public:
  IXML(){};
  virtual ~IXML(){};

  virtual bool Load(const char* sPath) = 0;
  virtual bool IsOpen() = 0;
  // операции с секциями
  
  // навигация
  virtual void ResetPos() = 0;
  virtual int  GetCountSection(const char* name = NULL) = 0;
  virtual std::string GetNameSection(int num) = 0;
  virtual bool EnterSection(const char* name, int num) = 0;
  virtual bool LeaveSection() = 0;
  // изменение кол-ва
  virtual bool AddChildSection(const char* childName, int num, const char* name);
  virtual bool RemoveChildSection(const char* childName, int numChild, const char* name, int num);

  virtual bool AddSection(const char* name) = 0;
  virtual bool RemoveSection(const char* name, int num) = 0;

  // изменение содержимого
  virtual bool AddXMLDeclaration(const char* strDecl) = 0;

  virtual bool WriteSectionAttr(const char* name, int num, const char* nameAttr, std::string buffer) = 0;
  virtual bool WriteSection(const char* name, int num, std::string buffer) = 0;
  // чтение
  virtual std::string ReadSectionAttr(const char* name, int num, const char* nameAttr) = 0;
  virtual std::string ReadSection(const char* name, int num) = 0;

  // сбросить содержимое изменений в файл
  virtual bool Save(const char* sPath = NULL) = 0;

  // доделанное чтение
  bool ReadFloat(const char* name, int num, float & v);
  bool ReadInt(const char* name, int num, int & v);
  bool ReadUint(const char* name, int num, unsigned int & v);
  bool ReadFloat3(const char* name, int num, float * v3);// разделитель или "space" или ";"
  bool ReadFloat4(const char* name, int num, float * v4);
};

#endif