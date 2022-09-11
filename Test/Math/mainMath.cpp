/*
	Эксперименты по замене DirectX математических функций в 
	Struct3D на boost-вские.
*/


#include "Struct3D.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/operation.hpp> 
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <queue>
#include "SetOrderElement.h"
#include "MapCallBack.h"

using namespace nsStruct3D;

using namespace boost::numeric::ublas;

//--------------------------------------------------------------
class TA : public TMapCallBack
{
  TCallBackRegistrator0                            c0;
  TCallBackRegistrator1<unsigned int>              c1;
  TCallBackRegistrator2<unsigned int,unsigned int> c2;
public:
  TA();
  void Func();
};
//--------------------------------------------------------------
TA::TA()
{
  AddCallBack(0, &c0);
  AddCallBack(1, &c1);
  AddCallBack(2, &c2);
}
//--------------------------------------------------------------
void TA::Func()
{
  Notify(0);
  Notify<unsigned int>(1,0);
  Notify<unsigned int,unsigned int>(2,0,0);
}
//--------------------------------------------------------------
class TB
{
  TA mA;
public:
  TB();

  void C0()
  {
    int x = 0;
  };
  void C1(unsigned int a)
  {
    int x = 0;
  };
  void C2(unsigned int a, unsigned int b)
  {
    int x = 0;
  };
};
//--------------------------------------------------------------
TB::TB()
{
  mA.Register(0, &TB::C0, this);
  mA.Register<unsigned int>(1, &TB::C1, this);
  mA.Register<unsigned int,unsigned int>(2, &TB::C2, this);
  mA.Func();
}
//--------------------------------------------------------------
int main(int argc, char** argv)
{
	//identity_matrix<float> ident_m(4,4);
	//matrix<float> m(4,4);
	//m = ident_m;
	//float* pM = (float*)&m(0,0);
	//int size1 = m.size1();
	//int size2 = m.size2();
	//std::cout << m << std::endl;


	//TMatrix16 matrix;
	//TMatrix16* pResMatrix = SetMatrixIdentity(&matrix);

  TB b;

	return 0;
}