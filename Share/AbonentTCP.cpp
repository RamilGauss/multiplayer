#include <string.h>
#include <stdio.h>
#include "AbonentTCP.h"
#include <errno.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <asm/ioctls.h> 
#include <sys/ioctl.h>
#include "protocol_reo.h"
#include <fcntl.h>

using namespace nsShareMisc;


#define PRINTF mLog.WriteF_time

//for test
void showData(unsigned char* lpByte, int sz)
{
  printf("\n");
  for (int i = 0; i < sz; ++i)
    printf(" %02x", lpByte[i]);    

	printf("\n");
}
//----------------------------------------------------------------------------------------
void* ThreadFunctionTCP_frd(void* p)
{
	AbonentTCP* ptr = (AbonentTCP*)p;
	ptr->ThreadFunction();
	return 0;
}
//----------------------------------------------------------------------------------------
AbonentTCP::AbonentTCP(int typeBehavior, char* ip, unsigned short np)
{
	mLog.SetPrintf(true);
	mLog.SetBufferization(false);
	mLog.SetEnable(false);	// выключить отладку

	mResRecv = 0;
	mResSend = 0;
	mSizeBufferRead = 0;
	mBufferRead = NULL;
	ReallocBufferRead(0,1024);

	mTypeBehavior = typeBehavior;
	mCallBack = NULL;
	fl_connect = false;
	cmd_end_thread = 0;
	mSocketLocal = -1;
	
	SetAddress(ip, np);
	pthread_create( &thread_id, NULL, ThreadFunctionTCP_frd, this );
}
//----------------------------------------------------------------------------------------
AbonentTCP::~AbonentTCP()
{
	CloseThread();
	begin();
	//-----------------------------------------------------------
	TListPtr::iterator bit = mListSendData.begin();
	TListPtr::iterator eit = mListSendData.end();	
	while(bit!=eit)
	{
		delete (*bit);
		bit++;
	}
	mListSendData.clear();
	//-----------------------------------------------------------
	end();
	
	ClearBufferRead();
}
//----------------------------------------------------------------------------------------
void AbonentTCP::CloseThread()
{
	cmd_end_thread = 1;
	pthread_cancel(thread_id);
	pthread_join(thread_id, NULL);
}
//----------------------------------------------------------------------------------------
void AbonentTCP::SetAddress(char *ip, unsigned short np)
{
	memcpy(ip_txt, ip, sizeof(ip_txt));
	port_number = np;

	bzero(&mSockAddrLocal, sizeof(mSockAddrLocal));
  mSockAddrLocal.sin_family = AF_INET;
  mSockAddrLocal.sin_port = htons(port_number);
  inet_aton( ip_txt, &mSockAddrLocal.sin_addr );
	
	char str[300];
	sprintf(str,"server IP: %s\n", ip);
	Print(str);
}
//----------------------------------------------------------------------------------------
int AbonentTCP::SendData( unsigned char *lpbyte, int sz_lpbyte)
{
	begin();// блокировка использования списка на отправление

	TContainer* sendData = new TContainer;
	sendData->SetData((char*)lpbyte,sz_lpbyte); // + GetData(), + Done()
	mListSendData.push_back(sendData);// отправится потом, в рабочем потоке
	PRINTF("AbonentTCP: List push_back. \n");
	
	end();
	return 0;
}
//----------------------------------------------------------------------------------------
static int All_send_data = 0;
bool AbonentTCP::SendData(int socket)
{
	bool res = true;
	begin();

	TListPtr::iterator bit = mListSendData.begin();
	TListPtr::iterator eit = mListSendData.end();	
  TListPtr::iterator nit;		
	while(bit!=eit)
	{
		int size = 0;
		char* data = (*bit)->GetData(size);
		//----------------------------------------------
		All_send_data += size;
		PRINTF("TContainer TCP all = %d\n",All_send_data);
		//----------------------------------------------
		mResSend = send( socket, data, size, 0 );
		res &= ( mResSend != -1 );

		delete (*bit);
		nit = bit;
    nit++;
    mListSendData.erase(bit);
    bit = nit;

		if(res==false)
			break;
	}
	
	end();
  return res;	
}
//----------------------------------------------------------------------------------------
void AbonentTCP::ThreadFunction()
{
  Print("Start thread");	
	// в общем виде что для сервера что для клиента:
	if(PrepareSocket()==false) {Print("Prepare socket FAIL!!!");return;}
  Print("Prepare socket OK");	
		
	if(WaitConnect()==false) {Print("Wait connect FAIL!!!");return;}// Client only
	Print("Wait connect OK");
	
	while(cmd_end_thread==0)
	{
		if(IsAccept())// Server only
		{
			do // для сервера цикл чтения-запись закончится только при разрыве связи с клиентом или при внешней команде "стоп"
			{
				WaitData(GetSocket());
				SendData(GetSocket());
			}while(IsConnectServer2Client()&&cmd_end_thread==0);
			CloseSocketClient();			
		}
	}
	close(mSocketLocal);
  Print("Stop");
}
//----------------------------------------------------------------------------------------
void AbonentTCP::SetCallBack(TCallBack callBack)
{
	mCallBack = callBack;
}
//-------------------------------------------------------------------------------------
void AbonentTCP::Print(char* msg)
{
	if(mTypeBehavior==eServer)
		PRINTF("TCP Server %s\n",msg);	
	else
		PRINTF("TCP Client %s\n",msg);	
}
//---------------------------------------------------------------------------------------------	
bool AbonentTCP::PrepareSocket()
{
	mSocketLocal = socket(AF_INET, SOCK_STREAM, 0 );
	if(mSocketLocal==-1)
	{
		Print("Fail socket()");
		return false;
	}

	switch(mTypeBehavior)
	{
		case eServer:
			return PrepareSocketServer();
		case eClient:
			return PrepareSocketClient();
		default:;
	}
	return false;
}
//---------------------------------------------------------------------------------------------			
bool AbonentTCP::PrepareSocketClient()
{
	return true;
}
//---------------------------------------------------------------------------------------------			
bool AbonentTCP::PrepareSocketServer()
{
	if( bind(mSocketLocal, (sockaddr*) &mSockAddrLocal, sizeof(mSockAddrLocal) ) == -1)
	{
		Print(strerror(errno));	
		return false;
	}

	//создаем прослушивающий сокет
	listen(mSocketLocal, 1);// 1 - максимальное кол-во соединений, в данном случае связь 1:1

	long data;
	fcntl(mSocketLocal, F_GETFL, &data);
	data |= O_NONBLOCK;
	fcntl(mSocketLocal, F_SETFL, data);

	return true;
}
//---------------------------------------------------------------------------------------------			
bool AbonentTCP::WaitConnect()// Client only
{
	if(mTypeBehavior!=eClient) return true;
	
	while(connect(mSocketLocal, (sockaddr*)&mSockAddrLocal, sizeof(mSockAddrLocal)) == -1)
  {
		if (cmd_end_thread)
			return false;
		usleep(100000);
  }

  fl_connect = true;

	return true;
}
//---------------------------------------------------------------------------------------------	
bool AbonentTCP::IsAccept()// Server only
{
	if(mTypeBehavior!=eServer) return true;
	
	if(cmd_end_thread)
		return false;
		
	socklen_t addrlen = sizeof(mSockAddrClient);
	if ((mSocketClient = accept(mSocketLocal, (sockaddr*)&mSockAddrClient, &addrlen)) != -1)
	{
		Print("Accept OK");
		return true;
	}
	
	return false;
}
//---------------------------------------------------------------------------------------------	
void AbonentTCP::WaitData(int socket)
{
	pollfd pfd;
	pfd.fd = socket;
	pfd.events = POLLIN;

	if((poll(&pfd, 1, 100) >= 0) && (pfd.revents == POLLIN))	//best for time
	{
		int cnt_recv = 0;
l_try_recv:
		int	sz = recv( socket, mBufferRead+cnt_recv, mSizeBufferRead-cnt_recv, MSG_DONTWAIT );
		mResRecv = sz;
		if(sz>0) PRINTF("Read %d, all=%d sBuf=%d\n",sz,cnt_recv,mSizeBufferRead);
		cnt_recv += sz;
		if(cnt_recv>=mSizeBufferRead)
		{
			int max_recv = mSizeBufferRead;// больше ведь прочитать не смогли
			ReallocBufferRead(mSizeBufferRead,cnt_recv);
			cnt_recv = max_recv;
			goto l_try_recv;
		}
		
		if(cnt_recv > 0)
		{
		  //PRINTF("call CallBack cnt=%d\n",cnt_recv);
			if(mCallBack)
				mCallBack( mBufferRead, cnt_recv);
		}
	} 
}
//---------------------------------------------------------------------------------------------	
void AbonentTCP::ClearBufferRead()
{
	delete[] mBufferRead;
	mBufferRead = NULL;
	mSizeBufferRead = 0;
}
//---------------------------------------------------------------------------------------------	
void AbonentTCP::ReallocBufferRead(int size_data, int new_size)
{
	PRINTF("ReallocBufferRead(%d,%d)\n",	size_data, new_size);
	
	char* buffer = NULL;
	if(size_data!=0)
	{
		buffer = new char[size_data];
		memcpy(buffer,mBufferRead,size_data);
	}
	ClearBufferRead();	
	mSizeBufferRead = new_size*2;
	mBufferRead = new char[mSizeBufferRead];
	if(size_data!=0)
	{
		if(new_size<size_data)
		{
			Print("new_size<size_data!!!!!");
			size_data = new_size;
		}
		memcpy(mBufferRead,buffer,size_data);
	}
	delete[] buffer;
}
//---------------------------------------------------------------------------------------------	
int AbonentTCP::GetSocket()
{
	switch(mTypeBehavior)
	{
		case eServer:
			return mSocketClient;
		case eClient:
			return mSocketLocal;
		default:;
	}
	return -1;
}
//---------------------------------------------------------------------------------------------	
void AbonentTCP::CloseSocketClient()
{
	if(mTypeBehavior!=eServer) return;
	close(mSocketClient);
}
//---------------------------------------------------------------------------------------------	
bool AbonentTCP::IsConnectServer2Client()
{
	bool res = (mTypeBehavior==eServer);
	if(res==false) return false;
	if((mResRecv==-1)||
	   (mResSend==-1))
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------	
//---------------------------------------------------------------------------------------------	