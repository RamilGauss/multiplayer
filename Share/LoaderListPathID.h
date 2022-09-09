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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef LoaderListPathIDH
#define LoaderListPathIDH

#include "LoadFromHDD.h"
#include "BL_ConfigFile.h"
#include <map>
#include <string>

/*
������ �����
[MAIN]
cnt=N
[PART0]  #[PART%d]
id=XX    # ����
path=ABC # �������� �����
....
*/

class TLoaderListPathID
{
  char sCurrentPath[260];
  TBL_ConfigFile iniReader;

public:
  TLoaderListPathID();
  ~TLoaderListPathID();

  bool Load(char* sPath, std::map<unsigned int,std::string>* pMap);

protected:

  bool LoadPart(int i, std::map<unsigned int,std::string>* pMap);
  bool FindCurrentPath(char* sPath);
};
#endif