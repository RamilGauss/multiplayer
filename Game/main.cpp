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
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 
#ifdef WIN32
	#include <windows.h>
	#include <atlconv.h>
#endif 

#include <string>
#include <map>

#include "BL_Debug.h"
#include "ClientGame.h"
#include "ServerGame.h"
#include "InputCmdTornado.h"

using namespace std;

// ����������: ��������� ������� ���� ������������ (��� ������������� ��������� � �������
// �������������� ������, ���������� �������� ������ (*.bat))
typedef enum{
	    tUndef,
	    tC,
	    tS,
	    tM,
	    tSS,
    }eTypeRealize;

struct TTypeGame
{
  string       name;
  eTypeRealize t;
};

TTypeGame typeGame[]=
{
  {"c",  tC},
  {"s",  tS},
  {"m",  tM},
  {"ss", tSS},
};
//-------------------------------------
typedef map<string,eTypeRealize> TMapStrType;
typedef TMapStrType::iterator TMapIter;
typedef vector<string> TVectorStr;
TMapStrType mapTypeGame;
//-------------------------------------
void Init();
void ViewHowUse();
//-------------------------------------
#ifdef WIN32
bool GetArgvArgcWin(TVectorStr& vec_argv);
INT WINAPI wWinMain(HINSTANCE,HINSTANCE,LPWSTR,int )
#else
bool GetArgvArgcConsole(int argc, char** argv, TVectorStr& vec_argv)
int main(int argc, char** argv)
#endif
{
  Init();
  //-----------------------------------------------------------------
  TVectorStr vec_argv;
  bool resGet =
#ifdef WIN32
  GetArgvArgcWin(vec_argv);
#else
  GetArgvArgcConsole(argc, argv, vec_argv);
#endif
	TInputCmdTornado cmdTornado;
	if((resGet==false)||
		 (cmdTornado.SetArg(vec_argv)==false))
  {
    ViewHowUse();
    return -1;
  }
  //-----------------------------------------------------------------  
	TInputCmdTornado::TInput inputTornado;
	cmdTornado.Get(inputTornado);

	eTypeRealize typeRealize = tUndef;
  TMapIter fit = mapTypeGame.find(inputTornado.type);
  if(fit!=mapTypeGame.end())
    typeRealize = fit->second;
  //-----------------------------------------------------------------  
	IGame *pGame = NULL;
  switch(typeRealize)
  {
    case tUndef:
			ViewHowUse();
			break;
		case tC:
      pGame = new TClientGame;
      break;
    case tS:
			pGame = new TServerGame(TServerGame::eSlave);
      break;
    case tM:
			pGame = new TServerGame(TServerGame::eMaster);
      break;
    case tSS:
			pGame = new TServerGame(TServerGame::eSuperServer);
      break;
  }
	if(pGame)
		pGame->Work(inputTornado.variant_use, 
                inputTornado.libName.data(), 
                inputTornado.param.data());
	delete pGame;
  pGame = NULL;
  return 0;
}
//-------------------------------------------------------------------------------
void Init()
{
  int cnt = sizeof(typeGame)/sizeof(TTypeGame);
  for(int i = 0 ; i < cnt ; i++ )
    mapTypeGame.insert(TMapStrType::value_type(typeGame[i].name,typeGame[i].t));
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
  return bool(argv>0);
}
#endif
//-------------------------------------------------------------------------------
void ViewHowUse()
{
  BL_MessageBug(
    "������������ ���� ����������.\n"
    "���� -r ��� ������������ ���������� ����\n"
    "��������� �����:\n"
    "  c - ������\n"
    " ������:\n"
    "  s  - slave\n"
    "  m  - master\n"
    "  ss - super-server\n"
    "\n"
    "���� -d ��� ����������� ����������.\n"
    "\n"
    "���� -p ������, ������������ ���������� ����.\n"
    "\n"
    "���� -v �������, ������� ����� ����������� �� ����������,\n"
    "��-��������� ������������ 0, (��. GetXXXDeveloperTool(int variant).\n"
    "\n"
    "����� -d � -r �������� �������������.\n"
    "\n"
    "��������:\n"
    "Tornado.exe -v 0 -r c -d ..\\DeveloperToolDLL\\ViewerModel.dll -p \"ip=192.168.23.226 port=1000\"\n");
}
//-------------------------------------------------------------------------------
