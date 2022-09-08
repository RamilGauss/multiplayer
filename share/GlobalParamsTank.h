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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef GlobalParamsTankH
#define GlobalParamsTankH

#define ServerLocalPort 4321

#define VERSION_PROGRAMM ((unsigned short)30) // unsigned short
#define VERSION_EDITOR   ((unsigned short)7)
//-------------------------------------------------------------------------------------------
extern char strVersionClient[100];
extern char strVersionServer[100];
extern char strVersionEditor[100];
//-------------------------------------------------------------------------------------------

#define STR_VERSION_CLIENT strVersionClient
#define STR_VERSION_SERVER strVersionServer
#define STR_VERSION_EDITOR strVersionEditor


#define DURATION_FIGHT_MINUTE 15 // длительность боя,минут
#define DURATION_FIGHT_MS (DURATION_FIGHT_MINUTE*60*1000) // длительность боя, мс
#define COUNT_COMMAND_IN_FIGHT 1// размер одной из команды в бою


namespace nsCallBackType
{
	// типы callback вызовов
	enum{
		eRcvPacket  = 0,		   
		eRcvStream  = 1,
		eDisconnect = 2,
	};
}

#define NAME_TITTLE "Клиент \"Tanks\""
#define Q_MESSAGE(sError) \
QMessageBox::warning( NULL, QObject::tr(NAME_TITTLE),sError );

#define SERVER_NUMBER_OF_NETWORK 0 //  выбор подсети (в случае наличия нескольких сетевых адаптеров)


#define QEVENT_DIRECTX_EXIT       (QEvent::Type)(QEvent::User+0)
#define QEVENT_PACKET             (QEvent::Type)(QEvent::User+1)
#define QEVENT_STREAM             (QEvent::Type)(QEvent::User+2)
#define QEVENT_DISCONNECT         (QEvent::Type)(QEvent::User+3)


#define PATH_LIST_MODELS "..\\model\\listModel"
// перечень файлов
// 1 Файл описания: ID, комментарии, ..\Model\Name\main.ini
// 2 Файл примитивов                 ..\Model\Name\primitive
// Файлы текстур и шейдеров задаются для каждой группы отдельно


#endif
