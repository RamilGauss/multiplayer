#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include <list>
#include <algorithm>
#include "ApplStandIndication.h"


using namespace std;

template<class Type>
void Delete(Type& elem)
{
  delete elem;
}
void MyFunc()
{
  typedef std::list<int*> TListPtr;
  TListPtr l;
  l.push_back(new int);
  l.push_back(new int);

  for_each(l.begin(), l.end(), Delete<int*> );

  l.clear();
}

int main(int argc, char** argv)
{
  //std::vector<char> vec;
  //vec.push_back(0x8);	

  //TPacketTransportLevel_SI packet;
  //packet.SetData((unsigned char*)&vec[0],vec.size());
  //int sizeP;
  //const char* p = packet.GetPacket(sizeP);
  //TPacketTransportLevel_SI packet2;
  //packet2.SetPacket((unsigned char*)p,sizeP);

  //MyFunc();
  TApplStandIndication si;
  si.TryConnect();
  //si.Set();

  _getch();
  return 0;
}
