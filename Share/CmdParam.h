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

#include <map>
#include <string>
#include <vector>
#include <set>

#include "TypeDef.h"

class SHARE_EI TCmdParam
{
  typedef std::multimap<std::string,std::string> TMultiMapStrStr;
  typedef TMultiMapStrStr::iterator TMultiMapStrStrIt;

  typedef std::set<std::string> TSetStr;
  typedef TSetStr::iterator TSetStrIt;

  TMultiMapStrStr mMapKey;
  TSetStr mSetDefKey;
  TSetStr mSetFoundKey;
public:
  TCmdParam();
  virtual ~TCmdParam();
  // задать список возможных ключей
  void SetDefKey(std::vector<std::string>& vecKey);
  // вставка элементов командной строки, парсинг по заданным ключам
  void SetArg(std::vector<std::string>& vecArgv);
  // есть ли такой ключ?
  bool IsKey(std::string& sKey);
  // определить кол-во значений, идущих после ключа в списке аргументов 
  // до следующего ключа или до окончания списка
  int  GetCountValueByKey(std::string& sKey);
  // получить строку по ключу
  bool GetByKey(std::string& sKey, int index, std::string& sOut);
protected:
  bool FindInDefKey( std::string& sKey );
};