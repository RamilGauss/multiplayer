/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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

#include <windows.h>
#include "ClientGame.h"
#include <string>
#include <map>
#include <atlconv.h>

using namespace std;

// Ќазначение: упростить отладку игры разработчику (нет необходимости создавать с дес€ток
// исполнительных файлов, достаточно написать батник (*.bat))
//-----------------------------------------
// ѕервый параметр:
// тип создаваемого воплощени€ игры
//   -coff - off-line-клиент

//   -con - on-line-клиент
//   -son - сервер дл€ малого кол-ва клиентов

// дл€ massive-on-line
//   -s - сервер slave 
//   -m - master-server
//   -ss - super-server
//-----------------------------------------
// ¬торой параметр:
// им€ DLL (сама библиотека должна лежать в папке ../DeveloperToolDLL/ )
//-----------------------------------------
// “ретий параметр:
// строка, адресованна€ воплощению игры
//-----------------------------------------
// Ќапример:
// Game.exe -con Client.dll "ip=192.168.23.226, port=1000"
// P.S.
// первый и второй параметр €вл€ютс€ об€зательными, третий - необ€зательный.
//-----------------------------------------
#define tCOFF 0
#define tCON  1
#define tSON  2
#define tS    3
#define tM    4
#define tSS   5

struct TTypeGame
{
  string name;
  int    t;
};

TTypeGame typeGame[]=
{
  {"-coff",tCOFF},
  {"-con", tCON},
  {"-son", tSON},
  {"-s",   tS},
  {"-m",   tM},
  {"-ss",  tSS},
};

map<string,int> mapTypeGame;
int typeGameKey = tCON;

void init();

INT WINAPI wWinMain(HINSTANCE,HINSTANCE,LPWSTR,int )
{
  init();
  //-----------------------------------------------------------------
  LPWSTR *szArglist;
  int nArgs;

  szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
  if( NULL == szArglist )
    return 0;
  if(nArgs<2)
  {
    LocalFree(szArglist);
    return 0;
  }
  //-----------------------------------------------------------------  
  USES_CONVERSION;
  map<string,int>::iterator fit = mapTypeGame.find(string(W2A(szArglist[1])));
  if(fit!=mapTypeGame.end())
    typeGameKey = fit->second;
  //-----------------------------------------------------------------  
  string sPathDLL = "..\\DeveloperToolDLL\\";
  sPathDLL += W2A(szArglist[2]);
  // Free memory allocated for CommandLineToArgvW arguments.
  LocalFree(szArglist);
  //-----------------------------------------------------------------  
  switch(typeGameKey)
  {
    case tCOFF:
      break;
    case tCON:
    {
      TClientGame game;
      game.Work(sPathDLL.data());
      break;
    }
    case tSON:
      break;
    case tS:
      break;
    case tM:
      break;
    case tSS:
      break;
  }

  return 0;
}
//-------------------------------------------------------------------------------
void init()
{
  int cnt = sizeof(typeGame)/sizeof(TTypeGame);
  for(int i = 0 ; i < cnt ; i++ )
    mapTypeGame.insert(map<string,int>::value_type(typeGame[i].name,typeGame[i].t));
}
//-------------------------------------------------------------------------------
