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


#ifndef CallBackRegistratorH
#define CallBackRegistratorH

#include <boost/signals2/signal.hpp>
//-------------------------------------------------------------
template < typename T1=void,
           typename T2=void,
           typename T3=void,
           typename T4=void,
           typename T5=void,
           typename T6=void> 
class TCallBackRegistratorBase
{
protected:
  boost::signals2::signal<void (T1)> sig;
public:
  // �������� Register(&TClass::Method, &object)
  template<typename F, class A1>
  void Register(F pFunc, A1 pObject)
  {
    sig.connect(boost::bind(pFunc, pObject, _1));
  }
  // �������� Unregister(&object)
  template<class A1>
  void Unregister(A1 pObject)
  {
    sig.disconnect(pObject);
  }
};
//-------------------------------------------------------------
template<typename T1> 
class TCallBackRegistrator1 : public TCallBackRegistratorBase<T1>
{
public: void Notify(T1 t1){sig(t1);}
};
//-------------------------------------------------------------
template<typename T1,typename T2> 
class TCallBackRegistrator2 : public TCallBackRegistratorBase<T1,T2>
{
public: void Notify(T1 t1,T2 t2){sig(t1,t2);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3> 
class TCallBackRegistrator3 : public TCallBackRegistratorBase<T1,T2,T3>
{
public: void Notify(T1 t1,T2 t2,T3 t3){sig(t1,t2,t3);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3,typename T4> 
class TCallBackRegistrator4 : public TCallBackRegistratorBase<T1,T2,T3,T4>
{
public: void Notify(T1 t1,T2 t2,T3 t3,T4 t4){sig(t1,t2,t3,t4);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3,typename T4,typename T5> 
class TCallBackRegistrator5 : public TCallBackRegistratorBase<T1,T2,T3,T4,T5>
{
public: void Notify(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5){sig(t1,t2,t3,t4,t5);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> 
class TCallBackRegistrator6 : public TCallBackRegistratorBase<T1,T2,T3,T4,T5,T6>
{
public: void Notify(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6){sig(t1,t2,t3,t4,t5,t6);}
};
//-------------------------------------------------------------


#endif
