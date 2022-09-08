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


#define DURATION_FIGHT_MINUTE 15 // ������������ ���,�����
#define DURATION_FIGHT_MS (DURATION_FIGHT_MINUTE*60*1000) // ������������ ���, ��
#define COUNT_COMMAND_IN_FIGHT 1// ������ ����� �� ������� � ���


namespace nsCallBackType
{
	// ���� callback �������
	enum{
		eRcvPacket  = 0,		   
		eRcvStream  = 1,
		eDisconnect = 2,
	};
}

#define NAME_TITTLE "������ \"Tanks\""
#define Q_MESSAGE(sError) \
QMessageBox::warning( NULL, QObject::tr(NAME_TITTLE),sError );

#define SERVER_NUMBER_OF_NETWORK 0 //  ����� ������� (� ������ ������� ���������� ������� ���������)


#define QEVENT_DIRECTX_EXIT       (QEvent::Type)(QEvent::User+0)
#define QEVENT_PACKET             (QEvent::Type)(QEvent::User+1)
#define QEVENT_STREAM             (QEvent::Type)(QEvent::User+2)
#define QEVENT_DISCONNECT         (QEvent::Type)(QEvent::User+3)


#define PATH_LIST_MODELS "..\\model\\listModel"
// �������� ������
// 1 ���� ��������: ID, �����������, ..\Model\Name\main.ini
// 2 ���� ����������                 ..\Model\Name\primitive
// ����� ������� � �������� �������� ��� ������ ������ ��������


#endif
