/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 
#ifdef WIN32
#include <windows.h>
#include <atlconv.h>
#endif 

#include "ClientGame.h"
#include <string>
#include <map>

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
#define tUndef -1

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

typedef map<string,int> TMapStrInt;
typedef TMapStrInt::iterator TMapIter;
typedef vector<string> TVectorStr;
TMapStrInt mapTypeGame;
int typeGameKey = tUndef;

void init();

#ifdef WIN32
bool GetArgvArgcWin(TVectorStr& vec_argv);
INT WINAPI wWinMain(HINSTANCE,HINSTANCE,LPWSTR,int )
#else
bool GetArgvArgcConsole(int argc, char** argv, TVectorStr& vec_argv)
int main(int argc, char** argv)
#endif
{
  init();
  //-----------------------------------------------------------------
  TVectorStr vec_argv;
  if(
#ifdef WIN32
  GetArgvArgcWin(vec_argv)
#else
  GetArgvArgcConsole(argc, argv, vec_argv)
#endif
  ==false) return 0;

  if(vec_argv.size()<2)
    return 0;
  //-----------------------------------------------------------------  
  TMapIter fit = mapTypeGame.find(vec_argv.at(1));
  if(fit!=mapTypeGame.end())
    typeGameKey = fit->second;
  //-----------------------------------------------------------------  
  string argDevTool;
  if(vec_argv.size()>=4)
    argDevTool = vec_argv.at(3);
  //-----------------------------------------------------------------  
  switch(typeGameKey)
  {
    case tCOFF:
      break;
    case tCON:
    {
      TClientGame game;
      game.Work(vec_argv.at(2).data(), argDevTool.data());
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
    default:;
  }

  return 0;
}
//-------------------------------------------------------------------------------
void init()
{
  int cnt = sizeof(typeGame)/sizeof(TTypeGame);
  for(int i = 0 ; i < cnt ; i++ )
    mapTypeGame.insert(TMapStrInt::value_type(typeGame[i].name,typeGame[i].t));
}
//-------------------------------------------------------------------------------
#ifdef WIN32
bool GetArgvArgcWin(TVectorStr& vec_argv)
{
  bool res = false;
  LPWSTR *szArglist;
  int nArgs;

  szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
  if( szArglist != NULL )
  {
    USES_CONVERSION;
    for(int i = 0 ; i < nArgs ; i++)
      vec_argv.push_back(string(W2A(szArglist[i])));
    res = true;
  }

  LocalFree(szArglist);
  return res;
}
//-------------------------------------------------------------------------------
#else
bool GetArgvArgcConsole(int argc, char** argv, TVectorStr& vec_argv)
{
  for(int i = 0 ; i < argv ; i++)
    vec_argv.push_back(string(argv[i]));
}
#endif
//-------------------------------------------------------------------------------