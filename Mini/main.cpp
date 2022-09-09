#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include <list>
#include <algorithm>
#include <vector>
//#include "ApplStandIndication.h"


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
	std::list<int> v;
	v.push_back(0);
	v.push_back(1);
	//void* p = &(v.at(v.size()-1));
  std::list<int>::iterator p1 = v.begin();
  void* p2 = &v.front();
	v.push_back(2);
	*p1 = 1;
	std::list<int>::iterator p1 = v.begin();

  //std::vector<char> vec;
  //vec.push_back(0x8);	

  //TPacketTransportLevel_SI packet;
  //packet.SetData((unsigned char*)&vec[0],vec.size());
  //int sizeP;
  //const char* p = packet.GetPacket(sizeP);
  //TPacketTransportLevel_SI packet2;
  //packet2.SetPacket((unsigned char*)p,sizeP);

  //MyFunc();
  //TApplStandIndication si;
  //si.TryConnect();
  //si.Set();

  _getch();
  return 0;
}
