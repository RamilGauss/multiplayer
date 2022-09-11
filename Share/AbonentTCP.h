#ifndef _ABONENT_TCP_H_
#define _ABONENT_TCP_H_

#include "mutex.h"
#include "ShareMisc.h"
#include <sys/socket.h>
#include <sys/types.h>	
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <sys/time.h>
#include <bits/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <list>

#include "Container.h"
#include "SaveOnHDD.h"

#define MAXDATASIZE	4096


/*
	Данный класс предназначен для соединений по TCP, связь 1:1, всегда один сервер соединен с одним клиентом.
*/

class AbonentTCP : public Mutex
{
	TSaveOnHDD mLog;

public:
	typedef void (*TCallBack)(char*,int);
private:
	// для реализации механизма буферизации отправки
	// будет отправлено в том порядке, в каком добавлено на отправку
  typedef std::list<TContainer*> TListPtr;
	TListPtr mListSendData;// отправится потом, в рабочем потоке

	char* mBufferRead;
	int mSizeBufferRead;

	//socket
  char ip_txt[16];
  unsigned short port_number;
	int	mSocketLocal;//socket_tcp;
	int mSocketClient;// только для сервера
	struct sockaddr_in mSockAddrLocal;// сервер интерпретирует его как свой адрес и порт, клиента как адрес сервера
	struct sockaddr_in mSockAddrClient;// только для сервера

	
	//background thread
	friend void* ThreadFunctionTCP_frd( void* );
	void ThreadFunction();
	pthread_t  	thread_id;
  int cmd_end_thread;

	//receive data function
	//void cmdHandlerEMUTC(unsigned char* lpb, int sz);

	bool fl_connect;
	
	TCallBack mCallBack;
	
	int mTypeBehavior;// Server or client сервер или клиент
	int mResSend;
	int mResRecv;
	
public:
	enum{eServer=0, 
	     eClient=1,
			 };

	AbonentTCP(int typeBehavior, char* ip = "0.0.0.0", unsigned short np = 4245/*1025*/);
	~AbonentTCP();

	bool IsConnect(){return fl_connect;}
	void SetAddress(char* ip, unsigned short np);	//without restart thread
	int SendData( unsigned char* lpbyte, int sz_lpbyte );	//command for emulator TC
	void SetThreadCmd(int c){begin(); cmd_end_thread = c; end();}
	
	void SetCallBack(TCallBack callback);
protected:

	void CloseThread();
	
	void Print(char* msg);
	
	bool PrepareSocket();
	bool WaitConnect();// Client only
	bool IsAccept();// Server only
	void WaitData(int socket);
	bool SendData(int socket);
		
	bool PrepareSocketClient();
	bool PrepareSocketServer();

	
	void ClearBufferRead();
	void ReallocBufferRead(int size_data, int new_size);

	int GetSocket();
	void CloseSocketClient();
	
	bool IsConnectServer2Client();
	
};

#endif
