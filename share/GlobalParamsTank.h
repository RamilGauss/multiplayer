#ifndef GlobalParamsTankH
#define GlobalParamsTankH

#define ServerLocalPort 4321

#define VERSION_PROGRAMM ((unsigned short)21) // unsigned short

#define STR_VERSION_CLIENT "Клиент \"WarTanks\" v0.021 NET DX9"
#define STR_VERSION_SERVER "Сервер \"WarTanks\" v0.021 NET DX9"

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

#define NAME_TITTLE "Клиент \"WarTanks\""
#define Q_MESSAGE(sError) \
QMessageBox::warning( NULL, QObject::tr(NAME_TITTLE),sError );

#define SERVER_NUMBER_OF_NETWORK 0 //  выбор подсети (в случае наличия нескольких сетевых адаптеров)


#define QEVENT_DIRECTX_EXIT       (QEvent::Type)(QEvent::User+0)
#define QEVENT_PACKET             (QEvent::Type)(QEvent::User+1)
#define QEVENT_STREAM             (QEvent::Type)(QEvent::User+2)
#define QEVENT_DISCONNECT         (QEvent::Type)(QEvent::User+3)

#endif
