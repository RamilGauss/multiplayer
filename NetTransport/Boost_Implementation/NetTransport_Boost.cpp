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

#include "NetTransport_Boost.h"

#include <boost/bind.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/connect.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "HiTimer.h"
#include "GCS.h"


using namespace std;

//----------------------------------------------------------------------------
void accept_handler(const boost::system::error_code& error)
{}
//----------------------------------------------------------------------------
class client
{
public:
  client()
  {}
  void handle_connect(const boost::system::error_code& error)
  {}
  void handle_write(const boost::system::error_code& error,size_t bytes_transferred)
  {}
  void handle_read(const boost::system::error_code& error,size_t bytes_transferred)
  {}
};
client g_client;
//----------------------------------------------------------------------------
GCS recursive_mutex;

void new_thread(void* pObject, int v)
{
  while(1)
  {
    recursive_mutex.lock();
    printf("thread!\n");
    recursive_mutex.unlock();

    boost::chrono::milliseconds ms(100);
    boost::this_thread::sleep_for( ms );
  }
}

TNetTransport_Boost::TNetTransport_Boost()
{
  unsigned int a = ht_GetMSCount();
  ht_msleep(80);
  a = ht_GetMSCount() - a;
  int asd = 0 ;


  boost::thread my_thread(new_thread, &g_client, 42);// g_thread_create
  // запустить новый поток, вызывающий функцию "hello_world"
  while(1)
  {
    recursive_mutex.lock();
    printf("main thread!\n");
    recursive_mutex.unlock();

    boost::chrono::milliseconds ms(1000);
    boost::this_thread::sleep_for( ms );
  }

  // init
  boost::asio::io_service      io_service;
  boost::asio::ip::tcp::socket mSocketUp(io_service);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // open
  const boost::asio::ip::address_v4 ipv4_address_Local = 
      boost::asio::ip::address_v4::from_string("192.168.23.226");
  const boost::asio::ip::address addr_Local(ipv4_address_Local);
  boost::asio::ip::tcp::endpoint endpoint_Local(addr_Local,1234);
  mSocketUp.open(endpoint_Local.protocol());
  mSocketUp.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));// reuse
  boost::asio::ip::tcp::no_delay option(true);// Nagle off 
  mSocketUp.set_option(option);
  mSocketUp.bind(endpoint_Local);
  // send
  std::string msg("asdasd");
  mSocketUp.send( boost::asio::buffer(msg.data(), sizeof(msg)));
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // connect - client
  const boost::asio::ip::address_v4 ipv4_addressRemote = boost::asio::ip::address_v4::from_string("192.168.23.206");
  const boost::asio::ip::address addrRemote(ipv4_addressRemote);
  boost::asio::ip::tcp::endpoint endpointRemote(addrRemote,1234);
  mSocketUp.connect(endpointRemote);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // listen
  boost::asio::ip::tcp::acceptor mSocketDown(io_service);
  mSocketDown.open(endpoint_Local.protocol());
  mSocketDown.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));// reuse
  boost::asio::ip::tcp::no_delay optionDown(true);// Nagle off 
  mSocketDown.set_option(optionDown);
  mSocketDown.bind(endpoint_Local);
  mSocketDown.listen(200000);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // register on event
  size_t bytes_transferred = 1024;
  char reply_[1024];
  boost::asio::async_read(mSocketUp, boost::asio::buffer( reply_, bytes_transferred),
    boost::bind( &client::handle_read,&g_client,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred)); 
  //boost::asio::async_write(mSocketUp, boost::asio::buffer( reply_, bytes_transferred),
  //  boost::bind( &client::handle_write,&g_client,
  //  boost::asio::placeholders::error,
  //  boost::asio::placeholders::bytes_transferred)); 
  //boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
  //boost::asio::async_connect(mSocketUp.lowest_layer(), endpoint_iterator,
  //  boost::bind(&client::handle_connect,&g_client,
  //  boost::asio::placeholders::error));

  boost::asio::ip::tcp::socket socket(io_service);
  mSocketDown.async_accept(socket, accept_handler);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // action on event 
  //-----------------------------------------------------------------
  // recv
  boost::asio::mutable_buffers_1 buffers(NULL,0);
  mSocketUp.receive(buffers);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // accept - server
  boost::asio::ip::tcp::socket newSocket(io_service);
  boost::asio::ip::tcp::endpoint newEndpoint;
  mSocketDown.accept(newSocket,newEndpoint);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~
  // close
  mSocketUp.close();
}
//----------------------------------------------------------------------------------
TNetTransport_Boost::~TNetTransport_Boost()
{
  Done();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Done()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport_Boost::Open(unsigned short port, unsigned char numNetWork)
{
  bool res = true;

  return res;
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Send(unsigned int ip, unsigned short port, 
							                   TBreakPacket packet, bool check)
{
  lock();

  unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Start()
{

}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Stop()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport_Boost::IsActive()
{
	return false;
}
//--------------------------------------------------------------------------
void TNetTransport_Boost::Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{

}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{

}
//----------------------------------------------------------------------------------
bool TNetTransport_Boost::Connect(unsigned int ip, unsigned short port)
{
  lock();
  bool res = true;
  unlock();
  return res;
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Close(unsigned int ip, unsigned short port)
{

}
//----------------------------------------------------------------------------------
